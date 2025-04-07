#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm> // For transforming input to lowercase
#include <deque>


#define BUFFER_SIZE 1024

using namespace std;

class User {
    private:
        int socketNo;
        // int portNum;
        std::string username;
        std::string usernameNormalized;
        std::string password;
        std::string location;
        std::string message;
        deque<std::string> locations;
        deque<std::string> messages;

    public:

    


};


#endif