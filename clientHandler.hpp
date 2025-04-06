#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <fstream>
#include <algorithm>

#define BUFFER_SIZE 1024

class ClientHandler {
    public:
    ClientHandler(int clientSocket);
    void run();

    private:
    char buffer[BUFFER_SIZE] = {0};
    std::string recieveMessage();
    int clientSocket;
    bool isLoggedIn = false;
    void welcome();
    void login();
    void registerUser();
    bool isUsernameUsed(std::string);

    void sendOptions();

};

#endif