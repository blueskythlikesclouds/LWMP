#pragma once

struct MessageInfo;

class MessageRequest
{
protected:
    const MessageInfo* info;
    Address address;

public:
    MessageRequest(const MessageInfo* info, const Address& address);

    const MessageInfo* getInfo() const;

    template<typename T>
    bool isOfType() const
    {
        return info == &T::INFO;
    }

    Address getAddress() const;
};
