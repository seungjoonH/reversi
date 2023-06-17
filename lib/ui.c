#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <ncurses.h>

#include "ui.h"
#include "network.h"
#include "logic.h"

int gridLty = 10;
int gridLtx = 5;
int gridR = 8;
int gridC = 8;
int gridSize = 4;
int pY = 4;
int pX = 4;

int getCh() { return getch(); }

void logo(int lty, int ltx) {
	mvprintw(lty + 0, ltx, "######  ####### ##    ## ####### ######  ####### ## ");	
	mvprintw(lty + 1, ltx, "##   ## ##      ##    ## ##      ##   ## ##      ## ");	
	mvprintw(lty + 2, ltx, "######  #####   ##    ## #####   ######  ####### ## ");	
	mvprintw(lty + 3, ltx, "##   ## ##       ##  ##  ##      ##   ##      ## ## ");	
	mvprintw(lty + 4, ltx, "##   ## #######   ####   ####### ##   ## ####### ## ");	
}

void win(int lty, int ltx) {
	mvprintw(lty + 0, ltx, "##    ##  ######  ##    ##     ##     ## ## ###    ## ## ");	
	mvprintw(lty + 1, ltx, " ##  ##  ##    ## ##    ##     ##     ## ## ####   ## ## ");
	mvprintw(lty + 2, ltx, "  ####   ##    ## ##    ##     ##  #  ## ## ## ##  ## ## ");
	mvprintw(lty + 3, ltx, "   ##    ##    ## ##    ##     ## ### ## ## ##  ## ##    ");
	mvprintw(lty + 4, ltx, "   ##     ######   ######       ### ###  ## ##   #### ## ");

}

void draw(int lty, int ltx) {
	mvprintw(lty + 0, ltx, "######  ######   #####  ##     ## ");
	mvprintw(lty + 1, ltx, "##   ## ##   ## ##   ## ##     ## ");
	mvprintw(lty + 2, ltx, "##   ## ######  ####### ##  #  ## ");
	mvprintw(lty + 3, ltx, "##   ## ##   ## ##   ## ## ### ## ");
	mvprintw(lty + 4, ltx, "######  ##   ## ##   ##  ### ###  ");
}

void lose(int lty, int ltx) {
	mvprintw(lty + 0, ltx, "##    ##  ######  ##    ##     ##       ######  ####### ####### ");
	mvprintw(lty + 1, ltx, " ##  ##  ##    ## ##    ##     ##      ##    ## ##      ##      ");
	mvprintw(lty + 2, ltx, "  ####   ##    ## ##    ##     ##      ##    ## ####### #####   ");
	mvprintw(lty + 3, ltx, "   ##    ##    ## ##    ##     ##      ##    ##      ## ##      ");
	mvprintw(lty + 4, ltx, "   ##     ######   ######      #######  ######  ####### ####### ");
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

void fill(int lty, int ltx, int size, int value) {
	int color = value + 3;
	int abs = value < 0 ? -value : value;

	attron(COLOR_PAIR(color));

	if (abs < 2) {
		for (int i = 0; i < size - 1; i++)
			for (int j = 0; j < size * 2 - 1; j++)
				mvaddch(lty + i, ltx + j, ' ');
	}
	else rectangle(lty, ltx, size * 2 - 2, size - 2);

	attroff(COLOR_PAIR(color));
}

void drawGrid() {
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
			fill(y + 1, x + 1, s, 0);
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

	if (role == turn) {
		attron(COLOR_PAIR(6));
		rectangle(lty + s * pY, ltx + s * 2 * pX, s * 2, s);
		attroff(COLOR_PAIR(6));
	}

	for (int i = 0; i < r; i++) {
		int y = lty + i * s + 1;
		for (int j = 0; j < c; j++) {
			int x = ltx + j * s * 2 + 1;
			fill(y, x, s, data[i][j]);
		}
	}
}

void initBoard() {
	before = (int **) malloc(gridR * sizeof(int *));
	data = (int **) malloc(gridR * sizeof(int *));
	for (int i = 0; i < gridR; i++) {
		before[i] = (int *) malloc(gridC * sizeof(int));
		data[i] = (int *) malloc(gridC * sizeof(int));
	}

	for (int i = 0; i < gridR; i++)
		for (int j = 0; j < gridC; j++)
			data[i][j] = 0;
	data[gridR / 2 - 0][gridC / 2 - 0] = 1;
	data[gridR / 2 - 1][gridC / 2 - 0] = -1;
	data[gridR / 2 - 0][gridC / 2 - 1] = -1;
	data[gridR / 2 - 1][gridC / 2 - 1] = 1;
}

int execute() {
	int c, term = 0;
	flushinp();
	
	if (gameover()) return -1;
	if (noAvailable()) return 0;

	while ((c = getch())) {
		term = 0;
		switch (c) {
			case KEY_LEFT: left(); break;
			case KEY_RIGHT: right(); break;
			case KEY_UP: up(); break;
			case KEY_DOWN: down(); break;
			case ' ': if (space()) break;
			case 'q': term = 1; break;
		}
		redraw();
		if (term) return c;
	}
	return 0;
}

void left() { if (pX > 0) pX--; }
void right() { if (pX < gridC - 1) pX++; }
void up() { if (pY > 0) pY--; }
void down() { if (pY < gridR - 1) pY++; }
int space() { return put(role); }

int put(int val) {
	if (data[pY][pX] != role * 2) return 1;
	data[pY][pX] = val;
	flip(pY, pX);
	redraw();
	backupData();
	return 0;
}

void setColor() {
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_GREEN);
	init_pair(3, COLOR_BLACK, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_WHITE);
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_YELLOW);
}

void redraw() {
	drawGrid();
	refresh();
}

void initUI() {
	initscr();
	setColor();

	clear();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();

	curs_set(0);

	logo(2, 11);
	initBoard();

	redraw();
}

void disposeUI() {
	attroff(1);
	endwin();
	free(data);
}

void isFinished() {
	initscr();
	clear();

	gameover();
	if (!winner) draw(10, 11);
	else if (role == winner) win(10, 11);
	else lose(10, 11);

	refresh();
}
