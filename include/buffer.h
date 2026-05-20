#ifndef  BUFFER_H
#define  BUFFER_H

#define MAX_LINES 100
#define MAX_CHAR 40
struct Node {
    char statement[MAX_CHAR];
    int next;
    int prev;
};

extern struct Node text_buffer[100];
extern int head;
extern int tail;
extern int free_index;

extern char current_file[50];

void buffer_init();

#endif