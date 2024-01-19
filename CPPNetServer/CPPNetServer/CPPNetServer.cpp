#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>

#include <iostream>

#include "WSAErrorCodeMap.h"
#include "ErrorCodeHandler.h"

int main(int argc, char* argv[])
{
	SOCKET serverSocket, acceptSocket;
	int port = 55555;
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);

	ErrorCodeHandler errorCodeHandler;


	if (wsaerr != 0)
	{
		errorCodeHandler.PrintWinSockError(WSAGetLastError(), "The Winsick dll not found!");
		return 0;
	}
	else
	{
		std::cout << "The Winsock dll found!" << std::endl;
		std::cout << "The Status: " << wsaData.szSystemStatus << std::endl;
	}

	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		errorCodeHandler.PrintWinSockError(WSAGetLastError(), "Error at Socket(): ");
		WSACleanup();
		return 0;
	}
	else
	{
		std::cout << "socket() is OK! " << std::endl;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
	service.sin_port = htons(port);
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		errorCodeHandler.PrintWinSockError(WSAGetLastError(), "bind() failed: ");
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else
	{
		std::cout << "bind() is OK!\n";
	}

	// Listen on that socket
	if (listen(serverSocket, 1) == SOCKET_ERROR)
	{
		errorCodeHandler.PrintWinSockError(WSAGetLastError(), "listen(): Error listening on socket");
	}
	else
	{
		std::cout << "listen() is OK! Waiting for connections...\n";
	}

	// Accept 
	acceptSocket = accept(serverSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET)
	{
		errorCodeHandler.PrintWinSockError(WSAGetLastError(), "accept failed: ");
		WSACleanup();
		return -1;
	}
	else
	{
		std::cout << "Accepted connection\n";
	}

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

	WSACleanup();
}