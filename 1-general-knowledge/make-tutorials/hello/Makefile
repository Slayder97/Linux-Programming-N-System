
.PHONY: hello hellomake

CC := gcc
CFLAGS := -I.

INC_FILES := hello.h


%.o: %.c $(INC_FILES)
	$(CC) -c -o $@ $<


hellomake: main.o hello.o
	$(CC) -o $@ main.o hello.o $(CFLAGS)


hello: 
	gcc -o hello main.c hello.c -I.


clean: 
	rm -rf hello hellomake *.o
