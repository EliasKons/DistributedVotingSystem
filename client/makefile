CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
SRC_DIR = .
INCLUDE_DIR = ../include


SRCS = $(wildcard $(SRC_DIR)/*.c) ../error_handler.c
OBJS = $(SRCS:.c=.o)
DEPS = thread.h $(INCLUDE_DIR)/error_handler.h $(INCLUDE_DIR)/defines.h

TARGET = pollSwayer

ARGS = localhost 5005 inputFile

.PHONY: all clean run valgrind

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET) $(ARGS)

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET) $(ARGS)

