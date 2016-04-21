# Makefile for Linux etc.

.PHONY: all clean
	all: bladegps

CC=gcc
CFLAGS=-O3 -Wall -I/home/paul/build/radio/bladeRF/host/libraries/libbladeRF/include
LDFLAGS= -L/home/paul/build/radio/bladeRF/host/build/output -lbladeRF

#build: quicktune.o
#		${CC} $^ ${LDFLAGS} -o $@

quicktune: clean mkstop
	${CC} quicktune.c ${CFLAGS} ${LDFLAGS} -o quicktune

mkstop:
	${CC} stop.c ${CFLAGS} ${LDFLAGS} -o stop

clean:
		rm -f quicktune stop
