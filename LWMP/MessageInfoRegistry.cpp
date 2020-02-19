#include "MessageInfoRegistry.h"

std::vector<const MessageInfo*> MessageInfoRegistry::registry;

bool MessageInfoRegistry::add(const MessageInfo* info)
{
    registry.push_back(info);
    return true;
}

const MessageInfo* MessageInfoRegistry::get(const size_t index)
{
    return registry[index];
}

size_t MessageInfoRegistry::getCount()
{
    return registry.size();
}