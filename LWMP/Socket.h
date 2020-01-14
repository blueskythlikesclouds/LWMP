#pragma once
#include <WinSock2.h>
#include <cstdint>
#include "Address.h"

class Socket
{
protected:
	SOCKET udpSocket;

public:
	Socket(int32_t type, int32_t protocol);
	virtual bool send(Address& address, uint8_t* data, size_t length) const;
	virtual bool receive(Address& address, uint8_t* data, size_t length, size_t* receivedLength = NULL) const;
	virtual ~Socket();
};
