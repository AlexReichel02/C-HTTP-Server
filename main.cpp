//============================================================================
// Name        : main.cpp
// Author      : Alex Reichel, Zachary Tisdale
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : Starts the Server which has index.html and testPresence.html
//============================================================================


#include "Server.hpp"
int main()
{

	Server serv = Server();
	serv.startServer();

    return 0;
}

