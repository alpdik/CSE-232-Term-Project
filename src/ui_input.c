#include <string.h>
#include <ncurses.h>
#include "buffer.h"
#include "ui.h"
#include "ui_input.h"
#include "operations.h"

/* accessors from ui.c */
int  ui_get_total_lines(void);
int  ui_get_selected_row(void);
int  ui_get_selected_col(void);
void ui_set_selected_row(int r);
void ui_set_selected_col(int c);
void redraw(void);

/* ── handleInput ────────────────────────────────────────────────── */
void handleInput(void)
{
    int ch;

    while ((ch = getch()) != 'Q' && ch != 'q' && ch != 27) {

        int row   = ui_get_selected_row();
        int col   = ui_get_selected_col();
        int total = ui_get_total_lines();

        switch (ch) {

        case KEY_UP:
            if (row > 0) {
                ui_set_selected_row(row - 1);
                ui_set_selected_col(4);
                redraw();
            }
            break;

        case KEY_DOWN:
            if (row < total - 1) {
                ui_set_selected_row(row + 1);
                ui_set_selected_col(4);
                redraw();
            }
            break;

        case KEY_LEFT:
            if (col > 4) {
                ui_set_selected_col(col - 1);
                redraw();
            }
            break;

        case KEY_RIGHT: {
            int idx = head;
            for (int r = 0; r < row && idx != -1; r++)
                idx = text_buffer[idx].next;
            if (idx != -1) {
                int len = (int)strlen(text_buffer[idx].statement);
                if (col < 4 + len - 1) {
                    ui_set_selected_col(col + 1);
                    redraw();
                }
            }
            break;
        }

        /* ENTER: confirm selection, then wait for operation key */
        case '\n':
        case '\r':
        case KEY_ENTER: {
            attron(A_REVERSE);
            mvhline(LINES - 1, 0, ' ', COLS);
            mvprintw(LINES - 1, 0,
                " Line %d selected — press I / D / R  or  Q to cancel", row + 1);
            attroff(A_REVERSE);
            move(row, ui_get_selected_col());
            refresh();

            int op = getch();

            if (op == 'I' || op == 'i') {
                char new_line[MAX_CHAR];
                new_line[0] = '\0';

                attron(A_REVERSE);
                mvhline(LINES - 1, 0, ' ', COLS);
                mvprintw(LINES - 1, 0, " New line: ");
                attroff(A_REVERSE);
                move(LINES - 1, 11);
                refresh();

                echo();
                getnstr(new_line, MAX_CHAR - 1);
                noecho();

                /* insert BEFORE node_at(row+1) = AFTER selected row */
                insert(row + 1);

                /* write text into the newly created slot */
                int slot = head;
                for (int r = 0; r < row + 1 && slot != -1; r++)
                    slot = text_buffer[slot].next;
                if (slot != -1) {
                    strncpy(text_buffer[slot].statement, new_line, MAX_CHAR - 1);
                    text_buffer[slot].statement[MAX_CHAR - 1] = '\0';
                }

                if (row < ui_get_total_lines() - 1)
                    ui_set_selected_row(row + 1);
                ui_set_selected_col(4);
                redraw();

            } else if (op == 'D' || op == 'd') {
                delete(row);
                int new_total = ui_get_total_lines() - 1;
                if (row > 0 && row >= new_total)
                    ui_set_selected_row(row - 1);
                ui_set_selected_col(4);
                redraw();

            } else if (op == 'R' || op == 'r') {
                /* Let user position cursor with LEFT/RIGHT, then ENTER to confirm */
                attron(A_REVERSE);
                mvhline(LINES - 1, 0, ' ', COLS);
                mvprintw(LINES - 1, 0,
                    " LEFT/RIGHT to position, ENTER to replace: ");
                attroff(A_REVERSE);
                move(ui_get_selected_row(), ui_get_selected_col());
                refresh();

                int rc;
                while ((rc = getch()) != '\n' && rc != '\r' && rc != KEY_ENTER) {
                    int c2 = ui_get_selected_col();
                    if (rc == KEY_LEFT && c2 > 4) {
                        ui_set_selected_col(c2 - 1);
                        redraw();
                    } else if (rc == KEY_RIGHT) {
                        int idx2 = head;
                        for (int r = 0; r < ui_get_selected_row() && idx2 != -1; r++)
                            idx2 = text_buffer[idx2].next;
                        if (idx2 != -1) {
                            int len2 = (int)strlen(text_buffer[idx2].statement);
                            if (c2 < 4 + len2 - 1) {
                                ui_set_selected_col(c2 + 1);
                                redraw();
                            }
                        }
                    }
                }

                /* replace() in operations.c calls cursorLine() and cursorChar()
                   which now return the correct values, then reads char with getchar().
                   We call it directly — Alp needs to update getchar() to getch(). */
                replace(cursorChar());

                ui_set_selected_col(4);
                redraw();

            } else {
                redraw();
            }
            break;
        }

        default:
            break;
        }
    }
}