CC=g++
CFLAGS=-Wall
main:	main.o	operate.o	prefix.o	colour.o	parray.o
clean:
		rm	-f	main.o	operate.o	prefix.o	colour.o	parray.o
