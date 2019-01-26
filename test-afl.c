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
  json_clear(p);
  exit(0);
}
