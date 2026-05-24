#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "buffer.h"
#include "ui.h"
#include "operations.h"

static int screen_map[MAX_LINES];
static int total_lines   = 0;
static int selected_row  = 0;
static int selected_col  = 4;

static void redraw(void)
{
    int row = 0;
    int idx = head;

    clear();
    total_lines = 0;

    while (idx != -1 && row < MAX_LINES) {
        screen_map[row] = row;

        if (row == selected_row) attron(A_REVERSE);
        mvprintw(row, 0, "%2d: %s", row + 1, text_buffer[idx].statement);
        if (row == selected_row) attroff(A_REVERSE);

        idx = text_buffer[idx].next;
        row++;
        total_lines++;
    }

    int status_row = LINES - 1;
    attron(A_REVERSE);
    mvhline(status_row, 0, ' ', COLS);
    mvprintw(status_row, 0,
        " UP/DOWN: move  |  I: insert after  |  D: delete  |  R: replace char  |  Q: quit");
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

    int ch;
    while ((ch = getch()) != 'Q' && ch != 'q' && ch != 27) {
        switch (ch) {

        case KEY_UP:
            if (selected_row > 0) {
                selected_row--;
                selected_col = 4;
                redraw();
            }
            break;

        case KEY_DOWN:
            if (selected_row < total_lines - 1) {
                selected_row++;
                selected_col = 4;
                redraw();
            }
            break;

        case KEY_LEFT:
            if (selected_col > 4) {
                selected_col--;
                move(selected_row, selected_col);
                refresh();
            }
            break;

        case KEY_RIGHT: {
            int idx = head;
            for (int r = 0; r < selected_row && idx != -1; r++)
                idx = text_buffer[idx].next;
            if (idx != -1) {
                int len = (int)strlen(text_buffer[idx].statement);
                if (selected_col < 4 + len - 1) {
                    selected_col++;
                    move(selected_row, selected_col);
                    refresh();
                }
            }
            break;
        }

        case 'I':
        case 'i': {
            char new_line[MAX_CHAR];
            new_line[0] = '\0';

            int srow = LINES - 1;
            attron(A_REVERSE);
            mvhline(srow, 0, ' ', COLS);
            mvprintw(srow, 0, " New line: ");
            attroff(A_REVERSE);
            move(srow, 11);
            refresh();

            echo();
            getnstr(new_line, MAX_CHAR - 1);
            noecho();

            /* insert() inserts BEFORE node_at(index).
               We want to insert AFTER selected_row, so pass selected_row+1. */
            insert(selected_row + 1);

            /* Find the newly inserted slot (it's now at position selected_row+1) */
            int slot = head;
            for (int r = 0; r < selected_row + 1 && slot != -1; r++)
                slot = text_buffer[slot].next;
            if (slot != -1) {
                strncpy(text_buffer[slot].statement, new_line, MAX_CHAR - 1);
                text_buffer[slot].statement[MAX_CHAR - 1] = '\0';
            }

            if (selected_row < total_lines - 1)
                selected_row++;
            selected_col = 4;
            redraw();
            break;
        }

        case 'D':
        case 'd':
            if (total_lines > 0) {
                delete(selected_row);
                if (selected_row > 0 && selected_row >= total_lines - 1)
                    selected_row--;
                selected_col = 4;
                redraw();
            }
            break;

        case 'R':
        case 'r': {
            int char_idx = selected_col - 4;

            int srow = LINES - 1;
            attron(A_REVERSE);
            mvhline(srow, 0, ' ', COLS);
            mvprintw(srow, 0, " Replace char at col %d — press new character: ", char_idx);
            attroff(A_REVERSE);
            move(selected_row, selected_col);
            refresh();

            /* replace() in operations.c uses getchar() which breaks in ncurses.
               We read the key with getch() here and update the buffer directly. */
            int newch = getch();
            if (newch >= 32 && newch < 127) {  /* printable ASCII only */
                int idx = head;
                for (int r = 0; r < selected_row && idx != -1; r++)
                    idx = text_buffer[idx].next;
                if (idx != -1) {
                    int len = (int)strlen(text_buffer[idx].statement);
                    if (char_idx >= 0 && char_idx < len)
                        text_buffer[idx].statement[char_idx] = (char)newch;
                }
            }

            selected_col = 4;
            redraw();
            break;
        }

        default:
            break;
        }
    }

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