CC = g++
CFLAGS = -g -Wall -std=c++11

default: botnet

botnet: Main.o Reader.o GraphDetector.o FlowDetector.o BotDiscover.o util.o
	$(CC) $(CFLAGS) -o botnet Main.o Reader.o GraphDetector.o FlowDetector.o BotDiscover.o util.o

Main.o: Main.cpp Reader.h GraphDetector.h FlowDetector.h BotDiscover.h
	$(CC) $(CFLAGS) -c Main.cpp

Reader.o: Reader.cpp Reader.h
	$(CC) $(FLAGS) -c Reader.cpp
	
GraphDetector.o: GraphDetector.cpp GraphDetector.h util.h
	$(CC) $(FLAGS) -c GraphDetector.cpp

FlowDetector.o: FlowDetector.cpp FlowDetector.h
	$(CC) $(FLAGS) -c FlowDetector.cpp

BotDiscover.o: BotDiscover.cpp BotDiscover.h util.h
	$(CC) $(FLAGS) -c BotDiscover.cpp
	
util.o: util.cpp util.h rnGen.h
	$(CC) $(FLAGS) -c util.cpp

clean:
	$(RM) comboCPP *.o *~
