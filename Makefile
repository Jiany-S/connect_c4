CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -g
SRCDIR = src
BUILDDIR = build
SRCS = $(wildcard $(SRCDIR)/*.c)
TARGET = $(BUILDDIR)/connect4

.PHONY: all debug release clean run test valgrind gdb-run

all: CFLAGS += -O2
all: $(TARGET)

debug: CFLAGS += -g -O0 -DDEBUG
debug: clean $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	./tests/test_runner.sh

valgrind: debug $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log $(TARGET) || true

gdb-run: debug $(TARGET)
	gdb -batch -ex "run" -ex "bt" -ex "info locals" --args $(TARGET) | tee gdb_trace.txt

clean:
	rm -rf $(BUILDDIR) valgrind.log gdb_trace.txt
