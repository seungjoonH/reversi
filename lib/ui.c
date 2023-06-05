#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>

const int gridLty = 10;
const int gridLtx = 5;
const int gridR = 8;
const int gridC = 8;
const int gridSize = 4;

void logo(int lty, int ltx) {
	mvprintw(lty + 0, ltx, "  _____  ________      ________ _____   _____ _____ ");
	mvprintw(lty + 1, ltx, " |  __ \\|  ____\\ \\    / /  ____|  __ \\ / ____|_   _|");
	mvprintw(lty + 2, ltx, " | |__) | |__   \\ \\  / /| |__  | |__) | (___   | |  ");
	mvprintw(lty + 3, ltx, " |  _  /|  __|   \\ \\/ / |  __| |  _  / \\___ \\  | |  ");
	mvprintw(lty + 4, ltx, " | | \\ \\| |____   \\  /  | |____| | \\ \\ ____) |_| |_ ");
	mvprintw(lty + 5, ltx, " |_|  \\_\\______|   \\/   |______|_|  \\_\\_____/|_____|");
}

void rectangle(int lty, int ltx, int w, int h) {
	int rby = lty + h;
	int rbx = ltx + w;

	mvhline(lty, ltx, 0, w);
	mvhline(rby, ltx, 0, w);
	mvvline(lty, ltx, 0, h);
	mvvline(lty, rbx, 0, h);
	mvaddch(lty, ltx, ACS_ULCORNER);
	mvaddch(rby, ltx, ACS_LLCORNER);
	mvaddch(lty, rbx, ACS_URCORNER);
	mvaddch(rby, rbx, ACS_LRCORNER);
}

void fill(int lty, int ltx, int size, int player) {
	int color = (player > 0) + 2;
	attron(COLOR_PAIR(color));

	for (int i = 0; i < size - 1; i++)
		for (int j = 0; j < size * 2 - 1; j++)
			mvaddch(lty + i, ltx + j, ' ');

	attroff(COLOR_PAIR(color));
}

void drawGrid(int pY, int pX) {
	int lty = gridLty;
	int ltx = gridLtx;
	int s = gridSize;
	int r = gridR;
	int c = gridC;

	for (int i = 0; i < r; i++) {
		int y = lty + i * s;
		for (int j = 0; j < c; j++) {
			int x = ltx + j * s * 2;
			rectangle(y, x, s * 2, s);
			if (i && j) mvaddch(y, x, ACS_PLUS);
		}
	}

	for (int i = 1; i < r; i++) {
		int y = lty + i * s;
		mvaddch(y, ltx, ACS_LTEE);
		mvaddch(y, ltx + c * s * 2, ACS_RTEE);
	}

	for (int j = 1; j < c; j++) {
		int x = ltx + j * s * 2;
		mvaddch(lty, x, ACS_TTEE);
		mvaddch(lty + r * s, x, ACS_BTEE);
	}

	attron(COLOR_PAIR(1));
	rectangle(lty + s * pY, ltx + s * 2 * pX, s * 2, s);
	attroff(COLOR_PAIR(1));
}

void initBoard(int **data) {
	for (int i = 0; i < gridR; i++)
		for (int j = 0; j < gridC; j++)
			data[i][j] = 0;
	data[gridR / 2 - 0][gridC / 2 - 0] = 1;
	data[gridR / 2 - 1][gridC / 2 - 0] = -1;
	data[gridR / 2 - 0][gridC / 2 - 1] = -1;
	data[gridR / 2 - 1][gridC / 2 - 1] = 1;
}

void drawBoard(int **data) {
	int lty = gridLty + 1;
	int ltx = gridLtx + 1;
	int s = gridSize;
	int r = gridR;
	int c = gridC;

	for (int i = 0; i < r; i++) {
		int y = lty + i * s;
		for (int j = 0; j < c; j++) {
			int x = ltx + j * s * 2;
			if (data[i][j]) fill(y, x, s, data[i][j]);
		}
	}
}

void put(int **data, int pY, int pX, int val) {
	data[pY][pX] = val;
	drawBoard(data);
}

void showUI() {
	initscr();

	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLUE);
	init_pair(3, COLOR_RED, COLOR_RED);

	int **board;

	board = (int **) malloc(gridR * sizeof(int *));
	for (int i = 0; i < gridR; i++) 
		board[i] = (int *) malloc(gridC * sizeof(int));

	clear();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();

	curs_set(0);

	int c;
	int pY = 4, pX = 4;

	logo(2, 11);
	initBoard(board);
	drawGrid(pX, pY);
	drawBoard(board);
	refresh();

	while ((c = getch())) {
		switch (c) {
			case KEY_LEFT: if (pX > 0) pX--; break;
			case KEY_RIGHT: if (pX < gridC - 1) pX++; break;
			case KEY_UP: if (pY > 0) pY--; break;
			case KEY_DOWN: if (pY < gridR - 1) pY++; break;
			case ' ': put(board, pY, pX, 1); break;
			default: break;
		}

		drawGrid(pY, pX);
		refresh();
	}

	endwin();

	for (int i = 0; i < gridR; i++) free(board[i]);
	free(board);
}
