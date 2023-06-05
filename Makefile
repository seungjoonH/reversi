CC=gcc
LDFLAGS=-lncurses

all: reversi

reversi: ui.o reversi.o
	$(CC) -o reversi obj/ui.o obj/reversi.o $(LDFLAGS)

ui.o: lib/ui.c lib/ui.h
	$(CC) -c lib/ui.c -o obj/ui.o

reversi.o: reversi.c
	$(CC) -c reversi.c -o obj/reversi.o

clean:
	rm -f reversi obj/ui.o obj/reversi.o

