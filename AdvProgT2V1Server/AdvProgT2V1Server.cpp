#include "Server.h"

#define SERVER_CODE1 //Pre-processor directive to enable compilation for SERVER_CODE1

const int FAILURE = 0; //return values for exception handling
const int SUCCESS = 1;


Server::Server() //CONSTRUCTOR
{
	cout << "Server object created";
	system("cls");
}


int Server::initaliseSocket() //Initialise a socket for server (WSAStartup()))
{
	cout << "Attempting to open socket for server...\n\n";
	requestedWinsockVersion = MAKEWORD(2, 2);
	wsaStartupResult = WSAStartup(requestedWinsockVersion, &winsockData); //WSAStartup() for initialisation - 0 = success
	if (wsaStartupResult != 0) //if not 0 then failed
	{
		cout << "\033[1;31mWSAStartup() has failed. Server has failed to initalise Winsock... \tError: " << WSAGetLastError() << endl;
		return FAILURE;
	}

	else //else must have succeeded
	{
		cout << "WSAStartup() has succeeded! Server has initialised Winsock..." << endl;
		cout << "Winsock status: " << winsockData.szSystemStatus << endl << endl;
		return SUCCESS;
	}
}


int Server::createSocket() //Creates a socket for the server
{
	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //TCP connection
	if (serverSocket == INVALID_SOCKET)
	{
		cout << "\033[1;31msocket() has failed. Server error creating socket... \tError: " << WSAGetLastError() << endl;
		WSACleanup();
		return FAILURE;
	}

	else
	{
		cout << "socket() has succeeded! Server has created a new socket..." << endl;
		return SUCCESS;
	}
}


int Server::bindSocket() //Binds a server socket to an address and port
{
	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
	service.sin_port = htons(port);
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		cout << "\033[1;31mbind() has failed. Server error binding to address and port... \tError: " << WSAGetLastError() << endl;
		closesocket(serverSocket);
		WSACleanup();
		return FAILURE;
	}

	else
	{
		cout << "bind() has succeeded! Server has bound to address and port..." << endl;
		return SUCCESS;
	}
}


int Server::listenOnSocket() //Listens for a response on server socket (TCP)
{
	if (listen(serverSocket, 1) == SOCKET_ERROR)
	{
		cout << "\033[1;31mlisten() has failed. Server error listening for connection..." << WSAGetLastError() << endl;
		return FAILURE;
	}

	else
	{
		cout << "listen() has succeeded! Server is listening for incoming connections..." << endl;
		return SUCCESS;
	}
}


int Server::acceptSocket() //Accepts a connection to a server socket
{
	clientAddressLength = sizeof(clientAddress);
	acceptedSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &clientAddressLength); //TCP connection

	if (acceptedSocket == INVALID_SOCKET)
	{
		cout << "\033[1;31maccept() has failed. Server has failed to accept a new socket connection... \tError: " << WSAGetLastError() << endl;
		WSACleanup();
		return FAILURE;
	}

	else
	{
		cout << "accept() has succeeded! Server has accepted a new socket connection..." << endl;
		return SUCCESS;
	}
}


