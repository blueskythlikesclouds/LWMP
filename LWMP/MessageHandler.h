#pragma once
#include <unordered_map>
#include <memory>
#include "PacketHandler.h"
#include "VarInt.h"

class MessageHandler
{
protected:
	varint_t requests;
	varint_t flags;
	std::unordered_map<uint32_t, std::shared_ptr<void>> messages;

	PacketHandler* handler;
	MemoryPool* pool;
	
public:
	MessageHandler(PacketHandler* handler, MemoryPool* pool);

	virtual Address getAddress() const;
	virtual void setAddress(Address& address);
	virtual void clear();
};
