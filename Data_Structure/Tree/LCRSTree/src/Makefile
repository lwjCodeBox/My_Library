.SUFFIXES : .c .o
CC = gcc
CFLAGS = -g -Wall -ansi -std=c99
 all: test

OBJS = LCRSTree.o Test_LCRSTree.o
SRCS = $(OBJS:.o=.c)

test: $(OBJS)
	$(CC) -o LCRSTree $(OBJS) 
	./LCRSTree
c:$(OBJS)
	$(CC) -o LCRSTree $(OBJS)

run:
	./LCRSTree

clean: 
	rm -rf $(OBJS)
allclean: 
	rm -rf $(OBJS) LCRSTree test
