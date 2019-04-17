#include <panel.h>
#include <ncurses.h>
#include <string.h>

#define NLINES 40
#define NCOLS 60

void init_wins(WINDOW **wins, int n);
void win_show(WINDOW *win, char *label, int label_color);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int main()
{	
    WINDOW *my_wins[5];
	PANEL  *my_panels[5];
	PANEL  *top;
	int ch;

	/* Initialize curses */
	initscr();
	start_color();
	cbreak();
	noecho();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);

	/* Initialize all the colors */
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);

	init_wins(my_wins, 5);
	
	/* Attach a panel to each window */ 	/* Order is bottom up */
	//my_panels[0] = new_panel(my_wins[0]); 	/* Push 0, order: stdscr-0 */
	//my_panels[1] = new_panel(my_wins[1]); 	/* Push 1, order: stdscr-0-1 */
	//my_panels[2] = new_panel(my_wins[2]); 	/* Push 2, order: stdscr-0-1-2 */
    //my_panels[3] = new_panel(my_wins[3]);
    //my_panels[4] = new_panel(my_wins[4]);
    
    //my_panels[4] = new_panel(my_wins[4]); 	
	//my_panels[3] = new_panel(my_wins[3]); 	
	//my_panels[2] = new_panel(my_wins[2]); 	
    my_panels[1] = new_panel(my_wins[1]);
    my_panels[0] = new_panel(my_wins[0]);
        
	/* Set up the user pointers to the next panel */
	set_panel_userptr(my_panels[0], my_panels[1]);
	set_panel_userptr(my_panels[1], my_panels[0]);
	//set_panel_userptr(my_panels[2], my_panels[3]);
	//set_panel_userptr(my_panels[3], my_panels[4]);
	//set_panel_userptr(my_panels[4], my_panels[0]);	
	
	/* Update the stacking order. 0 panel will be on top */
	update_panels();

	/* Show it on the screen */
	attron(COLOR_PAIR(5));
	mvprintw(0/*LINES-2*/, 0, "#logout #lobby #chatrooms");
	attroff(COLOR_PAIR(3));
	doupdate();

	top = my_panels[0];
	int x = 8;
	while((ch = getch()) != KEY_F(1))
	{
	    mvwprintw(my_wins[0], 38, x, "%c", ch);
	    x++;	
	    switch(ch)
		{	
		        case 9:
				top = (PANEL *)panel_userptr(top);
				top_panel(top);
				break;
				
				case 13:
				
		}
		update_panels();
		doupdate();
	}
	endwin();
	return 0;
}

/* Put all the windows */
void init_wins(WINDOW **wins, int n)
{	int x, y, i;
	char label[80];

	y = 2;
	x = 10;
	for(i = 0; i < n; ++i)
	{	
	    wins[i] = newwin(NLINES, NCOLS, y, x);
		sprintf(label, "Window Number %d", i + 1);
		win_show(wins[i], label, i + 1);
		//y += 3;
		//x += 7;
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
