#ifndef _LOGIC_H
#define _LOGIC_H

#include <stdbool.h>

int adjI(Direction dir);
int adjJ(Direction dir);
void initAvailable();
void setAvailable();
void eightDirSearch(int i, int j, int value);
void dirSearch(int i, int j, Direction dir, int value);
void flip(int i, int j);
bool gameover();
bool noAvailable();
bool hasNoStone();
bool isFull();
bool bothSkipped();
int sumStones();

#endif
