CC=gcc
FLAG= -Wall -g


all: server client
server: server.o
	$(CC)  $(FLAG) -o server server.o -pthread

client: client.o
	$(CC)  $(FLAG) -o client client.o


server.o: server.cpp
	$(CC)  $(FLAG) -c server.cpp
client.o: client.cpp
	$(CC)  $(FLAG) -c client.cpp
.PHONY: clean all

clean:
	rm -f *.o server client