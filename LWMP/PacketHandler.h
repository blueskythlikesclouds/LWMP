#pragma once

#include "Packet.h"
#include "Socket.h"

#include <atomic>
#include <mutex>
#include <vector>

class PacketHandler
{
protected:
    Socket* socket;

    std::vector<Packet> packets;

    std::thread thread;
    std::mutex mutex;

    std::atomic<bool> stop;

    virtual void update() = 0;

public:
    PacketHandler(Socket* socket);
    virtual ~PacketHandler();
    
    virtual std::unique_lock<std::mutex> lock();
    virtual const std::vector<Packet>& getPackets();
    virtual void clear();
};
