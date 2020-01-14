#pragma once
#include "Packet.h"
#include <mutex>
#include <atomic>
#include <queue>
#include "MemoryPool.h"
#include "Socket.h"

class PacketHandler
{
protected:
	std::queue<Packet> packets;

	std::thread thread;
	std::mutex mutex;

	std::atomic<bool> stop;

	Address address;
	Socket* socket;

	virtual void threadImplementation() = 0;

public:
	PacketHandler(Address& address, Socket* socket);
	virtual Address getAddress() const;
	virtual void setAddress(Address& address);
	virtual ~PacketHandler();
};
