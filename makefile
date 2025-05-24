all:
	nasm -g -f elf64 setsp.s
	nasm -g -f elf64 mutex.s
	gcc setsp.o mutex.o ust.c test.c -O0 -o ust
	rm *.o

