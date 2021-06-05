#include "PacketHandler.h"

PacketHandler::PacketHandler(const std::shared_ptr<Socket>& socket) : socket(socket), stop(false)
{
    thread = std::thread([this]()
    {
        while (!stop)
        {
            update();
            Sleep(10); // was wondering why my cpu was crying
        }
    });
}

PacketHandler::~PacketHandler()
{
    stop = true;

	// This object is dying, please go live by yourself and likely stop
    thread.detach();
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
