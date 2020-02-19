#pragma once

#include "PacketHandler.h"
#include "Socket.h"

class PacketSender : public PacketHandler
{
    void update() override;

public:
    PacketSender(Socket* socket);

    void send(std::shared_ptr<uint8_t[]> data, size_t length, const Address& address);
};
