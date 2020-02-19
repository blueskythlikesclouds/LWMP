#include "PacketHandler.h"

PacketHandler::PacketHandler(Socket* socket) : socket(socket), stop(false)
{
    thread = std::thread([this]()
    {
        while (!stop) 
        {
            update();
        }
    });
}

PacketHandler::~PacketHandler()
{
    stop = true;
    thread.join();
}

std::unique_lock<std::mutex> PacketHandler::lock()
{
    std::unique_lock<std::mutex> lock(mutex);
    return std::move(lock);
}

const std::vector<Packet>& PacketHandler::getPackets()
{
    return packets;
}

void PacketHandler::clear()
{
    packets.clear();
}
