#include <panel.h>
#include <ncurses.h>
#include <string.h>
#include "ncurses_main.h"

int main(int argc, char **argv)
{
	int ch;
	char buf[MAXWIDTH];
	char username[25];
	char strings[100][MAXWIDTH] = {{'\0'}};;
	char temp[MAXWIDTH];
	int i = 0;
	int enter = 0;
	UI *ui;
	
	/* We need to always be aware of the size of the terminal */
	int row = 42;
	int col = 85;
    ncurses_main();
    ui = ui_init();
    //------------------------------------------------------------------------------------------------
    //                         L O G I N    S E C T I O N
    //------------------------------------------------------------------------------------------------
    make_login(ui, row, col);
    
    while(1)
    {
    ch = wgetch(ui->input);
		switch(ch)
		{
		case KEY_LEFT:
			page_left(ui, row, col);
			break;
		case KEY_RIGHT:
			page_right(ui, row, col);
			break;
		case 8:     /* same as KEY_BACKSPACE */
		case 127:   /* delete, KEY_DC */
		case KEY_DC:
		case KEY_BACKSPACE:
			page_delete(ui, row, col);
			break;
		case '\r':
		case '\n':
		case KEY_SEND:
		case KEY_ENTER:
			page_usr_enter(ui, row, col, username);
			enter = 1;
			break;
		default:
			page_default(ui, ch);
			break;
		}
		if(enter)
		{
			break;
			endwin();
		}
	}
	
	//------------------------------------------------------------------------------------------------
    //                         C H A T R O O M    S E C T I O N
    //------------------------------------------------------------------------------------------------
    make_chatroom(ui, row, col, "lobby", "System", username);

	for (;;)
	{
		ch = wgetch(ui->input);
		switch(ch)
		{
		case 9:
		//Example of how to show a new chatroom
		    endwin();
		    make_chatroom(ui, row, col, "chat1", "FRENDS", "Frenchu");
		    i = 0;
            while(strings[i][0] != '\0')
            {
                strncpy(temp, strings[i], strlen(strings[i]));
                ui_printf(ui, "%s\n", temp);
                i++;
            }
            break;
		case KEY_UP:
		case KEY_PPAGE:;
			page_up(ui);
			break;
		case KEY_DOWN:
		case KEY_NPAGE:;
			page_down(ui);
			break;
		case KEY_LEFT:
			page_left(ui, row, col);
			break;
		case KEY_RIGHT:
			page_right(ui, row, col);
			break;
		case 8:     /* same as KEY_BACKSPACE */
		case 127:   /* delete, KEY_DC */
		case KEY_DC:
		case KEY_BACKSPACE:
			page_delete(ui, row, col);
			break;
		case '\r':
		case '\n':
		case KEY_SEND:
		case KEY_ENTER:
			page_enter(ui, row, col, buf);
			strncpy(strings[i], buf, strlen(buf));
			i++;
			break;
		default:
			page_default(ui, ch);
			break;
		}
	}
	
	endwin();
	exit(0);
}

