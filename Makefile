CC=g++
CFLAGS=-Wall	-g
main:	main.o	operate.o	prefix.o	colour.o	parray.o	lcve.o	wptable.o
clean:
		rm	-f	main main.o	operate.o	prefix.o	colour.o	parray.o	lcve.o	wptable.o
