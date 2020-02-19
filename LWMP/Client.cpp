#include "Client.h"

Client::Client() : Socket(SOCK_DGRAM, IPPROTO_UDP)
{
}

Socket::Type Client::getType() const
{
    return Type::CLIENT;
}