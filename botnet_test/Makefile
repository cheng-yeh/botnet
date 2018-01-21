CC = g++
CFLAGS = -g -Wall

default: comboCPP

comboCPP: Main.o Reader.o GraphDetector.o FlowDetector.o util.o
	$(CC) $(CFLAGS) -o comboCPP Main.o Reader.o GraphDetector.o FlowDetector.o util.o

Main.o: Main.cpp Reader.h GraphDetector.h FlowDetector.h
	$(CC) $(CFLAGS) -c Main.cpp

Reader.o: Reader.cpp Reader.h
	$(CC) $(FLAGS) -c Reader.cpp
	
GraphDetector.o: GraphDetector.cpp GraphDetector.h util.h
	$(CC) $(FLAGS) -c GraphDetector.cpp

FlowDetector.o: FlowDetector.cpp FlowDetector.h
	$(CC) $(FLAGS) -c FlowDetector.cpp
	
util.o: util.cpp util.h rnGen.h
	$(CC) $(FLAGS) -c util.cpp

clean:
	$(RM) comboCPP *.o *~
