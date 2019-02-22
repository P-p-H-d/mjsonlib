/*
 * MJSONLIB - SMALL JSON LIBRARY based on M*LIB
 *
 * Copyright (c) 2018, Patrick Pelissier
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * + Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * + Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>

#include "m-array.h"
#include "m-dict.h"
#include "m-string.h"
#include "m-variant.h"

#include "mjsonlib.h"

/* First let's handle the OUT/IN functions of boolean */
static inline void boolean_out_str(FILE *f, bool b)
{
  fprintf(f, b ? "true" : "false");
}

static inline bool boolean_in_str(bool *b, FILE *f)
{
  char c = fgetc(f);
  if (c == 't') {
    *b = true;
    c = fgetc(f);
    if (c != 'r') return false;
    c = fgetc(f);
    if (c != 'u') return false;
    c = fgetc(f);
    return c == 'e';
  } else if (c == 'f') {
    *b = false;
    c = fgetc(f);
    if (c != 'a') return false;
    c = fgetc(f);
    if (c != 'l') return false;
    c = fgetc(f);
    if (c != 's') return false;
    c = fgetc(f);
    return c == 'e';
  } else {
    return false;
  }
}

/* Then the OUT/IN functions of float */
static inline void real_out_str(FILE *f, float r)
{
  fprintf(f, "%f", (double) r);
}

static inline bool real_in_str(float *r, FILE *f)
{
  return fscanf(f, "%f", r) == 1;
}

/* Use of M*LIB to define the structure of a JSON */
#define JSON_OPLIST                                                     \
  (INIT(json_init M_IPTR), CLEAR(json_clear),                           \
   INIT_SET(json_init_set M_IPTR), SET(json_set M_IPTR),                \
   OUT_STR(json_out_str), IN_STR(json_in_str M_IPTR) )

DICT_DEF2(dict_json, string_t, STRING_OPLIST, mjson_t, JSON_OPLIST)

ARRAY_DEF(array_json, mjson_t, JSON_OPLIST)

VARIANT_DEF2(variant_json,
             (boolean, bool, M_OPEXTEND(M_DEFAULT_OPLIST, OUT_STR(boolean_out_str), IN_STR(boolean_in_str M_IPTR)) ),
             (real, float, M_OPEXTEND (M_DEFAULT_OPLIST, OUT_STR(real_out_str), IN_STR(real_in_str M_IPTR))),
             (string, string_t, STRING_OPLIST),
             (array, array_json_t, ARRAY_OPLIST(array_json, JSON_OPLIST)), 
             (dict, dict_json_t, DICT_OPLIST(dict_json, STRING_OPLIST, JSON_OPLIST)))

/* Let's complete the structure */
struct mjson_node_s {
  variant_json_t json;
};

void json_init(mjson_t *p)
{
  assert (p != NULL);
  *p = malloc (sizeof (struct mjson_node_s));
  if (*p == NULL) abort();
  variant_json_init((*p)->json);
}

void json_clear (mjson_t p)
{
  assert (p != NULL);
  variant_json_clear (p->json);
  free(p);
}

void json_init_set(mjson_t *p, mjson_t o)
{
  assert (p != NULL && o != NULL);
  *p = malloc (sizeof (struct mjson_node_s));
  if (*p == NULL) abort();
  variant_json_init_set((*p)->json, o->json);
}

void json_set(mjson_t *p, mjson_t o)
{
  assert (p != NULL && *p != NULL && o != NULL);
  variant_json_set((*p)->json, o->json);
}

void json_out_str(FILE *f, mjson_t o)
{
  assert (f != NULL && o != NULL);
  if (variant_json_boolean_p(o->json)) {
    boolean_out_str(f, *variant_json_get_boolean(o->json));
  } else if (variant_json_real_p(o->json)) {
    real_out_str(f, *variant_json_get_real(o->json));
  } else if (variant_json_string_p(o->json)) {
    string_out_str(f, *variant_json_get_string(o->json));
  } else if (variant_json_array_p(o->json)) {
    array_json_out_str(f, *variant_json_get_array(o->json));
  } else if (variant_json_dict_p(o->json)) {
    dict_json_out_str(f, *variant_json_get_dict(o->json));
  } else {
    fprintf(f, "null");
  }
}

