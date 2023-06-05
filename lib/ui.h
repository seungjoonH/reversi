#ifndef _UI_H
#define _UI_H

void logo(int lty, int ltx);
void rectangle(int lty, int ltx, int w, int h);
void fill(int lty, int ltx, int size, int player);
void drawGrid(int pY, int pX);
void initBoard(int **data);
void drawBoard(int **data);
void put(int **data, int pY, int pX, int val);
void showUI();

#endif
