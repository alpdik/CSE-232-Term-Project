#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "operations.h"
#include "ui.h"

static int node_at(int pos) {
    int i;
    int n;

    i = head;
    n = 0;
    while (i != -1 && n < pos) {
        i = text_buffer[i].next;
        n++;
    }
    return i;
}

void insert(int index) {
    int slot;
    int target;
    int p;

    if (free_index >= MAX_LINES) {
        return;
    }

    slot = free_index;
    free_index++;
    text_buffer[slot].statement[0] = '\0';

    if (head == -1) {
        text_buffer[slot].prev = -1;
        text_buffer[slot].next = -1;
        head = slot;
        tail = slot;
        return;
    }

    target = node_at(index);

    if (target == -1) {
        text_buffer[slot].prev = tail;
        text_buffer[slot].next = -1;
        text_buffer[tail].next = slot;
        tail = slot;
    } else {
        p = text_buffer[target].prev;
        text_buffer[slot].prev = p;
        text_buffer[slot].next = target;
        text_buffer[target].prev = slot;
        if (p == -1) {
            head = slot;
        } else {
            text_buffer[p].next = slot;
        }
    }
}

void delete(int index) {
    int target;
    int p;
    int n;

    target = node_at(index);
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
    target = node_at(line);
    if (target == -1) {
        return;
    }

    ch = getchar();
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
