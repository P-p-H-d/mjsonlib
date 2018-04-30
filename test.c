#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "mjsonlib.h"

int main(void)
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
  return 0;
}
