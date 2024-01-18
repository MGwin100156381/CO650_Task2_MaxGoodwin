#pragma once

#include "Comms.h"

using namespace std;


class Server : public Comms //INHERITANCE - Comms abstract class
{

public:
	Server(); //CONSTRUCTOR

	int initaliseSocket() override; //server implementation of socket initialization
	int createSocket() override; //server implementation of socket creation
	int acceptSocket() override; //server implementation of accepting a socket
	int receiveMessage() override; //server implementation of receiving message
	int receiveConfirmation() override; //server implementation of receiving confirmation
	int sendMessage() override; //server inplementation of sending message
	int sendConfirmation() override; //server implementation of sending confirmation
	int closeSocket() override; //server implementation of closing a socket

	int bindSocket(); //binding a address and port to a socket - client does not need
	int listenOnSocket(); //listening for a response on a socket - client does not need

	const char* getBuffer() const //returns the current characters stored in the buffer
	{
		return messageBuffer;
	}
};
