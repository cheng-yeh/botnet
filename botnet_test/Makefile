CC = g++
CFLAGS = -g -Wall

default: comboCPP

comboCPP: Main.o Graph.o
	$(CC) $(CFLAGS) -o comboCPP Main.o Graph.o

Main.o: Main.cpp Graph.h
	$(CC) $(CFLAGS) -c Main.cpp

Graph.o: Graph.cpp Graph.h util.h
	$(CC) $(FLAGS) -c Graph.cpp

clean:
	$(RM) comboCPP *.o *~


