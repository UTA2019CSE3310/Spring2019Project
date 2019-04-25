/*
 * A reference implementation for a scrolling ncurses window.
 *
 * Copyright (c) 2013 Daniel Corbe
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the authors, copyright holders or the contributors
 *    may be used to endorese or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS, AUTHORS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS OR CONTRIBUTORS BE HELD LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, COPYRIGHT ENFRINGEMENT, LOSS
 * OF PROFITS, REVENUE, OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef UI_H
#define UI_H

/* Maximum width of the terminal */
#define MAXWIDTH 256

/* Maximum number of lines to keep in memory */
#define SCROLLMAX 1000

/*
 * The scrollback buffer is represented
 * internally by a linked list.  The list
 * head should always point to the *LAST*
 * message in the buffer and iterate upwards
 * via the prev pointer. 
 *
 * Some notes:
 *  1) The string is permanently stored here.  ui->s is malloc()d 
 *     every time a line is added to the scroll buffer.
 *  2) We chomp anything that would create a new line (\r, \n, etc)
 *     to avoid screwing up the way the UI is painted.
 */
typedef struct ui_scroll
{
	size_t l;                       /* Length of the string */
	char *s;                        /* The string itself */
	struct ui_scroll *prev, *next;  /* For iteration */
	struct ui_scroll *top;          /* First line (oldest) */
	struct ui_scroll *bot;          /* Last line (newest) */
	struct ui_scroll *head;         /* Last unused position */
} UI_SCROLL;

/*
 * This is the object which describes our UI and gets
 * passed around from function to function
 *
 * Some Notes:
 *  1) pos is the current position of the scroll buffer.  It's largely
 *     tracked and used internally.  PageUP and PageDOWN scrolls the screen
 *     by int(rows/2).  If pos=0 that means we're at the bottom of the screen
 */
typedef struct ui
{
	NCURSES_BOOL newline;           /* If true, we need to insert a
					 * '\n' on the next pass in order
					 *  to scroll the window.
					 */
	UI_SCROLL *scroll;              /* The scrollback buffer */

	WINDOW *input;                  /* The input bar */
	WINDOW *text;                   /* The scolling text window */
	WINDOW *title;
	int rows, cols;                 /* The dimensions of the terminal */
	int lines;                      /* The line count */
	int pos;                        /* Scroll position */
} UI;


/* 
 * Public API (function prototypes)
 */
int main();

/*
 * ui_init() initializes the scrollback buffer and the ui struct
 */
UI *ui_init(void);

/*
 * Add a line to the scroll window
 *
 * If l = 0, then ui_addline() will attempt to figure out the
 * size of the string.  NOTE:  The string MUST be null-terminated.
 */
void ui_addline(UI *ui, size_t l, char *s);


/*
 * ui_scrollup() and ui_scrolldown() scroll the text area by exactly
 * half of the screen height
 */
void ui_scrollup(UI *ui);
void ui_scrolldown(UI *ui);
void ui_repaint(UI *ui);
int ui_printf(UI *ui, char *format, ...);
int waddch_stub(WINDOW *w, chtype c);
void ui_chomp(char *s);

/*
 * Some utility functions stolen from the K&R book.  Needed by ui_printf()
 */
void portable_itoa(int value, char *str, int base);
void strreverse(char* begin, char* end);

UI *ui_init(void)
{
	UI *ui;
	UI_SCROLL *ptr;

	ui = malloc(sizeof(struct ui));

	/* Initial values for the UI element */
	ui->lines = 0;
	ui->pos = 0;

	ui->input = NULL;
	ui->text = NULL;
	ui->title = NULL;
	ui->scroll = malloc(sizeof(struct ui_scroll));

	/* Initial values for the scrollback buffer */
	ptr = ui->scroll;
	ptr->prev = NULL;
	ptr->next = NULL;
	ptr->bot = ptr;
	ptr->top = ptr;
	ptr->head = ptr;
	ptr->l = 0;
	ptr->s = NULL;

	return ui;
}

