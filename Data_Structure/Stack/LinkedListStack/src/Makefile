.SUFFIXES : .c .o
CC = gcc
CFLAGS = -g -Wall -ansi -std=c99
 all: test

OBJS = LinkedListStack.o Test_LinkedListStack.o
SRCS = $(OBJS:.o=.c)

test: $(OBJS)
	$(CC) -o LinkedListStack $(OBJS) 
	./LinkedListStack
	
c: $(OBJS)
	$(CC) -o LinkedListStack $(OBJS)
	
run:
	./LinkedListStack

clean: 
	rm -rf $(OBJS)
allclean: 
	rm -rf $(OBJS) LinkedListStack test
