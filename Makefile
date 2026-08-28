CC = cc
CFLAGS = -Wall -Wextra -Iinclude -O2
SRCS = src/main.c src/ls.c src/display.c src/util.c
OBJS = $(SRCS:.c=.o)
TARGET = ls

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean