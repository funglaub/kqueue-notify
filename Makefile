CFLAGS  = -g -Wall -Wextra -pedantic -std=c99
PREFIX  = /usr/local
SRC     = kqueue-notify.c util.c
OBJ     = ${SRC:.c=.o}

.PHONY: clean

kqueue-notify: ${OBJ}
	${CC} -o $@ ${OBJ}

clean:
	 @rm -f *.o kqueue-notify

kqueue-notify.o: kqueue-notify.c kqueue-notify.h
util.o: util.c util.h
