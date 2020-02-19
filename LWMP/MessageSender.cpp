#include "BitWriter.h"
#include "MessageInfoRegistry.h"
#include "Messages.h"
#include "MessageSender.h"
#include "MessageStream.h"

#include <utility>

MessageSender::MessageSender(PacketSender* sender, MemoryPool* pool) : MessageHandler(sender, pool)
{
}

void MessageSender::update()
{
    PacketSender* sender = (PacketSender*)handler;
    std::unique_lock<std::mutex> lock = sender->lock();

    for (auto& address : addresses)
    {
        std::shared_ptr<uint8_t[]> data = pool->allocate<uint8_t[]>();

        uint32_t* hash = (uint32_t*)data.get();
        uint8_t* bits = data.get() + sizeof(uint32_t);

        BitWriter writer(bits, pool->getArenaSize() - sizeof(uint32_t));
        {
            for (size_t i = 0; i < MessageInfoRegistry::getCount(); i++) 
                MessageStream::writeMessages(writer, MessageInfoRegistry::get(i), requests, messages, address);
        }
        writer.flush();

        *hash = getMurmurHash(bits, writer.getPosition(), 0);
        sender->send(data, sizeof(uint32_t) + writer.getPosition(), address);
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

void MessageSender::send(const MessageInfo* info, std::shared_ptr<Message> message, const Address& address)
{
    addresses.insert(address);
    messages.emplace_back(info, std::move(message), address);
}
