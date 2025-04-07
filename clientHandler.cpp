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

    while(isLoggedIn){
        string menu = "1 Subscribe to a location\n2 Unsubscribe from a location\n3 Send a message to a location\n4 Send a private message\n5 See all the locations you are subscribed to\n6 See all the online users\n7 See last 10 messages\n8 change password\nType 'exit' to Quit\n";
        send(clientSocket, menu.c_str(), menu.size(), 0);
        string clientMsg = recieveMessage();

        if(clientMsg == "1") {
            // this->login();
        } else if( clientMsg == "2") {
            // this->registerUser();
        } else if( clientMsg == "3") {
            // this->registerUser();
        } else if( clientMsg == "4") {
        } else if( clientMsg == "5") {
        } else if( clientMsg == "6") {
        } else if( clientMsg == "7") {
        } else if( clientMsg == "8") {
        } else {
            string errorMessage = "Invalid input\n";
            send(clientSocket, errorMessage.c_str(), errorMessage.size(), 0);
        }
    }
    
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
    string loginMsg = "";
    if(validateUser(username, password)){
        isLoggedIn = true;
        currentUser = new User(clientSocket, username, password); // update user object
        loginMsg = "You're logged in!\n";
        send(clientSocket, loginMsg.c_str(), loginMsg.size(), 0);
    }else{
        isLoggedIn = true;
        loginMsg = "Login failed.\n";
        send(clientSocket, loginMsg.c_str(), loginMsg.size(), 0);
    }

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
   
    std::string normalizedUsername = username;
    std::transform(normalizedUsername.begin(), normalizedUsername.end(), normalizedUsername.begin(), ::toupper);
    registerNewUserToFile(username, normalizedUsername, password);
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
    while (getline(fileIn, line)) {
        if (line.find(normalizedUsername) != std::string::npos) {
            // std::cout << "Found '" << normalizedUsername << "' at line " << lineNumber << ": " << line << std::endl;
            return true;
        }
    }

    fileIn.close();
    return false;
}

bool ClientHandler::registerNewUserToFile(std::string username, std::string normalizedUsername, std::string password){
    ofstream fileOut("users.txt", std::ios::app);

    if(!fileOut.is_open()){
        cerr << "Error: Unable to open DB file.";
        return false;
    }

    fileOut << username << " " << normalizedUsername << " " << password << endl;
    fileOut.close();
    return true;
}

bool ClientHandler::validateUser(std::string username, std::string password){
    ifstream fileIn("users.txt");

    if(!fileIn.is_open()){
        cerr << "Error: Unable to read DB file when verifying username and password.";
        return false;
    }

    std::string normalizedUsername = username;
    std::transform(normalizedUsername.begin(), normalizedUsername.end(), normalizedUsername.begin(), ::toupper);

    std::string line;
    while (getline(fileIn, line)) {
        istringstream iss(line);
        string fileUsername, fileUsernameUpper, filePassword;

        if (iss >> fileUsername >> fileUsernameUpper >> filePassword) {
            if (fileUsername == username && filePassword == password) {
                return true;
            }
        }
    }

    fileIn.close();
    return false;
}