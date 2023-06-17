#include <stdio.h>

#include "enums.h"
#include "ui.h"
#include "network.h"
#include "logic.h"

int dirI(Direction dir) {
	switch (dir) {
		case NW:
		case N:
		case NE: return -1;
		case SW:
		case S:
		case SE: return 1;
		default: return 0;
	}
	return 0;
}

int dirJ(Direction dir) {
	switch (dir) {
		case NW:
		case W:
		case SW: return -1;
		case NE:
		case E:
		case SE: return 1;
		default: return 0;
	}
	return 0;
}

void initAvailable() {
	int r = gridR;
	int c = gridC;

	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			if (data[i][j] == 2 || data[i][j] == -2)
				data[i][j] = 0;
}

void setAvailable() {
	int r = gridR;
	int c = gridC;

	for (int i = 0; i < r; i++)
		for (int j = 0; j < c; j++)
			if (data[i][j] == role)
				eightDirSearch(i, j, 0);
}

void eightDirSearch(int i, int j, int value) {
	for (Direction dir = N; dir < DIR_LEN; dir++)
		dirSearch(i, j, dir, value);
}

void dirSearch(int i, int j, Direction dir, int value) {
	int dI = dirI(dir);
	int dJ = dirJ(dir);

	int cnt = 1;
	int aI, aJ;

	while (1) {
		aI = i + dI * cnt;
		aJ = j + dJ * cnt;
		
		if (aI < 0 || aI >= gridR) return;
		if (aJ < 0 || aJ >= gridC) return;
		if (data[aI][aJ] == 2 * role) return;
		if (data[aI][aJ] == value && cnt > 1) break;
		if (data[aI][aJ] != -role) return;
		
		cnt++;
	}

	if (value) {
		if (!data[aI][aJ]) return;
		for (int c = 1; c < cnt; c++)
			data[i + dI * c][j + dJ * c] = value;
	}
	else data[aI][aJ] = role * 2;
}

void flip(int i, int j) {
	eightDirSearch(i, j, role);
}
