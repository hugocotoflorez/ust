all:
	nasm -g -f elf64 sp.s
	nasm -g -f elf64 mutex.s
	gcc sp.o mutex.o ust.c test.c -O0 -ggdb -o ust
	rm *.o

