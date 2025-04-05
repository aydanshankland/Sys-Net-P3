CFLAGS = -g -Wall -pthread
LDFLAGS = -pthread
CC = g++

all: server client

server: server.o clientHandler.o tcpServer.o
	$(CC) $(CFLAGS) -o server server.o clientHandler.o tcpServer.o $(LDFLAGS)

client: client.o tcpClient.o
	$(CC) $(CFLAGS) -o client client.o tcpClient.o $(LDFLAGS)

clientHandler.o: clientHandler.cpp clientHandler.hpp
	$(CC) $(CFLAGS) -c clientHandler.cpp

tcpServer.o: tcpServer.cpp tcpServer.hpp
	$(CC) $(CFLAGS) -c tcpServer.cpp

server.o: server.cpp clientHandler.hpp tcpServer.hpp
	$(CC) $(CFLAGS) -c server.cpp

tcpClient.o: tcpClient.cpp tcpClient.hpp
	$(CC) $(CFLAGS) -c tcpClient.cpp

client.o: client.cpp tcpClient.hpp
	$(CC) $(CFLAGS) -c client.cpp

.PHONY: clean run_server run_client

clean:
	rm -f server client tcpServer.o tcpClient.o client.o server.o

# Start the server separately
run_server: server
	./server

# Start the client separately
run_client: client
	./client
