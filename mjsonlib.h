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
#ifndef _HEADER_MJSONLIB_H
#define _HEADER_MJSONLIB_H

#include <stdio.h>
#include <stdbool.h>

typedef struct mjson_node_s *mjson_t;

extern void json_init (mjson_t *);
extern bool json_init_in_str(mjson_t *, FILE *);
extern void json_init_set (mjson_t *, mjson_t);
extern void json_clear (mjson_t);
extern void json_set (mjson_t *, mjson_t);
extern void json_out_str(FILE *, mjson_t);
extern bool json_in_str(mjson_t *, FILE *);
extern double json_get_real(mjson_t, ...);
extern int json_get_boolean(mjson_t, ...);
extern const char *json_get_string(mjson_t, ...);
extern bool json_get_empty(mjson_t, ...);

#endif
