#include "Client.h"

Client::Client() : Socket(SOCK_DGRAM, IPPROTO_UDP)
{
}

SocketType Client::getType() const
{
    return SocketType::CLIENT;
}