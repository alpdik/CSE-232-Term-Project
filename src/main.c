#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "editor.h"
#include "operations.h"
#include "gc.h"
#include "ui.h"
#include "ui_input.h"

int main(void)
{
    buffer_init();

    char command[64];
    int running = 1;

    while (running) {
        printf("> ");
        fflush(stdout);

        if (scanf("%63s", command) != 1)
            break;

        switch (command[0]) {

        case 'E': {
            char filename[50];
            if (scanf("%49s", filename) == 1)
                edit(filename);
            else
                printf("Usage: E <filename>\n");
            break;
        }

        case 'P':
            print();   /* enters ncurses, returns when user presses Q */
            break;

        case 'S':
            save();
            break;

        case 'G':
            garbageCollection();
            printf("Garbage collection done.\n");
            break;

        case 'Q':
        case 'q':
            running = 0;
            break;

        /* I / D / R are handled INSIDE the ncurses print() loop.
           If someone types them at the main prompt we remind them. */
        case 'I':
        case 'D':
        case 'R':
            printf("Use P to open the editor, then press %c inside it.\n",
                   command[0]);
            break;

        default:
            printf("Unknown command: %s\n", command);
            printf("Commands: E <file>  P  S  G  Q\n");
            break;
        }
    }

    return 0;
}