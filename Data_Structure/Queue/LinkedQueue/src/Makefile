.SUFFIXES : .c .o
CC = gcc
CFLAGS = -g -Wall -ansi -std=c99
 all: test

OBJS = LinkedQueue.o Test_LinkedQueue.o
SRCS = $(OBJS:.o=.c)

test: $(OBJS)
	$(CC) -o LinkedQueue $(OBJS) 
	./LinkedListStack

c: $(OBJS)
        $(CC) -o LinkedListStack $(OBJS)

run:
        ./LinkedListStack

clean: 
	rm -rf $(OBJS)
allclean:
	rm -rf $(OBJS) LinkedQueue test
