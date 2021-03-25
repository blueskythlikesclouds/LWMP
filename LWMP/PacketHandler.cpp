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
	// Keeping this causes the game to not exit for some reason?
    // TODO: Check why
    //thread.join();
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
