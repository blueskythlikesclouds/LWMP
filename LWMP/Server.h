#pragma once

#include "Socket.h"

class Server : public Socket
{
    uint16_t port;

public:
    Server(uint16_t port);

    SocketType getType() const override;
    uint16_t getPort() const;
};
