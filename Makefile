CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LIBS = -lncurses

SRC = $(wildcard src/*.c)
BIN = editor

all: check-dependency $(BIN)

check-dependency:
	@pkg-config --exists ncurses || (echo "ERROR: ncurses not found. Check README file for installation instructions." && exit 1)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC) $(LIBS)

preprocess: $(SRC)
	$(CC) $(CFLAGS) -E src/main.c -o preprocessed.c

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN) preprocessed.c

.PHONY: all preprocess check-dependency run clean