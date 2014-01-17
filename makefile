CC=mpic++.mpich2

all: miller_rabin

DEPENDENCIES=miller_rabin.o utils.o master.o slave.o mpi_tiny.o 
miller_rabin: src/main.cpp $(DEPENDENCIES)
	$(CC) src/main.cpp $(DEPENDENCIES) -o miller_rabin
	
test: src/test.cpp $(DEPENDENCIES)
	$(CC) src/test.cpp $(DEPENDENCIES) -o test
	
miller_rabin.o: src/miller_rabin.cpp
	$(CC) src/miller_rabin.cpp -c -o miller_rabin.o

utils.o: src/utils.cpp
	$(CC) src/utils.cpp -c -o utils.o

master.o: src/master.cpp
	$(CC) src/master.cpp -c -o master.o

slave.o: src/slave.cpp
	$(CC) src/slave.cpp -c -o slave.o

mpi_tiny.o: src/mpi_tiny.cpp
	$(CC) src/mpi_tiny.cpp -c -o mpi_tiny.o

clean:
	-rm -f miller_rabin
	-rm -f test
	-rm -f *.o
