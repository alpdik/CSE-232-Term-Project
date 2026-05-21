#include <string.h>
#include "buffer.h"
#include "gc.h"

int garbageCollection() {
    struct Node compacted[MAX_LINES];
    int count;
    int i;
    int j;

    count = 0;
    i = head;
    while (i != -1) {
        strncpy(compacted[count].statement, text_buffer[i].statement, MAX_CHAR);
        compacted[count].statement[MAX_CHAR - 1] = '\0';
        count++;
        i = text_buffer[i].next;
    }

    for (j = 0; j < MAX_LINES; j++) {
        text_buffer[j].next = -1;
        text_buffer[j].prev = -1;
        text_buffer[j].statement[0] = '\0';
    }

    for (j = 0; j < count; j++) {
        strncpy(text_buffer[j].statement, compacted[j].statement, MAX_CHAR);
        text_buffer[j].statement[MAX_CHAR - 1] = '\0';
        text_buffer[j].prev = (j == 0) ? -1 : j - 1;
        text_buffer[j].next = (j == count - 1) ? -1 : j + 1;
    }

    head = (count == 0) ? -1 : 0;
    tail = (count == 0) ? -1 : count - 1;
    free_index = count;

    return count;
}
