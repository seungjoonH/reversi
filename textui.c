#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <curses.h>

int main(int args, char** argv)
{
    inittscr();
    noecho();
    curs_set(0);

    int yMax,xMax;
    getmaxyx(stdscr,yMax,xMax);

    WINDOW *win = newwin(yMax/2,xMax/2, yMax/4,xMax/4);
    box(win,0,0);

    wgetch(win);

    endwin();
    return 0;
}
