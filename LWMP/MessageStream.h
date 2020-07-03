#pragma once

class MemoryPool;

class MessageData;
class MessageRequest;

struct MessageInfo;

class MessageStream
{
public:
    static void readMessages(BitReader& reader, const MessageInfo* info, 
        MemoryPool& pool, std::vector<MessageRequest>& requests, std::vector<MessageData>& messages, const Address& address);

    static void writeMessages(BitWriter& writer, const MessageInfo* info, 
        const std::vector<MessageRequest>& requests, const std::vector<MessageData>& messages, const Address& address);
};
