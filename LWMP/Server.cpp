#include "Server.h"
#include <cstdio>

Server::Server(uint16_t port)
{
	initialize(INADDR_ANY, port, SOCK_DGRAM, 0);
	if (bind(udpSocket, (sockaddr*)&udpSocketAddress, udpSocketAddressLength) != 0)
	{
		fprintf(stderr, "Server::Server(): bind() failed\n");
		return;
	}
}
