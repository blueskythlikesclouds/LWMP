#include "MemoryPool.h"
#include "PacketReceiver.h"
#include "Socket.h"

void PacketReceiver::update()
{
    std::shared_ptr<uint8_t[]> data = pool->allocate<uint8_t[]>();
    size_t length = 0;
    Address address = Address::ANY;

    if (!socket->receive(data.get(), pool->getArenaSize(), &address, &length) || !length)
        return;

    std::unique_lock<std::mutex> lock(mutex);
    packets.emplace_back(data, length, address);
}

PacketReceiver::PacketReceiver(const std::shared_ptr<Socket>& socket, const std::shared_ptr<MemoryPool>& pool) : PacketHandler(socket), pool(pool)
{
}