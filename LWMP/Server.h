#pragma once
#include "Socket.h"

class Server : public Socket
{
public:
	Server(uint16_t port);
};
