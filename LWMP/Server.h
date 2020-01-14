#pragma once
#include "Socket.h"

class Server : public Socket
{
	Address address;
	
public:
	Server(uint16_t port);
	
	bool receive(Address& address, uint8_t* data, size_t length, size_t* receivedLength) const override;
	
	Address getAddress() const;
};
