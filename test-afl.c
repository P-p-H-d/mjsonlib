#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mjsonlib.h"

int main(void)
{
  mjson_t p;
  bool b = json_init_in_str(&p, stdin);
  if (!b) exit(1);

  printf("JSON= ");
  json_out_str(stdout, p);
  printf("\n");

  double d = json_get_real(p, "c", "[2]", "d", NULL);
  d = json_get_real(p, "b", NULL);
  int i = json_get_boolean (p, "c", "[0]", NULL);
  const char *str = json_get_string(p, "c", "[3]", NULL);
  
  json_clear(p);
  exit(0);
}