static int skip_space(FILE *f)
{
  int c;
  do {
    c = fgetc(f);
    if (ferror(f) || feof(f)) return 0;
  } while (isspace(c));
  ungetc(c, f);
  return c;
}

bool json_in_str(mjson_t *p, FILE *f)
{
  assert (p != NULL);
  bool b;
  char c = skip_space(f);
  if (ferror(f) || feof(f)) return false;
  variant_json_clear((*p)->json);
  if (c == 't' || c == 'f') {
    variant_json_init_boolean((*p)->json);
    b = boolean_in_str(variant_json_get_boolean((*p)->json), f);
    if (!b) return false;
  } else if ((c >= '0' && c <= '9') || (c == '-')){
    variant_json_init_real((*p)->json);
    b = real_in_str(variant_json_get_real((*p)->json), f);
    if (!b) return false;
  } else if (c == '\"') {
    variant_json_init_string((*p)->json);
    b = string_in_str(*variant_json_get_string((*p)->json), f);
    if (!b) return false;
  } else if (c == '[') {
    variant_json_init_array((*p)->json);
    b = array_json_in_str(*variant_json_get_array((*p)->json), f);
    if (!b) return false;
  } else if (c == '{') {
    variant_json_init_dict((*p)->json);
    b = dict_json_in_str(*variant_json_get_dict((*p)->json), f);
    if (!b) return false;
  } else if (c == 'n') {
    variant_json_init((*p)->json);
    c = fgetc(f);
    c = fgetc(f); if (c != 'u') return false;
    c = fgetc(f); if (c != 'l') return false;
    c = fgetc(f);
    return (c == 'l');
  } else {
    return false;
  }
  return true;
}

bool json_init_in_str(mjson_t *p, FILE *f)
{
  json_init(p);
  return json_in_str(p, f);
}

static mjson_t json_get_field(mjson_t p, va_list list)
{
  mjson_t r = p;
  while (true) {
    const char *str = va_arg(list, const char*);
    if (str == NULL)
      return r;
    if (str[0] == '[') {
      if (!variant_json_array_p(r->json)) return NULL;
      int n = atoi(str+1);
      if (n < 0 || (unsigned)n >= array_json_size(*variant_json_get_array(r->json)))
        return NULL;
      r = *array_json_get(*variant_json_get_array(r->json), n);
    } else {
      if (!variant_json_dict_p(r->json)) return NULL;
      string_t s;
      string_init_set_str (s, str); // BAD API: I want a macro to have a string_t from a const char* but STRING_CTE doesn't work :(
      mjson_t *v = dict_json_get (*variant_json_get_dict(r->json), s);
      string_clear(s);
      if (v == NULL) return NULL;
      r = *v;
    }
  }
}

double json_get_real(mjson_t p, ...)
{
  va_list ap;
  va_start(ap, p);
  mjson_t r = json_get_field(p, ap);
  va_end(ap);
  if (r == NULL || !variant_json_real_p(r->json))
    return NAN;
  else
    return *variant_json_get_real(r->json);
}

int json_get_boolean(mjson_t p, ...)
{
  va_list ap;
  va_start(ap, p);
  mjson_t r = json_get_field(p, ap);
  va_end(ap);
  if (r == NULL || !variant_json_boolean_p(r->json))
    return -1;
  else
    return *variant_json_get_boolean(r->json);
}

bool json_get_empty(mjson_t p, ...)
{
  va_list ap;
  va_start(ap, p);
  mjson_t r = json_get_field(p, ap);
  va_end(ap);
  if (r == NULL || !variant_json_empty_p(r->json))
    return false;
  else
    return true;
}

const char *json_get_string(mjson_t p, ...)
{
  va_list ap;
  va_start(ap, p);
  mjson_t r = json_get_field(p, ap);
  va_end(ap);
  if (r == NULL || !variant_json_string_p(r->json))
    return NULL;
  else
    return string_get_cstr(*variant_json_get_string(r->json));
}

