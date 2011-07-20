CC=g++
CFLAGS=-Wall -g -O2
OBJ=ga_engine.o individual.o population.o config.o common/utility.o  bitmatrix/bitmatrix.o main.o

LIB=-lconfuse

gentest:  $(OBJ)
	$(CC) $(CFLAGS) $(LIB) -o ../$@ $(OBJ) 

clean:
	rm -rf ../gentest *.o *~ bitmatrix/*~ bitmatrix/*.o
