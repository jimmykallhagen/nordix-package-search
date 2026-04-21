# Nordix Tools - Makefile
CC      = clang
CFLAGS  = -march=native -mtune=native -O3 -flto=full -Wall
PREFIX  = /usr/bin

TARGETS = nordix-pacman nordix-paru

all: $(TARGETS)

nordix-pacman: nordix-pacman.c
	$(CC) $(CFLAGS) -o $@ $<

nordix-paru: nordix-paru.c
	$(CC) $(CFLAGS) -o $@ $<


install: all
	install -Dm755 $(TARGETS) -t $(PREFIX)

clean:
	rm -f $(TARGETS)

.PHONY: all install clean