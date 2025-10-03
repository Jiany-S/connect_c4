CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -g
SRCDIR = src
BINDIR = bin
SRCS = $(wildcard $(SRCDIR)/*.c)
TARGET = $(BINDIR)/connect4

.PHONY: all debug release clean run test valgrind gdb-run asan

# default: optimized build
all: CFLAGS += -O2
all: $(TARGET)

debug: CFLAGS += -g -O0 -DDEBUG
debug: clean $(TARGET)

release: CFLAGS += -O2
release: clean $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	@echo "Running $(TARGET)"
	./$(TARGET)

# Non-interactive test target (use TEST_INPUT to choose file)
TEST_INPUT ?= tests/test1.in
test: $(TARGET)
	@mkdir -p tests/results
	./$(TARGET) < $(TEST_INPUT) | tee tests/results/$(notdir $(TEST_INPUT)).out || true

# Valgrind run; writes to valgrind.log
VAL_INPUT ?= tests/test1.in
valgrind: debug $(TARGET)
	@echo "Running valgrind with input $(VAL_INPUT)"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./$(TARGET) < $(VAL_INPUT) || true
	@echo "Valgrind log: valgrind.log"

# GDB batch run that saves backtrace to gdb_trace.txt
GDB_INPUT ?= tests/test1.in
gdb-run: debug $(TARGET)
	@echo "Running gdb (batch) with $(GDB_INPUT) -> gdb_trace.txt"
	gdb -batch -ex "set pagination 0" -ex "run < $(GDB_INPUT)" -ex "bt" -ex "info locals" --args $(TARGET) 2>&1 | tee gdb_trace.txt || true

# optional asan build (address sanitizer)
asan: CFLAGS += -fsanitize=address -g -O0
asan: clean $(TARGET)

clean:
	rm -rf $(BINDIR) tests/results valgrind.log gdb_trace.txt