void ui_addline(UI *ui, size_t l, char *s)
{
	UI_SCROLL *ptr;
	int rows, cols;

	getmaxyx(ui->text, rows, cols);

	/* Find the first unused position in the scrollback buffer */
	ptr = ui->scroll->head->bot;
	ptr->next = malloc(sizeof(struct ui_scroll));
	ptr->next->prev = ptr;
	ptr->next->next = NULL;
	ptr->head = ui->scroll->head;
	ptr->top = ui->scroll->head->top;
	ptr->bot = ptr->next; 
	ui->scroll->head->bot = ptr->next;
	ptr = ptr->next;

	/*
	 * TODO: Free the top of the list if we're >SCROLLMAX
	 */

	/*
	 * Make sure we know the size of the string
	 */
	if (l == 0)
		l = strlen(s);

	/*
	 * TODO:  We need to chomp any CR/LF characters
	 */

	/*
	 * Store the string.
	 */
	ptr->l = l;
	ptr->s = malloc(l + 1);           /* +1 for the \0 */
	strncpy(ptr->s, s, l);
	ptr->s[l] = '\0';		  /* Manually null-term */

	/*
	 * Because we need to know
	 */
	ui->lines++;
}

void ui_scrollup(UI *ui)
{
	int rows, cols;

	getmaxyx(ui->text, rows, cols);

	if (ui->lines <= rows || ui->pos + rows >= ui->lines)
	{
		beep();
		return;
	}

	/* Calculate new bottom position */
	ui->pos += (rows / 2);

	/* We might not be able to scroll back that far.  Adjust accordingly */
	if (ui->pos >= (ui->lines - rows))
		ui->pos = ui->lines - rows;

	/* repaint the text window */
	ui_repaint(ui);

	return;
}

void ui_scrolldown(UI *ui)
{
	int rows, cols;

	getmaxyx(ui->text, rows, cols);
	if (ui->pos < 1)      /* Already at the bottom of the window */
	{
		beep();
		return;
	}

	/* Calculate new bottom position */
	ui->pos -= (rows / 2);

	/* We might not be able to scroll forward that far.
	   Adjust accordingly */
	if (ui->pos < 0)
		ui->pos = 0;


	/* repaint the text window */
	ui_repaint(ui);

	return;
}

void ui_repaint(UI *ui)
{
	int rows, cols;          /* Dimensions of the screen */
    int row, col;           /* Save the original cursor position
	                                * so it can be restored later 
				                    */
	int i;
	UI_SCROLL *ptr;

	getmaxyx(ui->text, rows, cols);
	getyx(ui->text, row, col);

	/* Mark our new bottom position using ptr */
	ptr = ui->scroll->head->bot;
	for (i = 1; i < ui->pos; i++)
	{
	       	ptr = ptr->prev;
	}

	/* 
	 * Now we need to go back through line-by-line and repaint the text
	 * window.  We do it without newlines this time to prevent the window
	 * from being scrolled (and thus destroying the terminal's scroll
	 * buffer)
	 */
	for (i = rows - 1; i > -1; i--)
	{
		wmove(ui->text, i, 0);
		wclrtoeol(ui->text);
		wprintw(ui->text, "%s", ptr->s);
		ptr = ptr->prev;
	}

	/*
	 * When we're done repainting the text window, the cursor needs to
	 * be repositioned back in its original position.
	 */
	wmove(ui->text, row, col);
	
	/* And finally, a refresh is in order */
	wrefresh(ui->text);
}


/*
 * It actually does more than wprintw():
 *  - Keeps track of lines and properly updates the UI * element.
 *  - Interprets '\r' and resets the cursor position via wmove() 
 *
 *    We want to delay echoing the final '\n' so that we don't end up with
 * an extra row of empty characters at the bottom of the window.  To
 * facilitate this, we run ui_chomp() to convert it to the ASCII ACK marker
 * (\006).  When we encounter ACK in the format string, we treat it like a
 * newline without echoing it and then we set the ui->newline flag so we can
 * dump it to the terminal on the next pass.
 *
 * This was a lot of work, but it was worth it.
 *
 * TODO:
 *
 * 1) Does not yet support octals.
 * 2) Does not yet support 0 padding of Integers.
 * 3) Does not support specifying string length, IE %.*s
 * 4) Missing quite a lot of formatting characters.
 * 5) Missing some useful escape sequences.
 * 6) May or may not support ansi color .  I haven't tested yet.
 */

