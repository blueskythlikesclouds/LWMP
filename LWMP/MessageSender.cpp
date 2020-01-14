#include "MessageSender.h"
#include <utility>
#include "VarInt.h"

MessageSender::MessageSender(PacketSender* sender, MemoryPool* pool) : MessageHandler(sender, pool)
{
}

void MessageSender::request(const MessageInfo* messageInfo)
{
	requests |= 1 << messageInfo->id;
}

void MessageSender::add(const MessageInfo* messageInfo, std::shared_ptr<void> message)
{
	if (_bittest(&flags, messageInfo->id))
		fprintf(stderr, "Same type of message was already added before\n");

	flags |= 1 << messageInfo->id;
	messages[messageInfo->id] = std::move(message);
}

void MessageSender::send()
{
	Packet packet;
	packet.data = pool->rent<uint8_t[]>();

	size_t offset = writeVarInt(packet.data.get(), requests);
	offset += writeVarInt(packet.data.get() + offset, flags);

	for (size_t i = 0; i < messageInfos.size(); i++)
	{
		if (!_bittest(&flags, i))
			continue;

		MessageInfo* messageInfo = messageInfos[i];
		std::shared_ptr<uint8_t[]> message = std::reinterpret_pointer_cast<uint8_t[]>(messages[i]);
		
		for (auto& fieldInfo : messageInfo->fields)
		{
			if (fieldInfo.isVarInt)
			{
				offset += writeVarInt(packet.data.get() + offset, *(varint_t*)(message.get() + fieldInfo.offset));
			}
			else
			{
				memcpy(packet.data.get() + offset, message.get() + fieldInfo.offset, fieldInfo.byteSize);
				offset += fieldInfo.byteSize;
			}
		}
	}
	packet.length = offset;

	((PacketSender*)handler)->send(packet);

	clear();
}
