CC = g++
CFLAGS = -Wall -g -std=c++11

default: botnet

botnet: Main.o Reader.o GraphDetector.o FlowDetector.o BotDiscover.o util.o
	$(CC) $(CFLAGS) -o botnet Main.o Reader.o GraphDetector.o FlowDetector.o BotDiscover.o util.o

Main.o: Main.cpp Reader.h GraphDetector.h FlowDetector.h BotDiscover.h
	$(CC) $(CFLAGS) -c Main.cpp

Reader.o: Reader.cpp Reader.h
	$(CC) $(CFLAGS) -c Reader.cpp
	
GraphDetector.o: GraphDetector.cpp GraphDetector.h util.h
	$(CC) $(CFLAGS) -c GraphDetector.cpp

FlowDetector.o: FlowDetector.cpp FlowDetector.h
	$(CC) $(CFLAGS) -c FlowDetector.cpp

BotDiscover.o: BotDiscover.cpp BotDiscover.h util.h
	$(CC) $(CFLAGS) -c BotDiscover.cpp
	
util.o: util.cpp util.h rnGen.h
	$(CC) $(CFLAGS) -c util.cpp

clean:
	$(RM) comboCPP *.o *~
