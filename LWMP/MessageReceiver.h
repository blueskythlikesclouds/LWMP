#pragma once
#include "MessageHandler.h"
#include "PacketReceiver.h"
#include "MessageInfo.h"

class MessageReceiver : public MessageHandler
{
public:
	MessageReceiver(PacketReceiver* receiver, MemoryPool* pool);

	bool isRequested(const MessageInfo* messageInfo) const;
	bool contains(const MessageInfo* messageInfo) const;
	std::shared_ptr<void> get(const MessageInfo* messageInfo);

	template<typename T>
	bool isRequested() const
	{
		return isRequested(&T::INFO);
	}

	template<typename T>
	bool contains() const
	{
		return contains(&T::INFO);
	}

	template<typename T>
	std::shared_ptr<T> get()
	{
		return std::reinterpret_pointer_cast<T>(get(&T::INFO));
	}

	Address receive();
};
