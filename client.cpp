#include "tcpClient.hpp"

#include <iostream>

#define PORTNUM 60001

using namespace std;


int main() {

    std::string serverAddress;
    std::cout << "Enter hostname: ";
    std::cin >> serverAddress;

    if(serverAddress == "localhost"){
        serverAddress = "127.0.0.1";
    }



    TCPClient client(PORTNUM, serverAddress);

    std::string input, response;

    // Initial server welcome message
    while (true) {
        response = client.receiveData();
        if (response == "EXIT") break;

        std::cout << response;

        // Only wait for user input if server prompt expects it
        if (response.find("Username:") != std::string::npos ||
            response.find("Password:") != std::string::npos ||
            response.find("Enter") != std::string::npos ||
            response.find("Press") != std::string::npos ||
            response.find("Type") != std::string::npos ||
            response.find("Invalid input") != std::string::npos ||
            response.find("Subscribed") != std::string::npos ||
            response.find("not subscribed") != std::string::npos ||
            response.find("Successfully") != std::string::npos ||
            response.find("logged out") != std::string::npos ||
            response.find("locations:") != std::string::npos
        ) {
            std::getline(std::cin >> std::ws, input);
            client.sendData(input);
        }
    }

    std::cout << "[Client] Exiting.\n";


    return 0;
}