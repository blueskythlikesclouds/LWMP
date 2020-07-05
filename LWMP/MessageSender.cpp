#include "MemoryPool.h"
#include "MessageInfoRegistry.h"
#include "MessageSender.h"
#include "MessageStream.h"
#include "PacketSender.h"

MessageSender::MessageSender(const std::shared_ptr<PacketSender>& sender, const std::shared_ptr<MemoryPool>& pool) : MessageHandler(sender, pool)
{
}

void MessageSender::update()
{
    PacketSender* sender = (PacketSender*)handler.get();
    std::unique_lock<std::mutex> lock = sender->lock();

    for (auto& address : addresses)
    {
        std::shared_ptr<uint8_t[]> data = pool->allocate<uint8_t[]>();

        BitWriter writer(data.get(), pool->getArenaSize());
        {
            for (size_t i = 0; i < MessageInfoRegistry::getCount(); i++) 
                MessageStream::writeMessages(writer, MessageInfoRegistry::get(i), requests, messages, address);
        }
        writer.flush();

        sender->send(data, writer.getPosition(), address);
    }

    clear();
}

void MessageSender::clear()
{
    MessageHandler::clear();
    addresses.clear();
}

void MessageSender::request(const MessageInfo* info, const Address& address)
{
    addresses.insert(address);
    requests.emplace_back(info, address);
}

void MessageSender::send(const MessageInfo* info, const std::shared_ptr<Message>& message, const Address& address)
{
    addresses.insert(address);
    messages.emplace_back(info, message, address);
}
