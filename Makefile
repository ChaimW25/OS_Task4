CC=gcc
FLAG= -Wall -g
OBJECTS_LOOPS=  advancedClassificationLoop.o basicClassification.o
OBJECTS_REC= advancedClassificationRecursion.o basicClassification.o

all: server client
server: server.o
	$(CC)  $(FLAG) -o server server.o -pthread

client: client.o
	$(CC)  $(FLAG) -o client client.o


server.o: server.c
	$(CC)  $(FLAG) -c server.c
client.o: client.c
	$(CC)  $(FLAG) -c client.c
.PHONY: clean all

clean:
	rm -f *.o server client