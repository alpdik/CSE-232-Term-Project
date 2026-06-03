#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "buffer.h"
#include "ui.h"
#include "ui_input.h"

static int total_lines  = 0;
static int selected_row = 0;
static int selected_col = 4;

int  ui_get_total_lines(void)   { return total_lines;  }
int  ui_get_selected_row(void)  { return selected_row; }
int  ui_get_selected_col(void)  { return selected_col; }
void ui_set_selected_row(int r) { selected_row = r;    }
void ui_set_selected_col(int c) { selected_col = c;    }

void redraw(void)
{
    int row = 0;
    int idx = head;

    clear();
    total_lines = 0;

    while (idx != -1 && row < MAX_LINES) {
        if (row == selected_row) attron(A_REVERSE);
        mvprintw(row, 0, "%2d: %s", row + 1, text_buffer[idx].statement);
        if (row == selected_row) attroff(A_REVERSE);

        idx = text_buffer[idx].next;
        row++;
        total_lines++;
    }

    attron(A_REVERSE);
    mvhline(LINES - 1, 0, ' ', COLS);
    mvprintw(LINES - 1, 0,
        " UP/DOWN: move  |  ENTER: select  |  I: insert  |  D: delete  |  R: replace  |  Q: quit");
    attroff(A_REVERSE);

    move(selected_row, selected_col);
    refresh();
}

void print(void)
{
    if (head == -1) {
        printf("Buffer is empty. Use E <filename> to open a file.\n");
        return;
    }

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);

    selected_row = 0;
    selected_col = 4;

    redraw();
    handleInput();

    endwin();
}

int cursorLine(void)
{
    return selected_row;
}

int cursorChar(void)
{
    int c = selected_col - 4;
    return (c < 0) ? 0 : c;
}
