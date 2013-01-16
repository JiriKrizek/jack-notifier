CC = gcc

all: notify-init


notify-init: detect.o
	${CC} ${CFLAGS} -o $@ detect.o 

.c.o:
	${CC} ${CFLAGS} -c $< 

clean:
	rm *.o

install: notify-init
	ln -s ./notify-init /etc/init.d

.PHONY: install
