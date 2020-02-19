#pragma once

#include "MessageHandler.h"
#include "MessageInfo.h"
#include "PacketSender.h"

#include <set>

class MessageSender : public MessageHandler
{
    std::set<Address> addresses;

public:
    MessageSender(PacketSender* sender, MemoryPool* pool);

    void update() override;
    void clear() override;
    
    void request(const MessageInfo* info, const Address& address);
    void send(const MessageInfo* info, std::shared_ptr<Message> message, const Address& address);

    template <typename T>
    void request(const Address& address)
    {
        request(&T::INFO, address);
    }

    template <typename T>
    void send(std::shared_ptr<T> message, const Address& address)
    {
        send(&T::INFO, std::reinterpret_pointer_cast<Message>(message), address);
    }
};
