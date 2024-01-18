#include "Client.h"

#define CLIENT_CODE1 //Pre-processor direction to enable compilation for CLIENT_CODE1

const int FAILURE = 0; //return values for exception handling
const int SUCCESS = 1;


Client::Client() //CONSTRUCTOR
{
	cout << "Client object created!";
	system("cls");
}


int Client::initaliseSocket() //Initialise a socket for server (WSAStartup()))
{
	cout << "\nAttempting to open socket for client...\n\n";
	wsaStartupResult = WSAStartup(MAKEWORD(2, 2), &winsockData);
	if (wsaStartupResult != 0)
	{
		cout << "\033[1;31mWSAStartup() has failed. Client has failed to initialise Winsock... \tError:" << WSAGetLastError() << endl;
		return FAILURE; //exception check
	}

	else
	{
		cout << "WSAStartup() has succeeded! Client has initialised Winsock..." << endl;
		cout << "Winsock status: " << winsockData.szSystemStatus << endl << endl;
		return SUCCESS; //exception check
	}
}


int Client::createSocket() //Creates a socket for the server
{
	clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //TCP connection
	if (clientSocket == INVALID_SOCKET) {
		cout << "\033[1;31msocket() has failed. Client error creating socket... \tError: " << WSAGetLastError() << endl;
		WSACleanup();
		return FAILURE; //exception check where invalid is failure
	}

	else
	{
		cout << "socket() has succeeded! Client has created a new socket..." << endl;
		return SUCCESS; //excpetion check where not invalid is success
	}
}


int Client::acceptSocket() //Connects client socket to server
{
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) //if connect() returns SOCKET_ERROR then failure
	{
		cout << "\033[1;31mconnect() [accept] has failed. Client has failed to connect to server... \tError: " << WSAGetLastError() << endl;
		WSACleanup();
		return FAILURE; //exception check
	}

	else 
	{
		cout << "connect() [accept] has succeeded! Client has connected to server..." << endl;
		return SUCCESS; //exception check
	}
}


int Client::sendMessage() //Sends message through client socket to server
{
	cout << "\n\033[0mPlease enter 'QUIT' to exit the program or type the message you wish to send to the server: ";

	do
	{
		cin.getline(messageBuffer, sizeof(messageBuffer));
		cin.clear();
	} while (std::cin.fail()); //while input is invalid


	byteAmount = send(clientSocket, messageBuffer, 200, 0); //sends message in messageBuffer through socket
	if (byteAmount > 0) //if any data (more than 0 bytes) was sent then successful
	{
		cout << "\033[1;36m\n\nMessage has been sent to server:  '\033[32m" << messageBuffer << "\033[1;36m'\n" << endl;

		if (strcmp(messageBuffer, "QUIT") == 0) //if characters in messageBuffer are QUIT then close sockets and shutdown
		{
			cout << "\033[1;31mShutting down client...\033[0m" << endl;
			closesocket(clientSocket);
			WSACleanup();
			return SUCCESS; //exception check
		}

		return SUCCESS; //exception check
	}

	else //if less than 1 byte was sent then error
	{
		cout << "\033[1;31mError sending message to server." << endl;
		closesocket(clientSocket);
		WSACleanup();
		return FAILURE; //exception check
	}
}

int Client::sendConfirmation() //Sends confirmation message through client socket to server
{
	char receivedConfirmation[200] = "Client has received server message!"; //confirmation message that is sent to client when a message is received (echo)

	byteAmount = send(clientSocket, receivedConfirmation, 200, 0);
	if (byteAmount > 0)
	{
		cout << "\033[1;36m\nConfirmation message has been sent to the server!\n" << endl;
		cout << "\033[0m\t---------------------\n";
		return SUCCESS;
	}

	else
	{
		cout << "Error sending confirmation message to the server..." << endl;
		return FAILURE;
	}
}


