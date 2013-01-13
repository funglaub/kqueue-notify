#include <stdio.h>

#include "util.h"

int main(int argc, char *argv[])
{
  char *string, *string2;

  string = "Hello";
  string2 = ustrdup(string);

  printf("%s, %s\n", string, string2);


  return 0;
}
