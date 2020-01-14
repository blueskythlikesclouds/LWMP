#pragma once
#include "MessageHandler.h"
#include "MessageInfo.h"
#include "PacketSender.h"

class MessageSender : public MessageHandler
{
public:
	MessageSender(PacketSender* sender, MemoryPool* pool);
	
	void request(const MessageInfo* messageInfo);
	void add(const MessageInfo* messageInfo, std::shared_ptr<void> message);

	template<typename T>
	void request()
	{
		request(&T::INFO);
	}

	template<typename T>
	void add(std::shared_ptr<T> message)
	{
		add(&T::INFO, std::reinterpret_pointer_cast<void>(message));
	}

	void send();
};
