/* Author: Florian Unglaub <f.unglaub@gmail.com>
 *
 * Watches a directory for changes and prints the event type to STDOUT.
 *
 * Inspired by http://github.com/sschober/kqwait
 *
 * For documentation refer to:
 * http://people.freebsd.org/~jlemon/papers/kqueue.pdf
 * kqueue(2)
 *
 */

/* Kernel stuff: */
#include <sys/event.h>                  /* kqueue */
#include <sys/stat.h>                   /* S_ISDIR */
#include <sys/dir.h>

#include <fcntl.h>                      /* open(), O_RDONLY */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>                     /* exit(), malloc(), etc. */
#include <unistd.h>                     /* close() */
#include <err.h>
#include <sysexits.h>

#include "kqueue-notify.h"
#include "util.h"

void
print_dip(dir_info *dip) {
  for (int i=0; i < dip->count; i++)
    printf("[%d]: %s\n", i, dip->entries[i]);
}

dir_info *
init_dip(char *path) {
  dir_info *dip;

  dip = (dir_info *)umalloc(sizeof(dir_info));
  dip->path = path;
  dip->count = 0;
  dip->entries = NULL;
  return(dip);
}

void
free_dip(dir_info *dip) {
  if (dip == NULL)
    return;

  for (int i = 0; i < dip->count; i++)
    free(dip->entries[i]);

  free(dip->entries);
  free(dip);
}

dir_info *
add_entry(dir_info *dip, char *name) {
  dip->count++;
  dip->entries = urealloc(dip->entries, dip->count * sizeof(char *));
  dip->entries[dip->count - 1] = ustrdup(name);

  return(dip);
}

dir_info *
parse_dir(char *path) {
  DIR *dirp;
  struct stat sb;
  struct dirent *dp;
  dir_info *dip;

  if (stat(path, &sb) == -1 ) err(EX_NOINPUT, NULL);
  if (S_ISDIR(sb.st_mode) == 0) { printf("error\n"); return NULL; }

  dirp = opendir(path);
  if (dirp == NULL) err(EX_NOINPUT, NULL);

  dip = init_dip(path);

  while ((dp = readdir(dirp)) != NULL) {
    if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0 )
      continue; /* Skip . and .. */
    dip = add_entry(dip, dp->d_name);
  }

  closedir(dirp);
  return(dip);
}

int
dir_info_find(dir_info *dip, char *name) {
  for (int i = 0; i < dip->count; i++) {
    if (strcmp(dip->entries[i], name) == 0)
      return 1;                         /* found it */
  }

  return 0;
}

dir_info *
dir_diff(dir_info *a, dir_info *b) {
  dir_info *res, *iter, *other;

  if (a == NULL) return(b);
  if (b == NULL) return(a);

  res = init_dip(a->path);

  iter = a;
  other = b;

  if (a->count < b->count) {
    iter = b;
    other = a;
  }

  for (int i = 0; i < iter->count; i++) {
    if (dir_info_find(other, iter->entries[i]) == 0)
      res = add_entry(res, iter->entries[i]);
  }

  return(res);
}

void
printf_diff(dir_info *diff, dir_info *old, dir_info *new) {
  char *indicator;

  indicator = NULL;

  if (old->count > new->count)          /* something was removed */
    indicator = "-";
  else if (new->count > old->count)     /* something was added */
    indicator = "+";

  if (indicator != NULL) {
    for (int i=0; i < diff->count; i++)
      printf("[%s] %s\n", indicator, diff->entries[i]);
  }
}

int
main(int argc, char *argv[])
{
  char *path;
  struct kevent ev;
  int fd, kq, result;
  dir_info *dip, *dip_changed, *dip_diff;

  path = argv[1];
  fd = open(path, O_RDONLY);
  if (fd == -1) err(EX_NOINPUT, NULL);

  EV_SET(&ev, fd, EVFILT_VNODE, EV_ADD | EV_ENABLE | EV_CLEAR, TARGET_EVTS, 0,
         NULL);

  kq = kqueue();
  dip = parse_dir(path);

  for (;;) {
    result = kevent(kq, &ev, 1, &ev, 1, NULL);

    if (result > 0) {                   /* Something has changed */
      dip_changed = parse_dir(path);
      dip_diff = dir_diff(dip, dip_changed);
      printf_diff(dip_diff, dip, dip_changed);
      dip = parse_dir(path);
    }
  }
  return(EXIT_SUCCESS);
}
