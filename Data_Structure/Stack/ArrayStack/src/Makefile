.SUFFIXES : .c .o
CC = gcc
CFLAGS = -g -Wall -ansi -std=c99
 all: test

OBJS = ArrayStack.o Test_ArrayStack.o
SRCS = $(OBJS:.o=.c)

test: $(OBJS)
	$(CC) -o ArrayStack $(OBJS) 
	./ArrayStack

c: $(OBJS)
	$(CC) -o ArrayStack $(OBJS)

run:
	./ArrayStack

clean: 
	rm -rf $(OBJS)

allclean: 
	rm -rf $(OBJS) ArrayStack test
