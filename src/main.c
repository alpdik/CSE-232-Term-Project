#include <stdio.h>
#include "buffer.h"
#include "editor.h"
#include "operations.h"
#include "gc.h"
#include "ui.h"

int main() {
    buffer_init();

    char command[10];
    int flag = 1;

    while (flag) {
        printf("> ");
        scanf("%s", command);

        switch (command[0]) {
            case 'E': {
                char filename[50];
                scanf("%s", filename);
                edit(filename);
                break;
            }
            case 'I':
                insert(cursorLine());
                break;
            case 'D':
                delete(cursorLine());
                break;
            case 'R':
                replace(cursorChar());
                break;
            case 'P':
                print();
                break;
            case 'S':
                save();
                break;
            case 'G':
                garbageCollection();
                break;
            case 'Q':
                flag = 0;
                break;
            default:
                printf("Unknown command\n");
        }
    }

    return 0;
}