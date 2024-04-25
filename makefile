assignment1: a01.c
	gcc a01.c -g -o as01 -Wall -pedantic -ansi
run: 
	./as01
valgrind:
	valgrind ./as01
clean: 
	rm -f as01


