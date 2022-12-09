.SUFFIXES : .c .o
CC = gcc
CFLAGS = -g -Wall -ansi -std=c99
 all: test

OBJS = Graph.o Test_Graph.o
SRCS = $(OBJS:.o=.c)

test: $(OBJS)
	$(CC) -o Graph_exe $(OBJS) 
clean: 	
	rm -rf $(OBJS) Graph_exe
distclean: 
	rm -rf $(OBJS) Graph test
