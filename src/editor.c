#include "editor.h"
#include "buffer.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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