#pragma once

#include "Comms.h"

class Client : public Comms //INHERITANCE - Comms abstract class
{

public:
	Client(); // CONSTRUCTOR

	int initaliseSocket() override; //client implementation of socket initialization
	int createSocket() override; //client implementation of socket creation
	int acceptSocket() override; //client implementation of accepting a socket
	int sendMessage() override; //client implementation of sending message
	int sendConfirmation() override; //client implementation of receiving confirmation
	int receiveMessage() override; //client implementation of receiving message
	int receiveConfirmation() override; //client implementation of receiving confirmation
	int closeSocket() override; //client implementation of closing a socket
	 
	const char* getBuffer() const //returns the current characters stored in the buffer
	{
		return messageBuffer;
	}
};
