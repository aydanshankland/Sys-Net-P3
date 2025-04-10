# Systems and networks 
Systems and networks 2, project 3

## Overview 
This program creates a basic TCP server and client in C++. The server and client must be started manually using these commans: ./server ./client . The server listens for incoming client connetions on port 60001. It handles and managers multiple users, each with the ability to login. User information is stored in 'users.txt'. When a client first connects to the server they will be prompted with a 2 possible decisions, login or register. Once they client makes a choice the server will then respond with the appropriate message guiding the user. Once they are logged in they will be given 5 options: 1. subscribe to a location 2. Unsubscribe to a location 3. See all locations they are subscribed to 4. change password 5. logout. Based on the clients response the server will generate the appropriate response and send it to the client. While the client is connected, they can input "exit" to terminate their program and disconnect from the server at any time.

## Contributors
Aydan Shankland and Kevin Albee

## How to Run the Program
1. Compile the program using the 'make' command

2. Start the server using './server'

3. Start the client using './client'

Once the client is started you will be prompted with the next steps via the terminal.

## Clean up
To remove the compiled files, run: 'make clean'


