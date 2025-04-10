#include "tcpServer.hpp"

TCPServer::TCPServer(int portNum) { // Initialize atomic variable in the constructor
    this->portNum=portNum;
    this->serverClosed=false;
    servSocket = -1;
    startServer();
    run();
}

TCPServer::~TCPServer(){
    // close the server socket
    serverClosed.store(true); // Proper atomic write

    if (t.joinable()) {
        t.join(); // Ensure the exit thread finishes
    }

    if (servSocket != -1) {
        close(servSocket);
    }

    std::cout << "Server closed." << std::endl;
}

void TCPServer::startServer(){

    std::cout << "Server starting..." << std::endl << std::endl;

    // Create the socket
    // Reference: Dr. Mishra's tcpServer3.c file, as well as https://www.geeksforgeeks.org/how-to-read-from-a-file-in-cpp/
    servSocket = socket(AF_INET, SOCK_STREAM, 0);

    // check if server socket was made correctly
    if (servSocket == -1)
    {
        perror("Server failed to create a socket.");
        exit(1);
    }

    // Opens ports for reuse immediately after shutdown
    int opt = 1;
    setsockopt(servSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //https://www.ibm.com/docs/en/zos/2.4.0?topic=calls-setsockopt

    // define the server address
    sockaddr_in servAddress{};                  // declaring a structure for the address
    servAddress.sin_family = AF_INET;         // Structure Fields' definition: Sets the address family of the address the client would connect to
    servAddress.sin_port = htons(portNum);    // Passing the port number - converting in right network byte order
    servAddress.sin_addr.s_addr = INADDR_ANY; // Connecting to 0.0.0.0

    // binding the socket to the IP address and port
    // Params: which socket, cast for server address, its size
    if (bind(servSocket, (struct sockaddr *)&servAddress, sizeof(servAddress)) == -1)
    {
        perror("Bind failed.");
        close(servSocket);
        exit(1);
    }else{
        std::cout << "Server address: " << servSocket << std:: endl;
    }

    // listen for simultaneous connections
    // which socket, how many connections
    if (listen(servSocket, 5) == -1) // takes up to 5 queued connecections
    {
        perror("Listen failed.");
        close(servSocket);
        exit(1);
    }
}

void TCPServer::run(){
    std::cout << "Server listening on port " << portNum << "..." << std::endl;
    while (!serverClosed)
    {
        int clientSocket = accept(servSocket, nullptr, nullptr); // server socket to interact with client, structure like before - if you know - else nullptr for local connection

        if (clientSocket < 0)
        {
            if (serverClosed) {
                std::cout << "Server closed. No longer accepting connections." << std::endl;
                break;  // Break the loop only if the server has been closed
            }
            perror("Error: Failed to accept client.");
            continue;  // Continue the loop if server hasn't been closed
        }

        // Spawn a new thread to handle the client
        std::thread clientThread(&TCPServer::handleClient, this, clientSocket);
        clientThread.detach(); // Detach the thread to allow independent execution
    }
}



void TCPServer::exitServer(){
    const std::string exitCommand = "exit";
    std::string cliInput = "";

    while(!serverClosed){
        if(!std::cin)
        {
            serverClosed = true;
            return;
        }
        
        std::getline(std::cin, cliInput);

        if(!cliInput.empty() && cliInput != "")
        {
            std::transform(cliInput.begin(), cliInput.end(), cliInput.begin(), ::tolower);

            if(cliInput == exitCommand)
            {
                serverClosed = true;
            }
        }
    }
}

void TCPServer::handleClient(int clientSocket){
    std::cout << "Client connected" << std::endl;

    ClientHandler clientHandler(clientSocket);

        clientHandler.run();

        

    close(clientSocket);
    std::cout << "Client connection closed." << std::endl;
}

