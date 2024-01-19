#include <iostream>
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

std::string ErrorCodeHandler::PrintUnknownError(int errorCode)
{
	return "Unknown Error Code: " + std::to_string(errorCode);
}
