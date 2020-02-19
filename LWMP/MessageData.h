#pragma once

#include "Address.h"
#include "Messages.h"

#include <memory>

class MessageData
{
    const MessageInfo* info;
    std::shared_ptr<Message> message;
    Address address;

public:
    MessageData(const MessageInfo* info, std::shared_ptr<Message> message, const Address& address);

    const MessageInfo* getInfo() const;
    uint32_t getId() const;

    template<typename T>
    bool isOfType() const
    {
        return info == &T::INFO;
    }

    std::shared_ptr<Message> get() const;

    template<typename T>
    std::shared_ptr<T> get() const
    {
        return std::reinterpret_pointer_cast<T>(message);
    }

    Address getAddress() const;
};
