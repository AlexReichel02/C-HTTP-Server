/*
 * Server.cpp
 *
 *  Created on: Jan 18, 2023
 *      Author: alexreichel, Zachary Tisdale
 */

#include "Server.hpp"

Server::Server() {
	// TODO Auto-generated constructor stub
	 addressLength = sizeof(address);

	 address.sin_family = AF_INET;
	 address.sin_addr.s_addr = INADDR_ANY;
     address.sin_port = htons( PORT );

	 memset(address.sin_zero, '\0', sizeof address.sin_zero);
}

Server::~Server() {
	// TODO Auto-generated destructor stub
}

char* Server::parse(char line[], const char symbol[])
{
	//char *message;
	char * token = strtok(line, symbol);
	//int current = 0;

	token = strtok(NULL, " ");  // added /
	return token;
	/*
	 * This does not look to be needed since it will always return the second input.
	while (token != NULL) {

		token = strtok(NULL, " ");
		if (current == 0) {
			message = token;
			return message;
		}
		current = current + 1;
	}
	return message;*/
}

void Server::startServer(){
	// TODO: edit *hello
	// Not needed
	// char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	const char* errorMessage = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 20\n\n404 Error, Not Found";

	// Creating socket file descriptor
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
	    perror("Error in socket() method");
	    exit(EXIT_FAILURE);
	}

	if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address))<0) {
	    perror("Erro in bind() method");
	    exit(EXIT_FAILURE);
	}

	if (listen(serverSocket, 10) < 0) {
	    perror("Error in listen() method");
	    exit(EXIT_FAILURE);
	}

	while(true) {
		// "\n+++++++ Waiting for new connection ++++++++\n\n"
	    printf("=====Waiting for new connection=====\n");
	    if ((newClientSocket = accept(serverSocket, (struct sockaddr *)&address, (socklen_t*)&addressLength))<0) {
	        perror("Error in accept() method");
	        exit(EXIT_FAILURE);
	    }

	    char clientBuffer[30000] = {0};
	    readFromClient = read( newClientSocket , clientBuffer, 30000);
	    printf("%s\n", clientBuffer);
	    char *parse_string = parse(clientBuffer, " ");  //Try to get the path which the client ask for
	    printf("Client ask for path: %s\n", parse_string);

		/*
	    if(string(parse_string).size()==1){
	        write(newClientSocket , hello, strlen(hello));
	    }*/

	    int errorCode = 404;
	    string content = "<h1>404 Not Found</h1>";


		// =============== appending . infront of /file.html ? ===============
		string fileName;
		if (string(parse_string) == "/") {
			fileName = "./index.html";
		}
		else {
			fileName = string(".") + string(parse_string);
		}
	    
	    // Open the document in the local file system
	    ifstream readFile(fileName);
		
	    // Check if it opened and if it did, grab the entire contents
	    if (readFile.good()) {
	        //cout<<"Test"<<endl;
	        string str((istreambuf_iterator<char>(readFile)), istreambuf_iterator<char>());
	        content = str;
	        errorCode = 200;

			// ostringstream oss
	        ostringstream responseToClient;
	        responseToClient << "HTTP/1.1 " << errorCode << " OK\r\n";
	        responseToClient << "Cache-Control: no-cache, private\r\n";
	        responseToClient << "Content-Type: text/html\r\n";
	        responseToClient << "Content-Length: " << content.size() << "\r\n";
	        responseToClient << "\r\n";
	        responseToClient << content;

	        string output = responseToClient.str();
	        int size = output.size() + 1;

	        write(newClientSocket, output.c_str(), size);

	    }
		else{
	        write(newClientSocket , errorMessage, strlen(errorMessage));
	    }
	    readFile.close();
	    //printf("------------------message sent to client-------------------");
	    close(newClientSocket);
	}
}
