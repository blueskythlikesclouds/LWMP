#include "Socket.h"

Socket::Socket(int32_t type, int32_t protocol) : udpSocket(socket(AF_INET, type, protocol))
{
}

bool Socket::send(Address& address, uint8_t* data, uint32_t length) const
{
	sockaddr_in socketAddress = address.toSocketAddress();
	return sendto(udpSocket, (const char*)data, length, 0, (const sockaddr*)&socketAddress, sizeof(socketAddress)) != SOCKET_ERROR;
}

bool Socket::receive(Address& address, uint8_t* data, size_t length, size_t* receivedLength) const
{
	sockaddr_in socketAddress = address.toSocketAddress();
	size_t socketAddressLength = sizeof(socketAddress);
	
	size_t result = recvfrom(udpSocket, (char*)data, length, 0, (sockaddr*)&socketAddress, (int*)&socketAddressLength);
	if (result == SOCKET_ERROR)
		return false;

	if (receivedLength)
		*receivedLength = result;

	return true;
}

Socket::~Socket()
{
	closesocket(udpSocket);
}