int Server::receiveMessage() //Receives message through a server socket
{
	byteAmount = recv(acceptedSocket, messageBuffer, 200, 0);
	if (byteAmount > 0)
	{
		cout << "\033[1;36mMessage has been received by server from the client:  '\033[32m" << messageBuffer << "\033[1;36m'\033[1;33m\n" << endl;

		if (strcmp(messageBuffer, "QUIT") == 0)
		{
			cout << "\033[1;31mServer has received a shutdown request from the client. Shutting down server..." << endl;
			closesocket(acceptedSocket);
			closesocket(serverSocket);
			WSACleanup();
			return SUCCESS;
		}
	}

	else
	{
		closesocket(acceptedSocket);
		WSACleanup();
		return FAILURE;
	}

	clientInfoCheck = getnameinfo((SOCKADDR*)&clientAddress, clientAddressLength, clientString, NI_MAXHOST, serverString, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
	if (clientInfoCheck != 0) //if getnameinfo failed (0 = success)
	{
		cout << "\033[1;31mgetnameinfo() has failed. Server cannot retrieve connection information..." << clientInfoCheck << endl;
		WSACleanup();
		return FAILURE;
	}
	cout << "Accepted from address " << clientString << " and port " << serverString << endl;
	return SUCCESS;
}


int Server::receiveConfirmation() //Receives confirmation through a server socket
{
	byteAmount = recv(acceptedSocket, receivedConfirmation, 200, 0); //receives confirmation in receivedConfirmation through socket
	if (byteAmount > 0) //if any data (more than 0 bytes) was received then successful
	{
		cout << "\033[1;36m\nMessage received confirmation has been sent by client:  '\033[32m" << receivedConfirmation << "\033[1;36m'\033[0m\n\n\t-------------------\n" << endl;
		return SUCCESS;
	}

	else //if less than 1 bye was received then error
	{
		cout << "\033[1;31mError receiving confirmation from server." << endl;
		closesocket(acceptedSocket);
		WSACleanup();
		return FAILURE; //exception check
	}
}


int Server::sendMessage() //Sends message through a server socket
{
	do
	{
		cout << "\n\033[0mPlease type the message you wish to send to the client: ";
		cin.getline(messageBuffer, sizeof(messageBuffer));
		cin.clear();
	} while (cin.fail());

	byteAmount = send(acceptedSocket, messageBuffer, 200, 0);
	if (byteAmount > 0)
	{
		cout << "\033[1;33m\nMessage has been sent to the client: '\033[32m" << messageBuffer << "\033[1;33m'" << endl;;
		return SUCCESS;
	}

	else
	{
		cout << "\033[1;31mError sending message to the client." << endl;
		return FAILURE;
	}
}

int Server::sendConfirmation() //Receives confirmation through a server socket
{
	char receivedConfirmation[200] = "Server has received client message!"; //confirmation message that is sent to client when a message is received (echo)

	byteAmount = send(acceptedSocket, receivedConfirmation, 200, 0);
	if (byteAmount > 0)
	{
		cout << "\nConfirmation message has been sent to the client!" << endl;
		cout << "\033[0m\n\t-------------------\n\033[1;33m";
		return SUCCESS;
	}

	else
	{
		cout << "\033[1;31mError sending confirmation to the client." << endl;
		return FAILURE;
	}
}


int Server::closeSocket() //Closes a server socket
{
	closesocket(serverSocket);
	WSACleanup();
	return SUCCESS;
}


int main(int argc, char* argv[])
{
#ifdef SERVER_CODE1 //Pre-processor directive checks if SERVER_CODE1 is defined for compilation

	HWND console = GetConsoleWindow();
	SetWindowPos(console, 0, 885, 0, 899, 949, SWP_NOZORDER | SWP_NOACTIVATE); //console window size and position on screen

	try
	{
		Server server; //creates server object in main() to call relevant functions

		cout << "\033[1;33m\n\n\t\tSERVER\n\t---------------------\n\n";
		if (!server.initaliseSocket()) //exception check for whether server socket initialization has succeeded
		{
			throw runtime_error("\033[1;31mError initializing socket for server...");
		}

		if (!server.createSocket()) //exception check for whether server socket has been created
		{
			throw runtime_error("\033[1;31mError creating socket for server...");
		}

		if (!server.bindSocket()) //exception check for whether server socket has binded to address and port
		{
			throw runtime_error("\033[1;31mError binding server socket...");
		}

		if (!server.listenOnSocket()) //exception check for whether server is listening on socket
		{
			throw runtime_error("\033[1;31mError listening on socket...");
		}

		if (!server.acceptSocket()) //exception check for whether server socket has connected
		{
			throw runtime_error("\033[1;31mError accepting client socket...");
		}

		cout << "\n\t---------------------\n\n";
		while (true) //loop sending and receiving data from client
		{
			if (!server.receiveMessage() && strcmp(server.getBuffer(), "QUIT") != 0) //exception check for whether message has been received and is not QUIT
			{
				throw runtime_error("\033[1;31mConnection with client has been lost...\n");
				break;
			}

			if (strcmp(server.getBuffer(), "QUIT") == 0) //if user has entered QUIT into messageBuffer in client then close socket and break sending & receiving loop
			{
				server.closeSocket();
				break;
			}

			if (!server.sendConfirmation()) //exception check for whether confirmation has been sent back to client
			{
				throw runtime_error("\033[1;31mError sending confirmation message to client...");
				break;
			}

			if (!server.sendMessage()) //exception check for whether message has been sent
			{
				throw runtime_error("\033[1;31mError sending data to client...");
				break;
			}

			if (!server.receiveConfirmation()) //exception check for whether confirmation has been received 
			{
				throw runtime_error("\033[1;31mError receiving confirmation message from client...\n");
				break;
			}
		}

		cout << "\033[0m";
		system("pause");
		WSACleanup();
		return 0; //main() where 0 = success
	}

	catch (const exception& error) //for try{} and works in tandem with throw exceptions 
	{
		cout << "\033[1;31mServer Error: " << error.what() << endl; // what() returns the throw output from the exception that was triggered
		cout << "\033[0m";
		system("pause");
		WSACleanup();
		return -1; //exception catch for try{} where -1 is error
	}

	cout << "\033[0m";
	WSACleanup();
	return 0; //main() where 0 = success

#endif //SERVER_CODE1
}
