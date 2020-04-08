hamhouse.out: hamhouse.o
	gcc -o hamhouse.out hamhouse.o -lm

hamhouse.o: hamhouse.c
	gcc -c -o hamhouse.o hamhouse.c -lm
