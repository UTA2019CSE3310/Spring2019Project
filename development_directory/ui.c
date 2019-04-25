#include <panel.h>
#include <ncurses.h>
#include <string.h>
#include "ncurses_main.h"

int main(int argc, char **argv)
{
	int ch;
	char buf[MAXWIDTH];
	UI *ui_t[10];
	UI *ui; // = ui_t[0];
	
	/* We need to always be aware of the size of the terminal */
	int row = 42;
	int col = 85;
    ncurses_main(ui_t, 10);
     ui = ui_t[9];
     make_chatroom(ui, row, col, "lobby", "System", "Frenchu");
     show_chatroom(ui);

	for (;;)
	{
		ch = wgetch(ui->input);
		switch(ch)
		{
		case KEY_UP:
		case KEY_PPAGE:
			ui_scrollup(ui);
			/* This just sticks the cursor back on input */
			wrefresh(ui->input);
			break;
		case KEY_DOWN:
		case KEY_NPAGE:
			ui_scrolldown(ui);
			wrefresh(ui->input);
			break;
		case KEY_LEFT:
			getyx(ui->input, row, col);
			wmove(ui->input, row, col -1); 
			wrefresh(ui->input);
			break;
		case KEY_RIGHT:
			/*getyx(ui->input, row, col);
			wmove(ui->input, row, col +1); 
			wrefresh(ui->input);*/
			clear_win(ui);
			ui = ui_t[9];
			show_chatroom(ui);
			break;
		case 8:     /* same as KEY_BACKSPACE */
		case 127:   /* delete, KEY_DC */
		case KEY_DC:
		case KEY_BACKSPACE:
			getyx(ui->input, row, col);
			wmove(ui->input, row, col -1); 
			wdelch(ui->input);
			wrefresh(ui->input);
			break;
		case '\r':
		case '\n':
		case KEY_SEND:
		case KEY_ENTER:
			memset(buf, 0, MAXWIDTH);
			getyx(ui->input, row, col);
			wmove(ui->input, 0, 0);
			/* TODO: Not safe! */
			winnstr(ui->input, buf, col);
			wclrtoeol(ui->input);
			ui_printf(ui, "%s\n", buf);
			wrefresh(ui->input);
			clear_win(ui);
			ui = ui_t[0];
			make_chatroom(ui, row, col, "chat 0", "Freny", "Frenchu");
			show_chatroom(ui);
			break;
		default:
			waddch(ui->input, ch);
			wrefresh(ui->input);
			break;
		}
	}
	
	endwin();
	exit(0);
}

