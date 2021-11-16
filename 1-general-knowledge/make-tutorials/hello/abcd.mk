.PHONY :  rule1 all clean khanh 

CC := gcc
CFLAGS := -I.
INC_FILES := hello.h

%.o: %.c $(INC_FILES)
	$(CC) -o -c $@ $<

#hello.o: hello.c 
#	gcc -o -c hello.o hello.c

#main.o: main.c
#	gcc -o -c main.o main.c

khanh:
	@echo "hi im khanh"

rule1: hello.o main.o
	$(CC) -o hello hello.o main.o $(CFLAGS)


all:
	$(CC) -o hello hello.c main.c $(CFLAGS)
clean:
	rm -rf hello	
