all: multiplier

multiplier: multiplier.o
	g++ -pthread -o multiplier multiplier.o

multiplier.o: multiplier.cpp
	g++ -c -o multiplier.o multiplier.cpp
	
