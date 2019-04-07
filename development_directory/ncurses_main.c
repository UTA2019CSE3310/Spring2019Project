#include <panel.h>
#include <ncurses.h>
#include <string.h>
#pragma once

void init_colors();
void init_wins_panls(WINDOW **wins, PANEL  **my_panels, int n);
void win_show(WINDOW *win, char *label, int label_color);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
void init_curses();
void ncurses_main(int num_win);


void ncurses_main(int num_win);
{
    WINDOW *my_wins[num_wins];
    PANEL  *my_panels[num_wins];
    init_curses();
    init_colors();
    
    init_wins_panls(my_wins, my_panels, num_wins);
}
void init_wins_panls(WINDOW **wins, PANEL  **my_panels, int n)
{	
    int x, y, i, nlines, ncols;
	char label[80];

	y = 2;
	x = 10;
	nlines = 40;
	ncols = 60;
	for(i = 0; i < n; ++i)
	{	
	    wins[i] = newwin(nlines, ncols, y, x);
	    my_panels[i] = new_panel(my_wins[i]);
		//sprintf(label, "Window Number %d", i + 1);
		//win_show(wins[i], label, i + 1);
	}
}

/* Show the window with a border and a label */
void win_show(WINDOW *win, char *label, int label_color)
{	
    int startx, starty, height, width;

	getbegyx(win, starty, startx);
	getmaxyx(win, height, width);

	box(win, 0, 0);
	mvwaddch(win, 4, 0, ACS_LTEE); 
	mvwhline(win, 4, 1, ACS_HLINE, width - 2); 
	mvwaddch(win, 4, width - 1, ACS_RTEE);
	
	mvwaddch(win, height-3, 0, ACS_LTEE); 
	mvwhline(win, height-3, 1, ACS_HLINE, width - 2); 
	mvwaddch(win, height-3, width - 1, ACS_RTEE);
	
	print_in_middle(win, 1, 0, width, label, COLOR_PAIR(label_color));
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	
    int length, x, y;
    int height;
	float temp;
	
	getmaxyx(win, height, width);

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	mvwprintw(win, y+1, x+2,"%s", "By: Server");
	mvwprintw(win, y+2, x+4,"%s", "#help");
	
	mvwprintw(win, height -2, 1,"%s", "User_1: ");
	
	wattroff(win, color);
	refresh();
}

void init_curses()
{
    initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	intrflush(stdscr, FALSE);
}

void init_colors()
{
    init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);
}
