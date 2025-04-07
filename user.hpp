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
        int clientSocket;
        std::string username;
        std::string uppercaseUsername;
        std::string password;
        std::deque<std::string> subscribedLocations;

    public:
        User(int socket, const std::string& uname, const std::string& pwd) : clientSocket(socket), username(uname), password(pwd) {
            uppercaseUsername = uname;
            std::transform(uppercaseUsername.begin(), uppercaseUsername.end(), uppercaseUsername.begin(), ::toupper);
        }

        // Getters/Setters
        int getClientSocket() const { return clientSocket; }
        std::string getUsername() const { return username; }
        std::string getUppercaseUsername() const { return uppercaseUsername; }
        std::string getPassword() const { return password; }
        std::deque<std::string> getSubscribedLocations() const { return subscribedLocations; }
        void setClientSocket(int socket) { clientSocket = socket; }
        void setPassword(const std::string& newPassword) { password = newPassword; }

        // Subscription methods
        void subscribeTo(const std::string&);
        void unsubscribeFrom(const std::string&);
        bool isSubscribed(const std::string&) const ;
        string listSubscribedLocations() const;

};


#endif