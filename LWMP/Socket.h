#pragma once

enum class SocketType
{
    UNKNOWN,
    CLIENT,
    SERVER
};

class Socket
{
protected:
    SOCKET socket;
    bool connected;

public:
    Socket(int32_t type, int32_t protocol);
    virtual ~Socket();

    virtual bool receive(uint8_t* data, size_t length, Address* address = nullptr, size_t* receivedLength = nullptr);
    virtual bool send(uint8_t* data, size_t length, const Address& address);
    virtual SocketType getType() const = 0;
    virtual bool isConnected();
};