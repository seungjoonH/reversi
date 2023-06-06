CC=gcc
LDFLAGS=-lncurses

all: reversi

reversi: obj/ui.o obj/system.o obj/network.o obj/reversi.o
	$(CC) -o reversi obj/*.o $(LDFLAGS)

obj/ui.o: lib/ui.c lib/ui.h
	$(CC) -c lib/ui.c -o obj/ui.o

obj/system.o: lib/system.c lib/system.h
	$(CC) -c lib/system.c -o obj/system.o

obj/network.o: lib/network.c lib/network.h
	$(CC) -c lib/network.c -o obj/network.o

obj/reversi.o: reversi.c
	$(CC) -c reversi.c -o obj/reversi.o

clean:
	rm -rf reversi obj/
	mkdir obj/
