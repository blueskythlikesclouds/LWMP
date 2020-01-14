#include "Server.h"
#include <cstdio>

Server::Server(uint16_t port) : Socket(SOCK_DGRAM, 0), address((uint32_t)INADDR_ANY, port)
{
	sockaddr_in socketAddress = address.toSocketAddress();
	if (bind(udpSocket, (sockaddr*)&socketAddress, sizeof(socketAddress)) == 0)
		return;

	fprintf(stderr, "Server::Server(): bind() failed\n");
}

bool Server::receive(Address& address, uint8_t* data, size_t length, size_t* receivedLength) const
{
	sockaddr_in socketAddress = address.toSocketAddress();
	size_t socketAddressLength = sizeof(socketAddress);

	size_t result = recvfrom(udpSocket, (char*)data, length, 0, (sockaddr*)&socketAddress, (int*)&socketAddressLength);
	if (result == SOCKET_ERROR)
		return false;

	if (receivedLength)
		*receivedLength = result;

	address = Address::fromSocketAddress(socketAddress);
	return true;
}

Address Server::getAddress() const
{
	return address;
}
