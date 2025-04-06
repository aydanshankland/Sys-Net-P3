#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <cstring>
#include <sys/socket.h> //API and definitions for the sockets
#include <netinet/in.h> //Structures to store address information
#include <unistd.h>     // For read, close
#include <thread> //https://en.cppreference.com/w/cpp/thread/thread
#include <atomic>
#include <algorithm> // For transforming input to lowercase
#include <cctype>

#include "clientHandler.hpp"

#define BUFFER_SIZE 1024

using namespace std;

class TCPServer {
    private:
    int servSocket;
    int portNum;
    std::atomic<bool> serverClosed; // https://en.cppreference.com/w/cpp/atomic/atomic
    std::thread t;  // Thread to monitor exit command
    
    public:
    TCPServer(int portNum);
    ~TCPServer();

    void startServer();

    void run();

    void receiveMessage();
    void sendMessage();

    
    void exitServer();
    void handleClient(int clientSocket);
    


};


#endif