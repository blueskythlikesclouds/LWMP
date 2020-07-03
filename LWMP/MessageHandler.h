#pragma once

#include "MessageData.h"
#include "MessageRequest.h"

class PacketHandler;
class MemoryPool;

class MessageHandler
{
protected:
    std::vector<MessageRequest> requests;
    std::vector<MessageData> messages;

    std::shared_ptr<PacketHandler> handler;
    std::shared_ptr<MemoryPool> pool;

public:
    MessageHandler(const std::shared_ptr<PacketHandler>& handler, const std::shared_ptr<MemoryPool>& pool);
    virtual ~MessageHandler();
    
    virtual void update() = 0;
    virtual void clear();

    virtual const std::vector<MessageRequest>& getRequests();
    virtual const std::vector<MessageData>& getMessages();
};
