#include "WinSocket.h"
#include <WinSock2.h>

bool WinSocket::startup()
{
	WSADATA wsaData;
	return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

void WinSocket::cleanup()
{
	WSACleanup();
}
