all: myShell

mytoc.o: mytoc.c
	cc -c mytoc.c 

myShell.o: myShell.c mytoc.h
	cc -c myShell.c

myShell: mytoc.o myShell.o
	cc -o myShell mytoc.o myShell.o

clean:
	rm -f *.o mytoc myShell myShell
