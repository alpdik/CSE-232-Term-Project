#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "editor.h"
#include "buffer.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

<<<<<<< Updated upstream
void edit(char *filename) {
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("ERROR: Could not open file %s\n", filename);
        return;
    }

    buffer_init();
    strcpy(current_file, filename);

    int i = 0;
    while (i < MAX_LINES && fgets(text_buffer[i].statement, MAX_CHAR, file) != NULL) {
        int len = strlen(text_buffer[i].statement);
        if (len > 0 && text_buffer[i].statement[len - 1] == '\n') {
            text_buffer[i].statement[len - 1] = '\0';
        }

        text_buffer[i].prev = i - 1;
        text_buffer[i].next = i + 1;

        i++;
    }

    fclose(file);

    if (i == 0) {
        printf("ERROR: %s is empty\n", filename);
        return;
    }

    text_buffer[i - 1].next = -1;
    
    head = 0;
    tail = i - 1;
    free_index = i;
}

void save() {
    if (current_file[0] == '\0') {
        printf("ERROR: No file currently open\n");
        return;
    }

    FILE *file = fopen(current_file, "w");
    
    if (file == NULL) {
        printf("ERROR: Could not open file %s for writing\n", current_file);
        return;
    }

    int i = head;
    while (i != -1) {
        fprintf(file, "%s\n", text_buffer[i].statement);
        i = text_buffer[i].next;
    }

    fclose(file);
    printf("Saved %s\n", current_file);
}
=======
static char current_filename[256] = "";

void edit(char *filename) {
    FILE *f;
    char line[MAX_CHAR];
    int len;
    int slot;
    int i;

    strncpy(current_filename, filename, sizeof(current_filename) - 1);
    current_filename[sizeof(current_filename) - 1] = '\0';

    head = -1;
    tail = -1;
    free_index = 0;
    for (i = 0; i < MAX_LINES; i++) {
        text_buffer[i].next = -1;
        text_buffer[i].prev = -1;
        text_buffer[i].statement[0] = '\0';
    }

    f = fopen(filename, "r");
    if (f == NULL) {
        return;
    }

    while (fgets(line, MAX_CHAR, f) != NULL && free_index < MAX_LINES) {
        len = (int)strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        slot = free_index;
        free_index++;

        strncpy(text_buffer[slot].statement, line, MAX_CHAR);
        text_buffer[slot].statement[MAX_CHAR - 1] = '\0';
        text_buffer[slot].prev = tail;
        text_buffer[slot].next = -1;

        if (tail == -1) {
            head = slot;
        } else {
            text_buffer[tail].next = slot;
        }
        tail = slot;
    }

    fclose(f);
}

void save() {
    FILE *f;
    int i;

    if (current_filename[0] == '\0') {
        return;
    }

    f = fopen(current_filename, "w");
    if (f == NULL) {
        return;
    }

    i = head;
    while (i != -1) {
        fprintf(f, "%s\n", text_buffer[i].statement);
        i = text_buffer[i].next;
    }

    fclose(f);
}
>>>>>>> Stashed changes
