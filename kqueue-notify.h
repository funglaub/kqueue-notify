#ifndef _KQUEUE_NOTIFY_H_
#define _KQUEUE_NOTIFY_H_

#define TARGET_EVTS NOTE_RENAME|NOTE_WRITE|NOTE_DELETE

/* Structure holding all directory entries by name and their count */
typedef struct {
  int count;                            /* Number of directory entries */
  char *path;                           /* directory name */
  char **entries;                       /* Array of entries in that directory */
} dir_info;

#endif
