#include "MessageRequest.h"

MessageRequest::MessageRequest(const MessageInfo* info, const Address& address) : info(info), address(address)
{
}

const MessageInfo* MessageRequest::getInfo() const
{
    return info;
}

Address MessageRequest::getAddress() const
{
    return address;
}
