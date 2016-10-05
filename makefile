

multibit: main.c multibit.c
	gcc -g -Wall -std=c99 main.c -o multibit

run-%: % multibit
	./multibit $<

test: test.c MyAlgo.c
	gcc -O3 -std=c99 test.c -o test

test-%: % test
	./test $< testfile

bench: bench.c multibit.c
	gcc -O3 -std=c99 bench.c -o bench

