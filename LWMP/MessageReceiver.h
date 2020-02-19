#pragma once

#include "MessageHandler.h"
#include "PacketReceiver.h"

class MessageReceiver : public MessageHandler
{
public:
    MessageReceiver(PacketReceiver* receiver, MemoryPool* pool);

    void update() override;
};
