 


link:
	gcc obj/main.o  -Llib/static -lhello -o bin/statically-linked

static:
	ar rcs lib/static/libhello.a obj/helloWorld.o obj/helloLuong.o

all:
	gcc -c -o obj/main.o main.c -I./inc
	gcc -c -o obj/helloWorld.o src/helloWorld.c -I ./inc
	gcc -c -o obj/helloLuong.o src/helloLuong.c -I ./inc

clean:
	rm -rf hello
