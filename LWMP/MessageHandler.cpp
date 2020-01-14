#include "MessageHandler.h"

MessageHandler::MessageHandler(PacketHandler* handler, MemoryPool* pool) : requests(0), flags(0), handler(handler), pool(pool)
{
}

Address MessageHandler::getAddress() const
{
	return handler->getAddress();
}

void MessageHandler::setAddress(Address& address)
{
	handler->setAddress(address);
}

void MessageHandler::clear()
{
	requests = 0;
	flags = 0;
	messages.clear();
}
