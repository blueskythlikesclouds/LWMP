#include "MessageData.h"
#include "MessageInfo.h"

MessageData::MessageData(const MessageInfo* info, const std::shared_ptr<Message>& message, const Address& address)
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

std::shared_ptr<MsgMetadata> MessageData::getMetadata() const
{
    return metadata;
}

void MessageData::setMetadata(const std::shared_ptr<MsgMetadata>& metadata)
{
    this->metadata = metadata;
}
