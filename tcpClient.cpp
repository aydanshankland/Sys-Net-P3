#include "tcpClient.hpp"

TCPClient::TCPClient(int portNum, string serverAddress){
    this->portNum=portNum;
    this->serverAddress = serverAddress;
    this->connection();
}

TCPClient::~TCPClient() {
    close(clientSocket);
}


void TCPClient::connection(){
// Create the socket
    // Referenced: Dr. Mishra's tcpClient3.c file, as well as https://www.geeksforgeeks.org/socket-programming-in-cpp/
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // check if server socket was made correctly
    if (clientSocket == -1)
    {
        perror("Error: Client failed to create a socket.");
        //continue;
    }

    // define the server address
    sockaddr_in servAddress{};             // declaring a structure for the address
    servAddress.sin_family = AF_INET;      // Structure Fields' definition: Sets the address family of the address the client would connect to
    servAddress.sin_port = htons(portNum); // Passing the port number - converting in right network byte order

    // If a client is sending a message, then the serverAddress will still be blank and this IP needs to be added.
    if(serverAddress == "localhost")
    {
        serverAddress = "127.0.0.1";
    }

    // Convert the user-inputted IP address from string to binary
    if (inet_pton(AF_INET, serverAddress.c_str(), &servAddress.sin_addr) <= 0)
    {
        std::cout << "ERROR: Invalid address/Address not supported.\n\n";
        close(clientSocket);
        //continue; // Skip and allow the user to enter another request
    }

    // Needs a timeout, otherwise the client will hang indefinitely when a valid yet unfindable IP is entered. 
    // ref1: https://man.freebsd.org/cgi/man.cgi?query=setsockopt&sektion=2
    // ref2: https://stackoverflow.com/questions/4181784/how-to-set-socket-timeout-in-c-when-making-multiple-connections
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    // Sets the send timeout
    if(setsockopt(clientSocket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)))
    {
        perror("ERROR: Send timeout failed.");
    }

    // Sets the receive timeout
    if(setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)))
    {
        perror("ERROR: Receive timeout failed.");
    }

    // connecting the socket to the IP address and port
    // Params: which socket, cast for server address, its size
    if (connect(clientSocket, (struct sockaddr *)&servAddress, sizeof(servAddress)) == -1)
    {
        perror("ERROR: Server connection failed.");
        std::cout << "Error: No response from server at " << serverAddress << "\n\n"; 
        close(clientSocket);
        // continue;
    }

}

void TCPClient::sendData(string input){
    ssize_t bytesSent = send(clientSocket, input.c_str(), input.length(), 0);

    if (bytesSent == -1) {
        perror("ERROR: Failed to send data.");
    }
}

void TCPClient::receiveData(){
    char buffer[BUFFER_SIZE] = {0};
    memset(buffer, 0, BUFFER_SIZE); // Clear the buffer before receiving

    // Receive data from the client
    ssize_t bytesRecvd = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    
    if (bytesRecvd <= 0) {
        std::cout << "Client disconnected." << std::endl;
    } else {
        printReceivedData(buffer, bytesRecvd);
    }

    buffer[bytesRecvd] = '\0'; // Null-terminate received message
}

void TCPClient::printReceivedData(const char* buffer, ssize_t bytesRecvd){ 
    std::cout << buffer << std::endl;
}