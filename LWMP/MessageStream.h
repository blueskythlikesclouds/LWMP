#pragma once

#include "BitReader.h"
#include "BitWriter.h"
#include "MemoryPool.h"
#include "MessageData.h"
#include "MessageInfo.h"
#include "MessageRequest.h"

#include <vector>

class MessageStream
{
public:
    static void readMessages(BitReader& reader, const MessageInfo* info, 
        MemoryPool* pool, std::vector<MessageRequest>& requests, std::vector<MessageData>& messages, const Address& address);

    static void writeMessages(BitWriter& writer, const MessageInfo* info, 
        const std::vector<MessageRequest>& requests, const std::vector<MessageData>& messages, const Address& address);
};
