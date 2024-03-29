#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>

#include <iostream>

#include "WSAErrorCodeMap.h"
#include "ErrorCodeHandler.h"

void InitialiseServer(SOCKET&, ErrorCodeHandler&);
void BindServerSocket(const SOCKET& serverSocket, int port, ErrorCodeHandler& errorCodeHandler);
void AcceptConnection(SOCKET& serverSocket, SOCKET& acceptSocket, ErrorCodeHandler&);
void HandleClientCommunication(SOCKET&);
void ListenOnSocket(SOCKET& serverSocket, ErrorCodeHandler errorCodeHandler);
void HandleClientCommunication(SOCKET& acceptSocket);

int main(int argc, char* argv[])
{
	SOCKET serverSocket, acceptSocket;
	serverSocket = INVALID_SOCKET;
	ErrorCodeHandler errorCodeHandler;

	try
	{
		InitialiseServer(serverSocket, errorCodeHandler);
		BindServerSocket(serverSocket, 55555, errorCodeHandler);
		ListenOnSocket(serverSocket, errorCodeHandler);
		AcceptConnection(serverSocket, acceptSocket, errorCodeHandler);
		HandleClientCommunication(acceptSocket);
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 0;
	}

	WSACleanup();
}


void InitialiseServer(SOCKET& serverSocket, ErrorCodeHandler& errorCodeHandler) 
{
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);

	if (wsaerr != 0)
	{
		errorCodeHandler.HandleErrorAndCleanup(serverSocket, "Failed to initialise Winsock: The Winsick dll not found!");
	}

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		errorCodeHandler.HandleErrorAndCleanup(serverSocket, "Error at Socket(): ");
	}
	else
	{
		std::cout << "socket() is OK! " << std::endl;
	}
}

void BindServerSocket(const SOCKET& serverSocket, int port, ErrorCodeHandler& errorCodeHandler)
{
	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
	service.sin_port = htons(port);
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		errorCodeHandler.HandleErrorAndCleanup(serverSocket, "Failed to bind socket: bind() failed. ");
	}
	else
	{
		std::cout << "bind() is OK!\n";
	}
}

void AcceptConnection(SOCKET& serverSocket, SOCKET& acceptSocket, ErrorCodeHandler& errorCodeHandler)
{
	// Accept 
	acceptSocket = accept(serverSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET)
	{
		errorCodeHandler.HandleErrorAndCleanup(serverSocket, "accept failed: ");
	}
	else
	{
		std::cout << "Accepted connection\n";
	}
}

void ListenOnSocket(SOCKET& serverSocket, ErrorCodeHandler errorCodeHandler)
{
	// Listen on that socket
	if (listen(serverSocket, 1) == SOCKET_ERROR)
	{
		errorCodeHandler.HandleErrorAndCleanup(serverSocket, "listen(): Error listening on socket");
	}
	else
	{
		std::cout << "listen() is OK! Waiting for connections...\n";
	}
}

void HandleClientCommunication(SOCKET& acceptSocket)
{
	bool Quit{ false };

	const int bufferSize{ 200 };
	char buffer[bufferSize];
	memset(buffer, 0, sizeof(buffer));

	char confirmation[200] = "Message Received\n";

	int recvByteCount = recv(acceptSocket, buffer, bufferSize, 0);
	if (recvByteCount == 0)
		// Close the client socket
		closesocket(acceptSocket);

	if (recvByteCount > 0)
	{

		buffer[recvByteCount] = '\0'; // Null terminate to prevent string-related functions from reading beyond the buffer
		std::cout << "Message Received: " << buffer << std::endl;
	}
	else
	{
		// TO-DO: Handle Disconnect.
		WSACleanup();
	}

	int sentByteCount = send(acceptSocket, confirmation, strlen(confirmation), 0);

	if (sentByteCount > 0)
	{
		std::cout << "Message Sent!\n";
	}

	std::cout << "Press enter to continue...";
	std::cin.get();
}