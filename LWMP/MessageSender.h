#pragma once

#include "MessageHandler.h"

class PacketSender;

struct Message;
struct MessageInfo;

class MessageSender : public MessageHandler
{
    std::set<Address> addresses;

public:
    MessageSender(const std::shared_ptr<PacketSender>& sender, const std::shared_ptr<MemoryPool>& pool);

    void update() override;
    void clear() override;
    
    void request(const MessageInfo* info, const Address& address);
    void send(const MessageInfo* info, const std::shared_ptr<Message>& message, const Address& address);

    template <typename T>
    void request(const Address& address)
    {
        request(&T::INFO, address);
    }

    template <typename T>
    void send(const std::shared_ptr<T>& message, const Address& address)
    {
        send(&T::INFO, std::reinterpret_pointer_cast<Message>(message), address);
    }
};