EXEC := kjell

CFLAGS := -Wall -Wextra -ansi -pedantic

SRCS = $(wildcard src/*.c)
HDRS = $(wildcard src/*.h)

OBJS = ${SRCS:.c=.o}

$(EXEC): $(OBJS) $(HDRS) makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

all: $(EXEC)

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all run clean