

multibit: main.c multibit.c
	gcc -g -Wall main.c -o multibit

run-%: % multibit
	./multibit $<

test: test.c multibit.c
	gcc -g -Wall test.c -o test

test-%: % test
	./test $< testfile

bench: bench.c multibit.c
	gcc -O3 bench.c -o bench

