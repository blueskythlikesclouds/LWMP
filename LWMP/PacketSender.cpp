#include "PacketSender.h"

void PacketSender::threadImplementation()
{
	while (!stop)
	{
		std::unique_lock<std::mutex> lock(mutex);
		if (packets.empty())
			continue;

		Packet& packet = packets.front();
		socket->send(address, packet.data.get(), packet.length);

		packets.pop();
	}
}

PacketSender::PacketSender(Address& address, Socket* socket) : PacketHandler(address, socket)
{
}

void PacketSender::send(Packet& packet)
{
	std::unique_lock<std::mutex> lock(mutex);
	packets.push(packet);
}
