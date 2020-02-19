#pragma once

#include "Socket.h"

class Client : public Socket
{
public:
    Client();

    Type getType() const override;
};
