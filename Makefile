CC=gcc
LDFLAGS=-lncurses

all: reversi

reversi: ui.o system.o reversi.o
	$(CC) -o reversi obj/ui.o obj/system.o obj/reversi.o $(LDFLAGS)

system.o: lib/system.c lib/system.h
	$(CC) -c lib/system.c -o obj/system.o

ui.o: lib/ui.c lib/ui.h
	$(CC) -c lib/ui.c -o obj/ui.o

reversi.o: reversi.c
	$(CC) -c reversi.c -o obj/reversi.o

clean:
	rm -f reversi obj/

