#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <cstring>
#include <sys/socket.h> //API and definitions for the sockets
#include <netinet/in.h> //Structures to store address information
#include <unistd.h>     // For read, close
#include <arpa/inet.h>   // For inet_pton()

#define BUFFER_SIZE 1024


using namespace std;

class TCPClient{
    public:
    int portNum;
    string serverAddress;
    int clientSocket;

    TCPClient(int portNum, string serverAddress);

    ~TCPClient();

    void connection();

    void sendData(string input);

    void receiveData();

    void printReceivedData(const char* buffer, ssize_t bytesRecvd);

};


#endif