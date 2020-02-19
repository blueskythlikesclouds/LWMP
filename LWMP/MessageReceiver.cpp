#include "MessageInfoRegistry.h"
#include "MessageReceiver.h"
#include "MessageStream.h"

MessageReceiver::MessageReceiver(PacketReceiver* receiver, MemoryPool* pool) : MessageHandler(receiver, pool)
{
}

void MessageReceiver::update()
{
    PacketReceiver* receiver = (PacketReceiver*)handler;
    std::unique_lock<std::mutex> lock = receiver->lock();

    this->clear();

    for (auto& packet : receiver->getPackets())
    {
        if (packet.getLength() <= 4)
            continue;

        uint32_t* hash = (uint32_t*)packet.get().get();
        uint8_t* bits = (uint8_t*)packet.get().get() + sizeof(uint32_t);
        const size_t bitsLength = packet.getLength() - sizeof(uint32_t);

        if (*hash != getMurmurHash(bits, bitsLength, 0))
            continue;

        BitReader reader(bits, bitsLength);

        try {
            for (size_t i = 0; i < MessageInfoRegistry::getCount(); i++)
                MessageStream::readMessages(reader, MessageInfoRegistry::get(i), pool, requests, messages, packet.getAddress());
        }
        catch (std::exception& exception)
        {
            DEBUG_PRINT("%s\n", exception.what());
        }
    }

    receiver->clear();
}