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

    string input;

    bool connected = client.receiveData();

    while (connected) {
        std::getline(std::cin >> std::ws, input);  // flush leading newline
        client.sendData(input);
        connected = client.receiveData();
    }

    std::cout << "[Client] Exiting.\n";


    return 0;
}