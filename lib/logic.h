#ifndef _LOGIC_H
#define _LOGIC_H

int adjI(Direction dir);
int adjJ(Direction dir);
void initAvailable();
void setAvailable();
void eightDirSearch(int i, int j, int value);
void dirSearch(int i, int j, Direction dir, int value);
void flip(int i, int j);

#endif
