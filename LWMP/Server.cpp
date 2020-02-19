#include "Server.h"

Server::Server(const uint16_t port) : Socket(SOCK_DGRAM, 0), port(port)
{
    sockaddr_in socketAddress = Address((uint32_t)ADDR_ANY, port).toNative();

    if (bind(socket, (sockaddr*)&socketAddress, sizeof(socketAddress)) == 0)
        return;

    DEBUG_PRINT("Server::Server(): bind() failed\n");
}

Socket::Type Server::getType() const
{
    return Type::SERVER;
}

uint16_t Server::getPort() const
{
    return port;
}
