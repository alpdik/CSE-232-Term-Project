#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "buffer.h"
#include "ui.h"
#include "ui_input.h"

static int total_lines   = 0;
static int selected_row  = 0;   /* logical line index into the buffer   */
static int selected_col  = 4;
static int scroll_offset = 0;   /* first buffer line shown on screen     */

int  ui_get_total_lines(void)   { return total_lines;  }
int  ui_get_selected_row(void)  { return selected_row; }
int  ui_get_selected_col(void)  { return selected_col; }
void ui_set_selected_row(int r) { selected_row = r;    }
void ui_set_selected_col(int c) { selected_col = c;    }

/* Number of buffer rows that actually fit (last terminal row is the
   status bar). */
static int visible_rows(void)
{
    int rows = LINES - 1;
    return (rows < 1) ? 1 : rows;
}

/* Position the hardware cursor, translating the logical line index into a
   screen row via the current scroll offset. */
void ui_place_cursor(void)
{
    move(selected_row - scroll_offset, selected_col);
    refresh();
}

void redraw(void)
{
    int rows = visible_rows();
    int idx, line, screen_row;

    clear();

    /* Count every line in the buffer, not just the ones that fit. */
    total_lines = 0;
    for (idx = head; idx != -1; idx = text_buffer[idx].next)
        total_lines++;

    /* Clamp the cursor into range. */
    if (selected_row >= total_lines && total_lines > 0)
        selected_row = total_lines - 1;
    if (selected_row < 0)
        selected_row = 0;
    if (selected_col < 4)
        selected_col = 4;

    /* Scroll just enough to keep the selected line on screen. */
    if (selected_row < scroll_offset)
        scroll_offset = selected_row;
    else if (selected_row >= scroll_offset + rows)
        scroll_offset = selected_row - rows + 1;
    if (scroll_offset < 0)
        scroll_offset = 0;

    /* Walk to the first visible line. */
    idx  = head;
    line = 0;
    for (; line < scroll_offset && idx != -1; line++)
        idx = text_buffer[idx].next;

    /* Draw the visible window, truncating long lines to the terminal
       width so they never wrap and corrupt the layout. */
    for (screen_row = 0; idx != -1 && screen_row < rows; screen_row++) {
        int selected = (line == selected_row);

        attron(COLOR_PAIR(selected ? 1 : 3));
        mvprintw(screen_row, 0, "%2d: %.*s",
                 line + 1, COLS - 4, text_buffer[idx].statement);
        attroff(COLOR_PAIR(selected ? 1 : 3));

        idx = text_buffer[idx].next;
        line++;
    }

    attron(A_REVERSE);
    mvhline(LINES - 1, 0, ' ', COLS);
    mvprintw(LINES - 1, 0,
        " UP/DOWN: move  |  ENTER: select  |  I: insert  |  D: delete  |  R: replace  |  Q: quit");
    attroff(A_REVERSE);

    ui_place_cursor();
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
 
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);

    selected_row = 0;
    selected_col = 4;
 
    redraw();
    handleInput();
 
    endwin();
}

int cursorLine(void)
{
    int idx = head;
    int r   = 0;

    while (idx != -1 && r < selected_row) {
        idx = text_buffer[idx].next;
        r++;
    }

    return idx;
}

int cursorChar(void)
{
    int c = selected_col - 4;
    return (c < 0) ? 0 : c;
}