#pragma once

#include "Socket.h"

class Client : public Socket
{
public:
    Client();

    SocketType getType() const override;
};
