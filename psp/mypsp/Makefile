# MyPSP library 
# Akop Karapetyan, 2007

CC=psp-gcc
AR=psp-ar
RANLIB=psp-ranlib
RM=rm -rf
PSPSDK=$(shell psp-config --pspsdk-path)

CFLAGS=-O2 -G0 -Wall -I$(PSPSDK)/include

all: libmypsp.a

libmypsp.a: menu.o font.o init.o perf.o file.o psp.o audio.o ctrl.o \
            video.o image.o kybd.o ui.o util.o
	$(AR) cru $@ $?
	$(RANLIB) $@

clean:
	$(RM) *.o genfont stockfont.h

clean_all: clean
	$(RM) *.a

menu.o: menu.c menu.h
	$(CC) $(CFLAGS) -c -o $@ $<

font.o: font.c font.h stockfont.h
	$(CC) $(CFLAGS) -c -o $@ $<

init.o: init.c init.h
	$(CC) $(CFLAGS) -c -o $@ $<

image.o: image.c image.h video.o
	$(CC) $(CFLAGS) -c -o $@ $<

kybd.o: kybd.c kybd.h video.o font.o ctrl.o
	$(CC) $(CFLAGS) -c -o $@ $<

video.o: video.c video.h
	$(CC) $(CFLAGS) -c -o $@ $<

audio.o: audio.c audio.h
	$(CC) $(CFLAGS) -c -o $@ $<

ctrl.o: ctrl.c ctrl.h
	$(CC) $(CFLAGS) -c -o $@ $<

psp.o: psp.c psp.h
	$(CC) $(CFLAGS) -c -o $@ $<

perf.o: perf.c perf.h
	$(CC) $(CFLAGS) -c -o $@ $<

file.o: file.c file.h
	$(CC) $(CFLAGS) -c -o $@ $<

ui.o: ui.c ui.h psp.o file.o ctrl.o font.o menu.o video.o
	$(CC) $(CFLAGS) -c -o $@ $<

util.o: util.c util.h image.o file.o video.o
	$(CC) $(CFLAGS) -c -o $@ $<

stockfont.h: stockfont.fd genfont
	./genfont < $< > $@

genfont: genfont.c
	cc $< -o $@