int ui_printf(UI *ui, char *format, ...)
{
	va_list ap;
	char ch;
	int row, col, i;

	/*
	 * Our % flags
	 */
	const char *ss;
	int ii;
	char cc;

	/*
	 * Used for keeping track of line-by-line IO.
	 */
	char buf[256];
	int count = 0;

	/*
	 * Grossly abusive but it saves us from having to test ui->pos prior
	 * to every single waddch() call.
	 */
	int (*l_waddch)(WINDOW *, chtype);
	if (ui->pos > 0)
		l_waddch = waddch_stub;
	else
		l_waddch = waddch;

	/* Do we need to insert a newline from a previous pass? */
	if (ui->newline == TRUE)
	{
		l_waddch(ui->text, '\n');
		ui->newline = FALSE;
	}

	/*
	 * We need to modify the format string, but we must also guarantee
	 * SEGV safety in the process.  The only way to do that is to make
	 * a copy of format on the heap...
	 */
	char *fmt = malloc(strlen(format) + 1);
	strncpy(fmt, format, strlen(format));
	ui_chomp(fmt);
	format = fmt;

	/* Now, the heavy lifting. */
	va_start(ap, format);
	while (*format)
	{
		ch = *format++;
		if (ch == '%')
		{
			switch(*format++)
			{
			case 's':
				ss = va_arg(ap, const char *);
				for (i = 0; i < strlen(ss); i++)
				{
					buf[count] = ss[i];
					count++;
					l_waddch(ui->text, ss[i]);
				}
				continue;
			case 'd':
			case 'i':
				ii = va_arg(ap, int);
				portable_itoa(ii, &buf[count], 10);
				for (i = 0; i < strlen(&buf[count]); i++)
				{
					count++;
					l_waddch(ui->text, (&buf[count])[i]);
				}
				continue;
			case 'c':
				cc = va_arg(ap, int);
				buf[count] = cc;
				count++;
				l_waddch(ui->text, cc);
				continue;
			case '%':
			default:
				buf[count] = ch;
				count++;
				l_waddch(ui->text, ch);
				continue;
			}
		}
		else
		{
			switch(ch)
			{
			case '\a':
				beep();
				continue;
			case '\n':
				l_waddch(ui->text, ch);
				buf[count] = '\0';
				ui_addline(ui, count, buf);
				count = 0;
				buf[0] = '\0';
				continue;
			case 6:
				ui->newline = TRUE;
				buf[count] = '\0';
				ui_addline(ui, count, buf);
				count = 0;
				buf[0] = '\0';
				continue;
			case '\r':
				buf[count] = ch;
				count++;
				getyx(ui->text, row, col);
				wmove(ui->text, row, 0);
				wrefresh(ui->text);
				continue;
			default:
				buf[count] = ch;
				count++;
				l_waddch(ui->text, ch);
				continue;
			}
		}		       
	}
	va_end(ap);
	free(fmt);

	wrefresh(ui->text);
}

/*	
 * Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C"
 */
void portable_itoa(int value, char *str, int base) 
{
	static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	char *wstr;
	int sign;
	
	wstr = str;
	
	/* Validate Base */	
	if (base < 2 || base > 35)
	{ 
		*wstr = '\0'; 
		return; 
	}
	
	/* Take care of sign */
	if ((sign = value) < 0)
		value = -value;
	
	/* Conversion. Number is reversed. */	
	do *wstr++ = num[value % base]; while(value /= base);
	
	if(sign < 0)
		*wstr++='-';

	*wstr = '\0';
		
	/* Reverse string */
	strreverse(str, wstr - 1);	
}

void strreverse(char* begin, char* end) 
{
	char aux;
	
	while(end>begin)	
		aux = *end, *end-- = *begin, *begin++ = aux;	
}

/*
 * If we're not going to actually do anything, Just return OK and be done.
 */
int waddch_stub(WINDOW *w, chtype c)
{
	return OK;
}

void ui_chomp(char *s)
{
	int i;
	
	i = (strlen(s) - 1);
	if (s[i] == '\n')
		s[i] = 6;
}
#endif /* UI_H */
