#include "Address.h"

const Address Address::ANY;

Address::Address() : address(0), port(0)
{
}

Address::Address(const uint32_t address, const uint16_t port) : address(address), port(port)
{
}

Address::Address(const char* address, const uint16_t port) : address(0), port(port)
{
    inet_pton(AF_INET, address, &this->address);
}

Address::Address(const uint16_t port) : address(0), port(port)
{
}

bool Address::operator==(const Address& right) const
{
    return address == right.address
        && port == right.port;
}

bool Address::operator!=(const Address& right) const
{
    return !(*this == right);
}

bool Address::operator<(const Address& right) const
{
    if (address < right.address)
        return true;
    if (right.address < address)
        return false;
    return port < right.port;
}

bool Address::operator<=(const Address& right) const
{
    return !(right < *this);
}

bool Address::operator>(const Address& right) const
{
    return right < *this;
}

bool Address::operator>=(const Address& right) const
{
    return !(*this < right);
}

Address Address::fromHostName(const char* address, uint16_t port)
{
    ADDRINFOA* info = nullptr;
    INT result = getaddrinfo(address, "0", NULL, &info);

    while (info != nullptr)
    {
        if (info->ai_family == AF_INET)
        {
            sockaddr_in* addr = (sockaddr_in*)info->ai_addr;
            return { addr->sin_addr.S_un.S_addr, port };
        }
        info = info->ai_next;
    }

    return { 0u, 0 };
}

sockaddr_in Address::toNative() const
{
    sockaddr_in socketAddress{};
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.S_un.S_addr = address;
    socketAddress.sin_port = htons(port);
    memset(&socketAddress.sin_zero, 0, sizeof(socketAddress.sin_zero));

    return socketAddress;
}

Address Address::fromNative(sockaddr_in& socketAddress)
{
    return { socketAddress.sin_addr.S_un.S_addr, ntohs(socketAddress.sin_port) };
}

size_t Address::getHash() const
{
    size_t seed = 0x1D1A3FE2;
    seed ^= (seed << 6) + (seed >> 2) + 0x47222BCE + static_cast<size_t>(address);
    seed ^= (seed << 6) + (seed >> 2) + 0x484D01A6 + static_cast<size_t>(port);
    return seed;
}
