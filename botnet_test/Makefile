CC = g++
CFLAGS = -Wall -g -std=c++14

default: botnet

botnet: Main.o Reader.o GraphDetector.o Writer.o BotDiscover.o util.o
	$(CC) $(CFLAGS) -o botnet Main.o Reader.o GraphDetector.o Writer.o BotDiscover.o util.o

Main.o: Main.cpp Reader.h GraphDetector.h Writer.h BotDiscover.h
	$(CC) $(CFLAGS) -c Main.cpp

Reader.o: Reader.cpp Reader.h
	$(CC) $(CFLAGS) -c Reader.cpp
	
GraphDetector.o: GraphDetector.cpp GraphDetector.h util.h
	$(CC) $(CFLAGS) -c GraphDetector.cpp

Writer.o: Writer.cpp Writer.h
	$(CC) $(CFLAGS) -c Writer.cpp

BotDiscover.o: BotDiscover.cpp BotDiscover.h util.h
	$(CC) $(CFLAGS) -c BotDiscover.cpp
	
util.o: util.cpp util.h rnGen.h
	$(CC) $(CFLAGS) -c util.cpp

clean:
	$(RM) comboCPP *.o *~
