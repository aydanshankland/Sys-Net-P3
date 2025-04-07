#include "tcpClient.hpp"
#include <iostream>

#define PORTNUM 60001

using namespace std;

bool expectsInput(const string& message) {
    return message.find("Username:") != string::npos ||
           message.find("Password:") != string::npos ||
           message.find("Enter") != string::npos ||
           message.find("Press") != string::npos ||
           message.find("Type") != string::npos ||
           message.find("Invalid input") != string::npos ||
           message.find("Successfully") != string::npos ||
           message.find("logged out") != string::npos ||
           message.find("Subscribed") != string::npos ||
           message.find("not subscribed") != string::npos ||
           message.find("locations:") != string::npos;
}

int main() {
    std::string serverAddress;
    std::cout << "Enter hostname: ";
    std::cin >> serverAddress;

    if(serverAddress == "localhost") {
        serverAddress = "127.0.0.1";
    }

    TCPClient client(PORTNUM, serverAddress);

    std::string input;
    std::string response;

    while (true) {
        // Keep receiving until something expects a user response
        do {
            response = client.receiveData();
            if (response == "EXIT") {
                std::cout << "Client Exiting.\n";
                return 0;
            }

            std::cout << response;
        } while (!expectsInput(response));

        std::getline(std::cin >> std::ws, input);
        client.sendData(input);
    }

    return 0;
}
