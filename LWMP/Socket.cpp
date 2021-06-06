#include "Socket.h"

Socket::Socket(const int32_t type, const int32_t protocol) : socket(::socket(AF_INET, type, protocol)), connected(false)
{
}

Socket::~Socket()
{
    closesocket(socket);
}

bool Socket::receive(uint8_t* data, const size_t length, Address* address, size_t* receivedLength)
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
    {
        connected = false;
        return false;
    }

    if (receivedLength)
        *receivedLength = result;

    connected = true;
    return true;
}

bool Socket::send(uint8_t* data, size_t length, const Address& address)
{
    const sockaddr_in socketAddress = address.toNative();
    int result = sendto(socket, (const char*)data, length, 0, (const sockaddr*)&socketAddress, sizeof(socketAddress));
    return connected = result != SOCKET_ERROR;
}

bool Socket::isConnected()
{
    return connected;
}
