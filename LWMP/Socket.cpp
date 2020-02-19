#include "Socket.h"

Socket::Socket(const int32_t type, const int32_t protocol) : socket(::socket(AF_INET, type, protocol))
{
}

Socket::~Socket()
{
    closesocket(socket);
}

bool Socket::receive(uint8_t* data, const size_t length, Address* address, size_t* receivedLength) const
{
    size_t result;

    if (address)
    {
        sockaddr_in socketAddress;
        size_t socketAddressLength = sizeof(socketAddress);

        result = recvfrom(socket, (char*)data, length, 0, (sockaddr*)&socketAddress, (int*)&socketAddressLength);

        if (result != SOCKET_ERROR && address)
            *address = Address::fromNative(socketAddress);
    }
    else
    {
        result = recvfrom(socket, (char*)data, length, 0, nullptr, nullptr);
    }

    if (result == SOCKET_ERROR)
        return false; 

    if (receivedLength)
        *receivedLength = result;

    return true;
}

bool Socket::send(uint8_t* data, const uint32_t length, const Address& address) const
{
    sockaddr_in socketAddress = address.toNative();
    return sendto(socket, (const char*)data, length, 0, (const sockaddr*)&socketAddress, sizeof(socketAddress)) != SOCKET_ERROR;
}