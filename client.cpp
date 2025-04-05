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

    client.receiveData();

    do{
        cin >> input;

        client.sendData(input);

        client.receiveData();

    }while(input != "exit");


    return 0;
}