.PHONY: stage1 stage2 stage3 stage4 all clean

stage1:
	gcc -E hello.c -o hello.i

stage2:
	gcc -S hello.i -o hello.S

stage3:
	gcc -c hello.S -o hello.o

stage4:
	gcc -o hello hello.o


all:
	gcc -o hello hello.c
clean:
	rm -rf hello hello.i hello.S hello.o
