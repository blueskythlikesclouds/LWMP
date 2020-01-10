#pragma once
#include "Socket.h"

class Client : public Socket
{
public:
	Client(uint32_t address, uint16_t port);
	Client(const char* address, uint16_t port);
};
