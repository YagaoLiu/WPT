CC=g++
CFLAGS=-g
main:	main.o	operate.o	prefix.o	colour.o	parray.o	lcve.o	wptable.o
clean:
		rm	-f	main.o	operate.o	prefix.o	colour.o	parray.o	lcve.o	wptable.o
