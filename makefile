all:
	nasm -g -f elf64 setsp.s
	gcc setsp.o ust.c -o ust
	rm *.o

