#include "MessageReceiver.h"

MessageReceiver::MessageReceiver(PacketReceiver* receiver, MemoryPool* pool) : MessageHandler(receiver, pool)
{
}

bool MessageReceiver::isRequested(const MessageInfo* messageInfo) const
{
	return _bittest(&requests, messageInfo->id);
}

bool MessageReceiver::contains(const MessageInfo* messageInfo) const
{
	return _bittest(&flags, messageInfo->id);
}

std::shared_ptr<void> MessageReceiver::get(const MessageInfo* messageInfo)
{
	return messages[messageInfo->id];
}

Address MessageReceiver::receive()
{
	Packet packet;
	if (!((PacketReceiver*)handler)->receive(packet))
		return ANY_ADDRESS;

	size_t offset = readVarInt(packet.data.get(), &requests);
	offset += readVarInt(packet.data.get() + offset, &flags);

	messages.clear();
	for (size_t i = 0; i < messageInfos.size(); i++)
	{
		if (!_bittest(&flags, i))
			continue;

		MessageInfo* messageInfo = messageInfos[i];
		std::shared_ptr<uint8_t[]> message = pool->rent<uint8_t[]>();
		
		for (auto& fieldInfo : messageInfo->fields)
		{
			if (fieldInfo.isVarInt)
			{
				offset += readVarInt(packet.data.get() + offset, (varint_t*)(message.get() + fieldInfo.offset));
			}
			else
			{
				memcpy(message.get() + fieldInfo.offset, packet.data.get() + offset, fieldInfo.byteSize);
				offset += fieldInfo.byteSize;
			}
		}
		
		messages.insert({ messageInfo->id, message });
	}

	return packet.address;
}
