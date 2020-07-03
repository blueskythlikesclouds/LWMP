#include "Packet.h"
#include "PacketSender.h"
#include "Socket.h"

void PacketSender::update()
{
    std::unique_lock<std::mutex> lock(mutex);

    for (auto& packet : packets)
        socket->send(packet.get().get(), packet.getLength(), packet.getAddress());

    packets.clear();
}

PacketSender::PacketSender(const std::shared_ptr<Socket>& socket) : PacketHandler(socket)
{
}

void PacketSender::send(const std::shared_ptr<uint8_t[]>& data, size_t length, const Address& address)
{
    packets.emplace_back(data, length, address);
}
