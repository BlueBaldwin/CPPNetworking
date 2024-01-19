#include <iostream>
#include <winsock2.h>

#include "ErrorCodeHandler.h"



ErrorCodeHandler::ErrorCodeHandler() {};

void ErrorCodeHandler::PrintWinSockError(int errorCode, const std::string& contextMessage)
{
	const auto& errorCodeMap = WSAErrorCodeMap::GetInstance().GetErrorCodeMap();
	auto it = errorCodeMap.find(errorCode);

	if (it != errorCodeMap.end())
	{
		std::cout << contextMessage << ": " << it->second << std::endl;
	}
	else
	{
		std::cout << contextMessage << ": " << PrintUnknownError(errorCode) << std::endl;
	}
}

void ErrorCodeHandler::HandleErrorAndCleanup(const SOCKET& socket, const std::string& errorMessage)
{
	PrintWinSockError(WSAGetLastError(), errorMessage);
	if (socket != INVALID_SOCKET)
	{
		closesocket(socket);
	}
	WSACleanup();
	throw std::runtime_error(errorMessage);
}

std::string ErrorCodeHandler::PrintUnknownError(int errorCode)
{
	return "Unknown Error Code: " + std::to_string(errorCode);
}
