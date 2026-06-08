#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "operations.h"
#include "ui.h"
#include <ncurses.h>


void insert(int index) {
    int slot;
    int target;
    int p;

    if (free_index >= MAX_LINES) {
        return;
    }

    slot = free_index;
    free_index++;

    echo();
    getnstr(text_buffer[slot].statement, MAX_CHAR - 1);
    noecho();

    text_buffer[slot].statement[MAX_CHAR - 1] = '\0';

    if (head == -1) {
        text_buffer[slot].prev = -1;
        text_buffer[slot].next = -1;
        head = slot;
        tail = slot;
        return;
    }

    target = index;

    if (target == -1) {
        text_buffer[slot].prev = tail;
        text_buffer[slot].next = -1;
        text_buffer[tail].next = slot;
        tail = slot;
    } else {
        p = text_buffer[target].next;

        text_buffer[slot].prev = target;
        text_buffer[slot].next = p;
        text_buffer[target].next = slot;
        if (p == -1) {
            tail = slot;
        } else {
            text_buffer[p].prev = slot;
        }
        
    }
}

void delete(int index) {
    int target;
    int p;
    int n;

    target = index;
    if (target == -1) {
        return;
    }

    p = text_buffer[target].prev;
    n = text_buffer[target].next;

    if (p == -1) {
        head = n;
    } else {
        text_buffer[p].next = n;
    }

    if (n == -1) {
        tail = p;
    } else {
        text_buffer[n].prev = p;
    }

    text_buffer[target].next = -1;
    text_buffer[target].prev = -1;
    text_buffer[target].statement[0] = '\0';
}

void replace(int index) {
    int line;
    int target;
    int ch;
    int len;

    if (index < 0 || index >= MAX_CHAR - 1) {
        return;
    }

    line = cursorLine();
    target = line;
    if (target == -1) {
        return;
    }

    ch = getch();
    if (ch == EOF || ch == '\n') {
        return;
    }

    len = (int)strlen(text_buffer[target].statement);
    if (index > len) {
        return;
    }
    if (index == len) {
        text_buffer[target].statement[index + 1] = '\0';
    }
    text_buffer[target].statement[index] = (char)ch;
}
