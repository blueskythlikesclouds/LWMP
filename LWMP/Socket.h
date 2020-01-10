#pragma once
#include <WinSock2.h>
#include <cstdint>

class Socket
{
protected:
	SOCKET udpSocket;
	sockaddr_in udpSocketAddress;
	size_t udpSocketAddressLength;

	void initialize(uint32_t address, uint16_t port, int32_t type, int32_t protocol);
	
public:
	Socket();
	virtual bool send(uint8_t* data, uint32_t length) const;
	virtual bool receive(uint8_t* data, uint32_t length) const;
	virtual uint32_t getAddress() const;
	virtual uint16_t getPort() const;
	virtual ~Socket();
};
