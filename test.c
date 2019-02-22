#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mjsonlib.h"

static void test_file(const char filename[], bool printit)
{
  FILE *f = fopen(filename, "r");
  if (f== NULL) {
    fprintf(stderr, "ERROR: Cannot open %s\n", filename);
    exit(1);
  }
  
  mjson_t p;
  bool b = json_init_in_str(&p, f);
  if (!b) {
    fprintf(stderr, "ERROR: Cannot parse JSON file in %s\n", filename);
    exit(2);
  }

  if (printit) {
    printf("JSON= ");
    json_out_str(stdout, p);
    printf("\n");
  }

  json_clear(p);
  fclose(f);
}

static void test_suite(void)
{
  FILE *f = fopen("test.json", "r");
  assert (f != NULL);

  mjson_t p;
  bool b = json_init_in_str(&p, f);
  assert(b);
  fclose(f);
  printf("JSON= ");
  json_out_str(stdout, p);
  printf("\n");
  double d = json_get_real(p, "c", "[2]", "d", NULL);
  assert (d == 42.0);
  d = json_get_real(p, "b", NULL);
  assert (d == 1.5);
  int i = json_get_boolean (p, "c", "[0]", NULL);
  assert (i == false);
  const char *str = json_get_string(p, "c", "[3]", NULL);
  assert (strcmp(str, "Hello") == 0);
  json_clear(p);
}

int main(int argc, const char *argv[])
{
  if (argc == 1)
    test_suite();
  else
    test_file(argv[1], argc > 2);
  exit(0);
}
