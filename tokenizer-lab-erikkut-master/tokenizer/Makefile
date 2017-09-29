all: test

mytoc.o:  mytoc.c
	cc -c mytoc.c 

test.o: test.c mytoc.h
	cc -c test.c

test: mytoc.o test.o
	cc -o test mytoc.o test.o

clean:
	rm -f *.o mytoc test test
