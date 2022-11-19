all:
	gcc -g -o trab2 *.c

run:
	./trab2

clean:
	rm trab2 *.o

zip:
	zip trab2.zip Makefile *.c *.o *.h