#pragma once

#include "PacketHandler.h"

class PacketReceiver : public PacketHandler
{
    std::shared_ptr<MemoryPool> pool;

    void update() override;

public:
    PacketReceiver(const std::shared_ptr<Socket>& socket, const std::shared_ptr<MemoryPool>& pool);
};
