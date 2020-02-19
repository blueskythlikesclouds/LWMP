#pragma once

#include <cstddef>
#include <typeindex>

struct Address
{
    union
    {
        uint32_t address;
        uint8_t numbers[4];
    };
    uint16_t port;

    static const Address ANY;

    Address();
    Address(uint32_t address, uint16_t port);
    Address(const char* address, uint16_t port);
    Address(uint16_t port);

    bool operator==(const Address& right) const;
    bool operator!=(const Address& right) const;
    bool operator<(const Address& right) const;
    bool operator<=(const Address& right) const;
    bool operator>(const Address& right) const;
    bool operator>=(const Address& right) const;

    struct sockaddr_in toNative() const;
    static struct Address fromNative(struct sockaddr_in& socketAddress);

    size_t getHash() const;

};

namespace std
{
    template<>
    struct hash<Address>
    {
        size_t operator()(const Address& address) const noexcept
        {
            return address.getHash();
        }
    };
}