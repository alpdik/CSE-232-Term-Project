#include <string.h>
#include <ncurses.h>
#include "buffer.h"
#include "ui.h"
#include "ui_input.h"
#include "operations.h"

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
                move(ui_get_selected_row(), ui_get_selected_col());
                refresh();
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
                    move(ui_get_selected_row(), ui_get_selected_col());
                    refresh();
                }
            }
            break;
        }

        case '\n':
        case '\r':
        case KEY_ENTER: {
            int op;

            attron(A_REVERSE);
            mvhline(LINES - 1, 0, ' ', COLS);
            mvprintw(LINES - 1, 0,
                "Line %d | Arrows move | I/D/R | Q cancel", row + 1);
            attroff(A_REVERSE);
            move(ui_get_selected_row(), ui_get_selected_col());
            refresh();

            while (1) {
                op = getch();

                if (op == 'Q' || op == 'q' || op == 27) {
                    redraw();
                    break;
                }

                if (op == KEY_LEFT) {
                    int c2 = ui_get_selected_col();

                    if (c2 > 4) {
                        ui_set_selected_col(c2 - 1);
                        move(ui_get_selected_row(), ui_get_selected_col());
                        refresh();
                    }

                } else if (op == KEY_RIGHT) {
                    int c2 = ui_get_selected_col();
                    int idx2 = head;

                    for (int r = 0; r < ui_get_selected_row() && idx2 != -1; r++)
                        idx2 = text_buffer[idx2].next;

                    if (idx2 != -1) {
                        int len2 = (int)strlen(text_buffer[idx2].statement);

                        if (c2 < 4 + len2 - 1) {
                            ui_set_selected_col(c2 + 1);
                            move(ui_get_selected_row(), ui_get_selected_col());
                            refresh();
                        }
                    }

                } else if (op == 'I' || op == 'i') {
                    insert(cursorLine());
                    ui_set_selected_col(4);
                    redraw();
                    break;

                } else if (op == 'D' || op == 'd') {
                    delete(cursorLine());

                    if (row > 0)
                        ui_set_selected_row(row - 1);

                    ui_set_selected_col(4);
                    redraw();
                    break;

                } else if (op == 'R' || op == 'r') {
                    attron(A_REVERSE);
                    mvhline(LINES - 1, 0, ' ', COLS);
                    mvprintw(LINES - 1, 0, " Type new character: ");
                    attroff(A_REVERSE);
                    move(ui_get_selected_row(), ui_get_selected_col());
                    refresh();

                    replace(cursorChar());

                    ui_set_selected_col(4);
                    redraw();
                    break;
                }
            }

            break;
        }

        default:
            break;
        }
    }
}