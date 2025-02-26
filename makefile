all: edlin
run: edlin
	./edlin
edlin: edlin.o
	g++ -g -Wall edlin.o -o edlin
edlin.o: edlin.cpp
	g++ -g -Wall -c edlin.cpp -o edlin.o