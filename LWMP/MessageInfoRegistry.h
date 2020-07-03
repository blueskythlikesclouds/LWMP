#pragma once

struct MessageInfo;

class MessageInfoRegistry
{
    static std::vector<const MessageInfo*> registry;
    
public:
    static bool add(const MessageInfo* info);

    static const MessageInfo* get(size_t index);
    static size_t getCount();
};
