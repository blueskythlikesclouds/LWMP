#include "Address.h"
#include <WS2tcpip.h>

Address::Address() : address(0), port(0)
{
}

Address::Address(uint32_t address, uint16_t port) : address(address), port(port)
{
}

Address::Address(const char* address, uint16_t port) : port(port)
{
	inet_pton(AF_INET, address, &this->address);
}

Address Address::fromSocketAddress(sockaddr_in& socketAddress)
{
	return {socketAddress.sin_addr.S_un.S_addr, ntohs(socketAddress.sin_port)};
}

sockaddr_in Address::toSocketAddress() const
{
	sockaddr_in socketAddress{};
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_addr.S_un.S_addr = address;
	socketAddress.sin_port = htons(port);
	memset(&socketAddress.sin_zero, 0, sizeof(socketAddress.sin_zero));

	return socketAddress;
}
