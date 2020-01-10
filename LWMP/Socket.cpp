#include "Socket.h"
#include <cstdio>

void Socket::initialize(uint32_t address, uint16_t port, int32_t type, int32_t protocol)
{
	udpSocket = socket(AF_INET, type, protocol);
	if (udpSocket == INVALID_SOCKET)
	{
		fprintf(stderr, "Socket::initialize(): Socket creation failed\n");
		return;
	}
	
	udpSocketAddressLength = sizeof(sockaddr_in);
	ZeroMemory(&udpSocketAddress, udpSocketAddressLength);
	udpSocketAddress.sin_family = AF_INET;
	udpSocketAddress.sin_port = htons(port);
	udpSocketAddress.sin_addr.S_un.S_addr = address;
}

Socket::Socket() : udpSocket(INVALID_SOCKET), udpSocketAddress(), udpSocketAddressLength(sizeof(sockaddr_in))
{
}

bool Socket::send(uint8_t* data, uint32_t length) const
{
	return sendto(udpSocket, (const char*)data, length, 0, (const sockaddr*)&udpSocketAddress, udpSocketAddressLength) != SOCKET_ERROR;
}

bool Socket::receive(uint8_t* data, uint32_t length) const
{
	return recvfrom(udpSocket, (char*)data, length, 0, (sockaddr*)&udpSocketAddress, (int*)&udpSocketAddressLength) != SOCKET_ERROR;
}

uint32_t Socket::getAddress() const
{
	return udpSocketAddress.sin_addr.S_un.S_addr;
}

uint16_t Socket::getPort() const
{
	return ntohs(udpSocketAddress.sin_port);
}

Socket::~Socket()
{
	closesocket(udpSocket);
}
