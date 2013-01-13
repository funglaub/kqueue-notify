#ifndef UTIL_H
#define UTIL_H

void *umalloc(size_t l);
void *urealloc(void *p, size_t l);
char *ustrdup(const char *s);
#endif
