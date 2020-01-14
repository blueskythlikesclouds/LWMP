#include "PacketReceiver.h"

void PacketReceiver::threadImplementation()
{
	while (!stop)
	{
		std::shared_ptr<uint8_t[]> arena = pool->rent<uint8_t[]>();
		size_t length;

		Address address = this->address;
		if (!socket->receive(address, arena.get(), pool->getArenaSize(), &length))
			continue;

		std::unique_lock<std::mutex> lock(mutex);
		packets.push({ arena, length, address });
	}
}

PacketReceiver::PacketReceiver(Address& address, Socket* socket, MemoryPool* pool) : PacketHandler(address, socket), pool(pool)
{
}

bool PacketReceiver::receive(Packet& packet)
{
	std::unique_lock<std::mutex> lock(mutex);
	if (packets.empty())
		return false;

	packet = packets.front();
	
	packets.pop();
	return true;
}
