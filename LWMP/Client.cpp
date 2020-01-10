#include "Client.h"
#include <WS2tcpip.h>

Client::Client(uint32_t address, uint16_t port)
{
	initialize(address, port, SOCK_DGRAM, IPPROTO_UDP);
}

Client::Client(const char* address, uint16_t port)
{
	uint32_t addr;
	inet_pton(AF_INET, address, &addr);
	initialize(addr, port, SOCK_DGRAM, IPPROTO_UDP);
}
