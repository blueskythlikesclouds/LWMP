#include "MessageInfoRegistry.h"
#include "MessageReceiver.h"
#include "MessageStream.h"
#include "Messages.h"
#include "Packet.h"
#include "PacketReceiver.h"

MessageReceiver::MessageReceiver(const std::shared_ptr<PacketReceiver>& receiver, const std::shared_ptr<MemoryPool>& pool) : MessageHandler(receiver, pool)
{
}

void MessageReceiver::update()
{
    PacketReceiver* receiver = (PacketReceiver*)handler.get();
    std::unique_lock<std::mutex> lock = receiver->lock();

    clear();

    for (auto& packet : receiver->getPackets())
    {
        BitReader reader(packet.get().get(), packet.getLength());

        try 
        {
            for (size_t i = 0; i < MessageInfoRegistry::getCount(); i++)
                MessageStream::readMessages(reader, MessageInfoRegistry::get(i), *pool, requests, messages, packet.getAddress());
        }

        catch (std::exception& exception)
        {
            DEBUG_PRINT("%s\n", exception.what());
        }
    }

    std::shared_ptr<MsgMetadata> metadata;
    for (auto& message : messages)
    {
        if (!message.isOfType<MsgMetadata>())
            continue;

        metadata = message.get<MsgMetadata>();
        break;
    }

    for (auto& message : messages)
        message.setMetadata(metadata);

    receiver->clear();
}