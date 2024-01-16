CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread
SRC_DIR = .
ADTS_DIR = ADTS
INCLUDE_DIR = include

SRCS = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(ADTS_DIR)/*.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(wildcard $(INCLUDE_DIR)/*.h)

TARGET = poller

ARGS = 5005 10 5 log.txt stat.txt

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
