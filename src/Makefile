CC=g++
CFLAGS=-Wall -g -c -O2


#libraries
LIB=  -lpthread boost?

gentest: main.o individual.o population.o utility
	$(CC) $(CFLAGS) $(LIB) -o $@ main.o individual.o population.o utility.o

clean:
	rm -f gentest *.o
