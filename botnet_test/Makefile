CC      = g++
CFLAGS  = -Wall -g -std=c++11
OBJECTS = Main.o Reader.o GraphDetector.o Writer.o BotDiscover.o Combo.o Graph.o util.o
EXEC    = botnet

default : $(EXEC)

$(EXEC) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJECTS)

Main.o : Main.cpp Reader.h GraphDetector.h Writer.h BotDiscover.h
	$(CC) $(CFLAGS) -c Main.cpp

Reader.o : Reader.cpp Reader.h
	$(CC) $(CFLAGS) -c Reader.cpp
	
GraphDetector.o : GraphDetector.cpp GraphDetector.h util.h
	$(CC) $(CFLAGS) -c GraphDetector.cpp

Writer.o : Writer.cpp Writer.h
	$(CC) $(CFLAGS) -c Writer.cpp

BotDiscover.o : BotDiscover.cpp BotDiscover.h util.h
	$(CC) $(CFLAGS) -c BotDiscover.cpp

Combo.o : Combo.cpp Combo.h Graph.h
	$(CC) $(CFLAGS) -c Combo.cpp

Graph.o : Graph.cpp Graph.h
	$(CC) $(CFLAGS) -c Graph.cpp
	
util.o : util.cpp util.h rnGen.h
	$(CC) $(CFLAGS) -c util.cpp

clean :
	$(RM) $(EXEC) *.o *~
