#include "MessageData.h"
#include "MessageInfo.h"

MessageData::MessageData(const MessageInfo* info, std::shared_ptr<Message> message, const Address& address)
    : info(info), message(std::move(message)), address(address)
{
}

std::shared_ptr<Message> MessageData::get() const
{
    return message;
}

const MessageInfo* MessageData::getInfo() const
{
    return info;
}

uint32_t MessageData::getId() const
{
    return info->id;
}

Address MessageData::getAddress() const
{
    return address;
}