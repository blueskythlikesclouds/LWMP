#pragma once
#include "PacketHandler.h"
#include "Socket.h"
#include "MemoryPool.h"

class PacketSender : public PacketHandler
{
	void threadImplementation() override;
	
public:
	PacketSender(Address& address, Socket* socket);

	void send(Packet& packet);
};
