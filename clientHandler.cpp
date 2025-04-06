#include "clientHandler.hpp"

using namespace std;

ClientHandler::ClientHandler(int clientSocket){
    this->clientSocket = clientSocket;
}

void ClientHandler::run() {
    do{
        this->welcome();
        string clientMsg = recieveMessage();

        if(clientMsg == "1") {
            this->login();
        } else if( clientMsg == "2") {
            this->registerUser();
        } else {
            string errorMessage = "Invalid input\n";
            send(clientSocket, errorMessage.c_str(), errorMessage.size(), 0);
        }
    }while(!isLoggedIn);
    
}

string ClientHandler::recieveMessage() {
    memset(buffer, 0, BUFFER_SIZE);

    // Receive data
    ssize_t bytesRecvd = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

    if (bytesRecvd <= 0) {
        std::cout << "Client disconnected." << std::endl;
    }

    buffer[bytesRecvd] = '\0';
    string clientMsg(buffer);

    return clientMsg;
}

void ClientHandler::welcome(){
    string welcomeMessage = "Welcome\nPress 1 to login\nPress 2 to register\n";
    send(clientSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
}

void ClientHandler::login(){
    string usernameRequest = "Username:\n";
    send(clientSocket, usernameRequest.c_str(), usernameRequest.size(), 0);
    
    string username = recieveMessage();
    
    string passwordRequest = "Password:\n";
    send(clientSocket, passwordRequest.c_str(), passwordRequest.size(), 0);
    
    string password = recieveMessage();

    // NEED TO ADD LOGIC TO CHECK USERNAME AND PASSWORD
    // if login is correct, change isLoggedIn to true
    // if it is wrong display an message telling htem and let the run() funcntion hanlde the rest
    

}

void ClientHandler::registerUser() {
    string usernameRequest = "Username:\n";
    send(clientSocket, usernameRequest.c_str(), usernameRequest.size(), 0);
    
    string username = recieveMessage();

    while(isUsernameUsed(username)){
        string usernameErrorMsg = "Username: " + username + " already used.\nEnter a new Username:\n";
        send(clientSocket, usernameErrorMsg.c_str(), usernameErrorMsg.size(), 0);
    
        username = recieveMessage();
    }
    
    string passwordRequest = "Password:\n";
    send(clientSocket, passwordRequest.c_str(), passwordRequest.size(), 0);
    
    string password = recieveMessage();

    string successMessage = "Succesfully registered\n";
    send(clientSocket, successMessage.c_str(), successMessage.size(), 0);
   
    
}

bool ClientHandler::isUsernameUsed(std::string username){
    ifstream fileIn("users.txt");

    if(!fileIn.is_open()){
        cerr << "Error: Unable to read DB file when verifying username availability.";
        return false;
    }

    std::string normalizedUsername = username;
    std::transform(normalizedUsername.begin(), normalizedUsername.end(), normalizedUsername.begin(), ::toupper);

    std::string line;
    int lineNumber = 1;
    while (getline(fileIn, line)) {
        if (line.find(normalizedUsername) != std::string::npos) {
            std::cout << "Found '" << normalizedUsername << "' at line " << lineNumber << ": " << line << std::endl;
            return true;
        }
        lineNumber++;
    }

    fileIn.close();
    return false;
}