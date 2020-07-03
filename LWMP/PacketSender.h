#pragma once

#include "PacketHandler.h"

class PacketSender : public PacketHandler
{
    void update() override;

public:
    PacketSender(const std::shared_ptr<Socket>& socket);

    void send(const std::shared_ptr<uint8_t[]>& data, size_t length, const Address& address);
};
