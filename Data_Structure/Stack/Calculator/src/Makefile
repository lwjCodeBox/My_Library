.SUFFIXES : .c .o
CC = gcc
INC = ../../LinkedListStack/src

CFLAGS = -g -Wall -ansi -std=c99 
 all: test

OBJS = Calculator.o $(INC)/LinkedListStack.o Test_Calculator.o
SRCS = $(OBJS:.o=.c)

NEW_FILE = LinkedListStack.o

test: $(OBJS)
	$(CC) -o Calculator $(OBJS)
	mv $(INC)/LinkedListStack.o ./
	./Calculator

c: $(OBJS)
	$(CC) -o Calculator $(OBJS)
	mv $(INC)/LinkedListStack.o ./	
run:
	./Calculator
	
clean: 
	rm -rf $(OBJS) $(NEW_FILE)
allclean: 
	rm -rf $(OBJS) $(NEW_FILE) Calculator test
