#pragma once

#include "MessageHandler.h"

class PacketReceiver;

class MessageReceiver : public MessageHandler
{
public:
    MessageReceiver(const std::shared_ptr<PacketReceiver>& receiver, const std::shared_ptr<MemoryPool>& pool);

    void update() override;
};
