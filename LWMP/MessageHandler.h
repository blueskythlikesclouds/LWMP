#pragma once

#include "MemoryPool.h"
#include "MessageData.h"
#include "MessageRequest.h"
#include "PacketHandler.h"

#include <vector>

class MessageHandler
{
protected:
    std::vector<MessageRequest> requests;
    std::vector<MessageData> messages;

    PacketHandler* handler;
    MemoryPool* pool;

public:
    MessageHandler(PacketHandler* handler, MemoryPool* pool);
    virtual ~MessageHandler();
    
    virtual void update() = 0;
    virtual void clear();

    virtual const std::vector<MessageRequest>& getRequests();
    virtual const std::vector<MessageData>& getMessages();
};
