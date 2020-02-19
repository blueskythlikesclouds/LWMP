#pragma once

#include "MemoryPool.h"
#include "PacketHandler.h"
#include "Socket.h"

class PacketReceiver : public PacketHandler
{
    MemoryPool* pool;

    void update() override;

public:
    PacketReceiver(Socket* socket, MemoryPool* pool);
};
