#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>

#include <iostream>

void InitialiseClient(SOCKET& clientSocket, ErrorCodeHandler errorCodeHandler);


int main(int argc, char* argv[])
{
	SOCKET clientSocket;
	int port = 55555;
	

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		std::cerr << "Client: connect() Failed to connect: " << WSAGetLastError();
		WSACleanup();
		return 0;
	}
	else
	{
		std::cout << "Client: connect() is OK!\n";
		std::cout << "Client: Can start sending and recieving data...\n";
	}
	
	// Sending Messages

	const int bufferSize{ 200 };
	char buffer[bufferSize];
	bool exitChat{ false };

	while (!exitChat)
	{
		std::cout << "Enter Message to send to the server :\n";
		std::cin.getline(buffer, bufferSize);
		int sentByteCount = send(clientSocket, buffer, strlen(buffer), 0);

		if (sentByteCount > 0)
		{
			std::cout << "Message Sent! " << buffer << std::endl;
		}
		else
		{
			// TO-DO: Handle disconnect
			WSACleanup();
			break;
		}

		int recvByteCount = recv(clientSocket, buffer, bufferSize, 0);

		if (recvByteCount > 0) 
		{
			buffer[recvByteCount] = '\0';
			std::cout << "Messaged Received: " << buffer << std::endl;
		}
		else
		{
			WSACleanup();
		}
	}
	

	// Close and disconect the socket
	system("pause");
	WSACleanup();
	return 0;
}

