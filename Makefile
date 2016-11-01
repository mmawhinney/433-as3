
CC=arm-linux-gnueabihf-gcc
CFLAGS=-std=c99 -Wall -Werror -g -D _POSIX_C_SOURCE=200809L
LDFLAGS=-L$(HOME)/cmpt433/public/asound_lib_BBB -lm -lpthread -lasound

src_files := beatbox.c audioMixer.c locks.c control.c udp.c beatController.c accelController.c
src_objs := ${src_files:.c=.o}

PUBDIR = ~/cmpt433/public/myApps

all: beatbox wav node

beatbox: $(src_objs)
	$(CC) $(src_objs) -o beatbox $(LDFLAGS)
	cp beatbox $(PUBDIR)/beatbox 

wav:
	mkdir -p $(PUBDIR)/beatbox-wav-files/
	cp -R wave-files/* $(PUBDIR)/beatbox-wav-files/

node:
	mkdir -p $(PUBDIR)/beatbox-server-copy/
	cp -R server/* $(PUBDIR)/beatbox-server-copy

clean:
	rm *.o beatbox
