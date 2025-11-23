CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11 -g
SRCDIR = src
TESTDIR = tests
BUILDDIR = build
SRCS = $(filter-out $(SRCDIR)/client.c, $(wildcard $(SRCDIR)/*.c))
TESTS = $(wildcard $(TESTDIR)/*.c)
TARGET = $(BUILDDIR)/connect4
CLIENT = $(BUILDDIR)/connect4_client
LDFLAGS = 

.PHONY: all debug release clean run test valgrind gdb-run multithread client

all: CFLAGS += -O2
all: $(TARGET) $(CLIENT)

debug: CFLAGS += -g -O0 -DDEBUG
debug: clean $(TARGET) $(CLIENT)

# Multithreaded build for the bot (adds -pthread)
# Build with `make multithread` to enable pthreads and MT_BUILD compile flag.
multithread: CFLAGS += -O2 -DMT_BUILD -pthread
multithread: clean $(TARGET) $(CLIENT)

$(TARGET): $(SRCS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

$(CLIENT): $(SRCDIR)/client.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -o $(CLIENT) $(SRCDIR)/client.c

client: $(CLIENT)

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	@chmod +x $(TESTDIR)/test_runner.sh
	$(TESTDIR)/test_runner.sh

test-hard:
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) $(TESTDIR)/test_bot_hard.c $(SRCDIR)/bot_hard.c $(SRCDIR)/game.c -o $(BUILDDIR)/test_bot_hard -lpthread
	./$(BUILDDIR)/test_bot_hard

valgrind: debug $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log $(TARGET) || true

gdb-run: debug $(TARGET)
	gdb -batch -ex "run" -ex "bt" -ex "info locals" --args $(TARGET) | tee gdb_trace.txt

clean:
	rm -rf $(BUILDDIR) valgrind.log gdb_trace.txt
