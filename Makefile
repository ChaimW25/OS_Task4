CC=gcc
FLAG= -Wall -g

all: server client
server: server.c
	$(CC)  $(FLAG) -o server server.c -pthread

client: client.c
	$(CC)  $(FLAG) -o client client.c

.PHONY: clean all

clean:
	rm -f *.o server client