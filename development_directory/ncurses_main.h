/*
 * Code taken from Daniel Corbe "Copyright (c) 2013 Daniel Corbe" is used to implement scrolling in window
 */
#include <panel.h>
#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <curses.h>
#include <unistd.h>
#include "ui.h"
#pragma once

void ncurses_main(UI **ui, int size);
void init_UIs(UI **ui, int size);
void help_win(WINDOW *win);
void init_wins_panls(WINDOW **wins, PANEL  **my_panels, int n);
void init_colors();
void win_show(WINDOW *win, char *title, char* creator, char* username);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *title, char* creator, char* username);
void init_curses();
int centered(char* str, int startx, int width);
void clear_win(UI* ui);
void make_chatroom(UI *ui, int row, int col,char* title, char* maker, char*  username);
void show_chatroom(UI* ui);

void ncurses_main(UI **ui, int size)
{
    init_curses();
    init_UIs(ui, size);
    init_colors();
}
void init_UIs(UI **ui, int size)
{
    int i = 0;
    for(;i <size;i++)
    {
        ui[i] = ui_init();
    }
}

void help_win(WINDOW *win)
{	
    char* title = "HELP";
    char* button = "P R E S S   A N Y   B U T T O N   T O   C O N T I N U E";
    char* encode = "#e: encode -- obfuscate a message based on a Caesar cipher prior to being sent";
    char* decode = "#d: decode -- de-obfuscate a message based on a caesar cipher at any given time";
    char* block = "#b: block -- blocks a user from a chatroom; cannot be used in lobby";
    char* lobby = "#l: lobby -- takes user back to lobby";
    char* make_c = "#mc: make chatroom -- makes a chatroom";
    char* delete_c = "#dc: delete chatroom: deletes a chatroom";
    char* logout = "#lo: logout -- logs a user out";
    char* file = "#f: file -- sends a file to all users in a chatroom";
    char* show_c = "#sc: show chatroom -- shows all available chatrooms in lobby";
    char* change_c = "#c: change chatroom -- changes to inputed chatroom port";
    
    int startx, starty, height, width;

	getbegyx(win, starty, startx);
	getmaxyx(win, height, width);
	
    box(win, 0, 0);
	mvwaddch(win, 2, 0, ACS_LTEE); 
	mvwhline(win, 2, 1, ACS_HLINE, width); 
	mvwaddch(win, 2, width - 1, ACS_RTEE);
	
	getmaxyx(win, height, width);
	
	int length, x, y;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 85;
		
	wattron(win, COLOR_PAIR(2));
	mvwprintw(win, y-1, 41, "%s", title); //x = 41 (width of screen - size of title) / 2
	mvwprintw(win, y+1, 14, "%s", button);
	
	mvwprintw(win, y + 3, 1,"%s", encode);
	mvwprintw(win, y + 5, 1,"%s", decode);
	mvwprintw(win, y + 7, 1,"%s", block);
	mvwprintw(win, y + 9, 1,"%s", lobby);
	mvwprintw(win, y + 11, 1,"%s", make_c);
	mvwprintw(win, y + 13, 1,"%s", delete_c);
	mvwprintw(win, y + 15, 1,"%s", logout);
	mvwprintw(win, y + 17, 1,"%s", file);
	mvwprintw(win, y + 19, 1,"%s", show_c);
	mvwprintw(win, y + 21, 1,"%s", change_c);
	
	
	wattroff(win, COLOR_PAIR(2));
	refresh();
}

void init_wins_panls(WINDOW **wins, PANEL  **my_panels, int n)
{	
    int x, y, i, nlines, ncols;

	y = 2;
	x = 10;
	nlines = 40;
	ncols = 85;
	for(i = 0; i < n; i++)
	{		    	    
	    if( i == n-1)
	    {
	        wins[n-1] = newwin(25, ncols, y, x);
	        my_panels[n-1] = new_panel(wins[n-1]);
	        help_win(wins[i]);
	        continue;
	    }
	    
        wins[i] = newwin(nlines, ncols, y, x);
	    my_panels[i] = new_panel(wins[i]);
	}
	
}

/* Show the window with a border and a label */
void win_show(WINDOW *win, char *title, char* creator, char* username)
{	
    int startx, starty, height, width;

	getbegyx(win, starty, startx);
	getmaxyx(win, height, width);

	box(win, 0, 0);
	mvwaddch(win, 5, 0, ACS_LTEE); 
	mvwhline(win, 5, 1, ACS_HLINE, width - 2); 
	mvwaddch(win, 5, width - 1, ACS_RTEE);
	
	mvwaddch(win, height - 3, 0, ACS_LTEE); 
	mvwhline(win, height - 3, 1, ACS_HLINE, width - 2); 
	mvwaddch(win, height-3, width - 1, ACS_RTEE);
	
	
	print_in_middle(win, 1, 0, width, title, creator, username);	
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *title, char* creator, char* username)
{	
    int length, x, y;
    int height;
	float temp;
	char creator2[strlen(creator) + 5];
	char nick[strlen(username) + 10];
	
	sprintf(creator2, "By: %s", creator);
	sprintf(nick, "Username: %s", username);
	
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
		
    wattron(win, COLOR_PAIR(1));
	x = centered(title, startx, width);
	mvwprintw(win, y, x, "%s", title);

	x = centered(creator2, startx, width);
	mvwprintw(win, y+1, x,"%s", creator2);
	
	x = centered(nick, startx, width);
	mvwprintw(win, y+2, x,"%s", nick);
	
	x = centered("#help", startx, width);
	mvwprintw(win, y+3, x,"%s", "#help");
	
	
	wattroff(win, COLOR_PAIR(1));
	refresh();
}

int centered(char* str, int startx, int width)
{
    int x, length;
    float temp;
    
    length = strlen(str);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	
	return x;
}


void init_curses()
{
    initscr();
    nonl();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	intrflush(stdscr, FALSE);
}

void init_colors()
{
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
	
	init_pair(8, COLOR_WHITE,   COLOR_BLUE);
}

void clear_win(UI* ui)
{
    wclear(ui->input);
    wclear(ui->title);
    wclear(ui->text);
}
void make_chatroom(UI *ui, int row, int col,char* title,
    char* maker, char*  username)
{
    /* Paint the text scrollable */
	ui->text = newwin(row - 9, col - 2, 6, 1);
	scrollok(ui->text, TRUE);

	
	wmove(ui->text, row - 4, 0);

	/* Paint the input window */
	ui->input = newwin(1, col - 2, row - 2, 1);
	keypad(ui->input, TRUE);
	wmove(ui->input, 0, 0);

    /* Paint the title window*/
    ui->title = newwin(row, col, 0, 0);
    win_show(ui->title, title, maker, username);

	/* Refresh all windows *
	refresh();
	wrefresh(ui->title);
	wrefresh(ui->text);
	wrefresh(ui->input);*/
}
void show_chatroom(UI* ui)
{
    /* Refresh all windows */
	refresh();
	wrefresh(ui->title);
	wrefresh(ui->text);
	wrefresh(ui->input);
}
