#ifndef UI_H
#define UI_H

void print(void);
int  cursorLine(void);
int  cursorChar(void);

/* Internal state accessors — declared here so ui_input.c can use them
   without forward declarations */
int  ui_get_total_lines(void);
int  ui_get_selected_row(void);
int  ui_get_selected_col(void);
void ui_set_selected_row(int r);
void ui_set_selected_col(int c);
void ui_place_cursor(void);
void redraw(void);

#endif