int Client::receiveMessage() //Receives message through a client socket
{
	byteAmount = recv(clientSocket, messageBuffer, 200, 0); //receives message in messageBuffer through socket 
	if (byteAmount > 0)
	{
		cout << "\033[1;33m\nMessage has been received by client from the server:  '\033[32m" << messageBuffer << "\033[1;36m'\033[32m " << endl;
		return SUCCESS; //exception check
	}

	else
	{
		closesocket(clientSocket);
		WSACleanup();
		return FAILURE; //exception check
	}
}


int Client::receiveConfirmation() //Receives confirmation message through a client socket
{
	byteAmount = recv(clientSocket, receivedConfirmation, 200, 0); //receives confirmation in receivedConfirmation through socket
	if (byteAmount > 0) //if any data (more than 0 bytes) was received then successful
	{
		cout << "\033[1;33m\nMessage received confirmation has been sent by server:  '\033[32m" << receivedConfirmation << "\033[1;36m'\033[0m\n\n\t-------------------" << endl;
	}

	else //if less than 1 bye was received then error
	{
		cout << "\033[1;31mError receiving confirmation from server." << endl;
		closesocket(clientSocket);
		WSACleanup();
		return FAILURE; //exception check
	}
}


int Client::closeSocket() //Closes a client socket
{
	cout << "\033[1;31mError receiving confirmation from server." << endl;
	closesocket(clientSocket);
	WSACleanup();
	return SUCCESS; //exception check
}


int main(int argc, char* argv[])
{
#ifdef CLIENT_CODE1 //Pre-processor directive checks if CLIENT_CODE1 is defined for compilation

	HWND console = GetConsoleWindow();
	SetWindowPos(console, 0, 0, 0, 899, 949, SWP_NOZORDER | SWP_NOACTIVATE); //console window size and position on screen

	try 
	{
		Client client; //creates client object in main() to call relevant functions

		cout << "\033[1;36m\n\n\t\tCLIENT\n\t---------------------\n";

		if (!client.initaliseSocket()) //exception check for whether client socket initialization has succeeded
		{
			throw runtime_error("\033[1;31mError initializing socket for client");
		}

		if (!client.createSocket()) //exception check for whether client socket has been created
		{
			throw runtime_error("\033[1;31mError creating socket for client");
		}

		if (!client.acceptSocket()) //exception check for whether client socket has connected
		{
			throw runtime_error("\033[1;31mError connecting to server");
		}

		cout << "\n\t---------------------\n";
		while (true) //loop sending and receiving data from server
		{
			if (!client.sendMessage() && strcmp(client.getBuffer(), "QUIT") != 0) //exception check for whether message has been sent and is not QUIT
			{
				throw runtime_error("\033[1;31mError sending data to server");
				break;
			}

			if (strcmp(client.getBuffer(), "QUIT") != 0 && !client.receiveConfirmation()) //exception check for whether confirmation has been sent and is not QUIT
			{
				throw runtime_error("\033[1;31mError receiving confirmation from server...");
				break;
			}

			if (strcmp(client.getBuffer(), "QUIT") == 0) //if user has entered QUIT into buffer in client then break sending & receiving loop
			{
				break;
			}

			if (strcmp(client.getBuffer(), "QUIT") != 0 && !client.receiveMessage()) //exception check to check that messageBuffer is not QUIT and that message has been received - otherwise must have disconnected
			{
				throw runtime_error("\033[1;31mConnection with server has been lost...\n");
				break;
			}

			if (!client.sendConfirmation()) //exception check for whether confirmation has been sent and is not QUIT
			{
				throw runtime_error("\033[1;31mError sending confirmation to the server...");
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
		cout << "\033[1;31m\nClient Error: " << error.what() << endl; // what() returns the throw output from the exception that was triggered
		cout << "\033[0m";
		system("pause");
		WSACleanup();
		return -1; //exception catch for try{} where -1 is error
	}

	cout << "\033[0m";
	WSACleanup();
	return 0; //main() where 0 = success

#endif //CLIENT_CODE1
}
