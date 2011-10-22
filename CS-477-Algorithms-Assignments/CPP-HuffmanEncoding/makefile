all: charFreq huff

charFreq: charFreq.o
	g++ -o charFreq charFreq.o

charFreq.o: charFreq.cpp
	g++ -c -o charFreq.o charFreq.cpp

huff: huff.o
	g++ -o huff huff.o 

huff.o: huff.cpp
	g++ -c -o huff.o huff.cpp
clean: 
	rm -rf *.o charFreq huff

