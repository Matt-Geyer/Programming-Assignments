all: prodcon

prodcon: prodcon.o
	g++ -pthread -o prodcon prodcon.o

prodcon.o: prodcon.cpp
	g++ -c -o prodcon.o prodcon.cpp
	
