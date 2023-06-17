#ifndef _UI_H
#define _UI_H

#include "enums.h"

int **before;
int **data;

extern int gridLty;
extern int gridLtx;
extern int gridR;
extern int gridC;
extern int gridSize;
extern int pY;
extern int pX;

int getCh();

void logo(int lty, int ltx);
void win(int lty, int ltx);
void draw(int lty, int ltx);
void lose(int lty, int ltx);
void rectangle(int lty, int ltx, int w, int h);
void fill(int lty, int ltx, int size, int player);
void drawGrid();
void initBoard();

int execute();
void left();
void right();
void up();
void down();
int space();

int put(int val);
void setColor();
void redraw();
void initUI();
void disposeUI();
void isFinished();

#endif
