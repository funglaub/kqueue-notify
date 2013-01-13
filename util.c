#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

void *
umalloc(size_t l)
{
  void *p;

  p = malloc(l);
  if (p == NULL) err(EX_OSERR, NULL);
  memset(p, 0, l);

  return p;
}

void *
urealloc(void *p, size_t l)
{
  void *r;

  r = realloc(p, l);
  if(r == NULL) err(EX_OSERR, NULL);
  return r;
}

char *
ustrdup(char *s) {
  size_t len;
  char *new;

  len = strlen(s) + 1;                  /* \0 */
  new = (char *)umalloc(sizeof(char) * len);
  strncpy(new, s, len);

  return new;
}
