#ifndef CLIENTHANDLER_HPP
#define CLIENTHANDLER_HPP

#include "user.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <fstream>
#include <sstream>
#include <algorithm>

#define BUFFER_SIZE 1024

class ClientHandler {
    public:
    ClientHandler(int clientSocket);
    void run();

    private:
    char buffer[BUFFER_SIZE] = {0};
    User* currentUser = nullptr;
    std::string recieveMessage();
    int clientSocket;
    bool isLoggedIn = false;
    void welcomeAnonymous();
    void welcomeAuthUser();
    void login();
    void registerUser();
    bool isUsernameUsed(std::string);
    bool registerNewUserToFile(std::string username, std::string normalizedUsername, std::string password);
    bool validateUser(std::string username, std::string password);
    void changePassword();
    void subscribeToLocation();
    void unsubscribeFromLocation();
    void viewSubscriptions();
    void logout();

    void sendOptions();

};

#endif