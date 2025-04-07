#include "clientHandler.hpp"

using namespace std;

ClientHandler::ClientHandler(int clientSocket){
    this->clientSocket = clientSocket;
}

void ClientHandler::run() {
    while (true) {
        // Unauthenticated menu
        while (!isLoggedIn){
            this->welcomeAnonymous();
            std::string clientMsg = recieveMessage();

            if (clientMsg == "1") {
                this->login();
            } else if (clientMsg == "2") {
                this->registerUser();
            } else if (clientMsg == "exit") {
                return;    // end server session
            } else {
                std::string errorMessage = "Invalid input\n";
                send(clientSocket, errorMessage.c_str(), errorMessage.size(), 0);
            }
        } 

        // Authenticated menu
        while (isLoggedIn) {
            this->welcomeAuthUser();                     
            std::string clientMsg = recieveMessage();

            if (clientMsg == "1") {
                subscribeToLocation();
            } else if (clientMsg == "2") {
                unsubscribeFromLocation();
            } else if (clientMsg == "3") {
                viewSubscriptions();
            } else if (clientMsg == "4") {
                changePassword();
            } else if (clientMsg == "5") {
                logout();  // return to unauthenticated
            } else if (clientMsg == "exit") {
                logout();  // cleanup
                return;    // end server session
            } else {
                std::string errorMessage = "Invalid input\n";
                send(clientSocket, errorMessage.c_str(), errorMessage.size(), 0);
            }
        }
    }
}


string ClientHandler::recieveMessage() {
    memset(buffer, 0, BUFFER_SIZE);

    // Receive data
    ssize_t bytesRecvd = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

    if (bytesRecvd <= 0) {
        std::cout << "Client disconnected." << std::endl;
        return "";
    }

    buffer[bytesRecvd] = '\0';
    string clientMsg(buffer);

    // Remove leading/trailing whitespace
    clientMsg.erase(0, clientMsg.find_first_not_of(" \n\r\t"));
    clientMsg.erase(clientMsg.find_last_not_of(" \n\r\t") + 1);

    return clientMsg;
}

void ClientHandler::welcomeAnonymous(){
    string welcomeMessage = "Welcome\nPress 1 to login\nPress 2 to register\n";
    send(clientSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0);
}

void ClientHandler::welcomeAuthUser(){
    string menu =
                "1 Subscribe to a location\n"
                "2 Unsubscribe from a location\n"
                "3 See all the locations you are subscribed to\n"
                "4 change password\n"
                "5 Logout\n"
                "Type 'exit' to Quit client session\n";
    send(clientSocket, menu.c_str(), menu.size(), 0);
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
        isLoggedIn = false;
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

    std::string normalizedUsername = username;
    std::transform(normalizedUsername.begin(), normalizedUsername.end(), normalizedUsername.begin(), ::toupper);
    registerNewUserToFile(username, normalizedUsername, password);

    string successMessage = "Successfully registered.\nReturning to main menu...\n\n";
    send(clientSocket, successMessage.c_str(), successMessage.size(), 0);
    
    return;
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

void ClientHandler::changePassword(){
    std::string prompt = "Enter new password:\n";
    send(clientSocket, prompt.c_str(), prompt.size(), 0);
    std::string newPass = recieveMessage();

    currentUser->setPassword(newPass);

    // Update users.txt
    std::ifstream inFile("users.txt");
    std::ofstream outFile("users_tmp.txt");
    std::string line;

    while (getline(inFile, line)) {
        std::istringstream iss(line);
        std::string uname, unameUpper, pwd;
        if (iss >> uname >> unameUpper >> pwd) {
            if (uname == currentUser->getUsername()) {
                outFile << uname << " " << unameUpper << " " << newPass << "\n";
            } else {
                outFile << line << "\n";
            }
        }
    }

    inFile.close();
    outFile.close();
    remove("users.txt");
    rename("users_tmp.txt", "users.txt");

    std::string msg = "\nPassword changed successfully.\n";
    send(clientSocket, msg.c_str(), msg.size(), 0);
}

void ClientHandler::subscribeToLocation(){
    std::string prompt = "Enter location to subscribe:\n";
    send(clientSocket, prompt.c_str(), prompt.size(), 0);
    std::string location = recieveMessage();

    if (currentUser->isSubscribed(location)) {
        std::string msg = "\nYou are already subscribed to " + location + ".\n\n";
        send(clientSocket, msg.c_str(), msg.size(), 0);
    } else {
        currentUser->subscribeTo(location);
        std::string msg = "\nSubscribed to " + location + ".\n\n";
        send(clientSocket, msg.c_str(), msg.size(), 0);
    }
}

void ClientHandler::unsubscribeFromLocation(){
    std::string prompt = "Enter location to unsubscribe:\n";
    send(clientSocket, prompt.c_str(), prompt.size(), 0);
    std::string location = recieveMessage();

    if (!currentUser->isSubscribed(location)) {
        std::string msg = "\nYou are not subscribed to " + location + ".\n\n";
        send(clientSocket, msg.c_str(), msg.size(), 0);
    } else {
        currentUser->unsubscribeFrom(location);
        std::string msg = "\nUnsubscribed from " + location + ".\n\n";
        send(clientSocket, msg.c_str(), msg.size(), 0);
    }
}

void ClientHandler::viewSubscriptions(){
    std::string result = currentUser->listSubscribedLocations();
    send(clientSocket, result.c_str(), result.size(), 0);
}

void ClientHandler::logout(){
    isLoggedIn = false;
    delete currentUser;
    currentUser = nullptr;

    string logoutMsg = "\nYou've logged out.\n\n";
    send(clientSocket, logoutMsg.c_str(), logoutMsg.size(), 0);
}