cim.o: Makefile cim.h cim.c
	$(CC) -Wall -Werror -O2 -I. -c cim.c -o cim.o

clean:
	rm -f cim.o *.core vgcore.*
