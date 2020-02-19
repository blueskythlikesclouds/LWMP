#pragma once

#include "Address.h"

class Socket
{
protected:
    SOCKET socket;

public:
    enum class Type
    {
        UNKNOWN,
        CLIENT,
        SERVER
    };

    Socket(int32_t type, int32_t protocol);
    virtual ~Socket();

    virtual bool receive(uint8_t* data, size_t length, Address* address = nullptr, size_t* receivedLength = nullptr) const;
    virtual bool send(uint8_t* data, size_t length, const Address& address) const;
    virtual Type getType() const = 0;
};
