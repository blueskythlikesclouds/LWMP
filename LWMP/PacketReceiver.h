#pragma once
#include "PacketHandler.h"
#include "MemoryPool.h"
#include "Socket.h"

class PacketReceiver : public PacketHandler
{
	MemoryPool* pool;
	
	void threadImplementation() override;
	
public:
	PacketReceiver(Address& address, Socket* socket, MemoryPool* pool);

	bool receive(Packet& packet);
};
