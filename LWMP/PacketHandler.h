#pragma once

#include "Packet.h"

class Socket;

class PacketHandler
{
protected:
    std::shared_ptr<Socket> socket;

    std::vector<Packet> packets;

    std::thread thread;
    std::mutex mutex;

    std::atomic<bool> stop;

    virtual void update() = 0;

public:
    PacketHandler(const std::shared_ptr<Socket>& socket);
    virtual ~PacketHandler();
    
    virtual std::unique_lock<std::mutex> lock();
    virtual const std::vector<Packet>& getPackets();
    virtual void clear();

    virtual const bool hasPackets()
    {
        return getPackets().size() > 0;
    }
};
