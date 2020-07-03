#include "MessageHandler.h"

MessageHandler::MessageHandler(const std::shared_ptr<PacketHandler>& handler, const std::shared_ptr<MemoryPool>& pool) : handler(handler), pool(pool)
{
}

MessageHandler::~MessageHandler() = default;

void MessageHandler::clear()
{
    requests.clear();
    messages.clear();
}

const std::vector<MessageRequest>& MessageHandler::getRequests()
{
    return requests;
}

const std::vector<MessageData>& MessageHandler::getMessages()
{
    return messages;
}