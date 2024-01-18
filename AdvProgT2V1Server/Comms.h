#pragma once

#include <string> //strings

#include <iostream> //input/outputs
#include <iomanip>

#include <winsock2.h> //WINOWS API & TCP
#include <ws2tcpip.h>
#include <tchar.h>

using namespace std;

class Comms 
{

protected:
    //Client members/variables
    SOCKET clientSocket;
    
    int port = 55555;


    //Server members/variables
    SOCKET serverSocket, acceptedSocket;

    SOCKADDR_STORAGE clientAddress;

    int clientInfoCheck, clientAddressLength;

    char serverString[NI_MAXSERV], clientString[NI_MAXHOST];


    //Client and Server shared members/variables
    WSADATA winsockData;
    WORD requestedWinsockVersion;

    int wsaStartupResult;
    int byteAmount = 0;

    char messageBuffer[200];
    char receivedConfirmation[200];

    virtual int initaliseSocket() = 0; //(Initialization) WSAStartup() for a socket
    virtual int createSocket() = 0; //creates a new socket
    virtual int acceptSocket() = 0; //accept a socket connection
    virtual int sendMessage() = 0; //send typed message through socket
    virtual int sendConfirmation() = 0; //send confirmation through socket
    virtual int receiveMessage() = 0; //receive typed message through socket
    virtual int receiveConfirmation() = 0; //receive confirmation through socket
    virtual int closeSocket() = 0; //close a socket

};
