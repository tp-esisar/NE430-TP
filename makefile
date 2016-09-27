

multibit: main.c multibit.c
	gcc -g -Wall multibit.c -o multibit

run: multibit
	./multibit routes