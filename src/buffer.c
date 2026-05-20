#include "buffer.h"

struct Node text_buffer[MAX_LINES];
int head;
int tail;
int free_index;
char current_file[50];

void buffer_init() {
    head = -1;
    tail = -1;
    free_index = 0;
    current_file[0] = '\0';

    for (int i = 0; i < MAX_LINES; i++) {
        text_buffer[i].next = -1;
        text_buffer[i].prev = -1;
        text_buffer[i].statement[0] = '\0';
    }
}