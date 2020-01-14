#include "MessageInfo.h"
#include <algorithm>

std::unordered_map<uint32_t, struct MessageInfo*> messageInfos;

size_t MessageInfo::getMaxPackedLength() const
{
	size_t size = 5;

	for (auto& fieldInfo : fields)
	{
		size += fieldInfo.byteSize;
		if (fieldInfo.isVarInt)
			size++;
	}

	return size;
}

MessageInfo::MessageInfo(uint32_t id, std::initializer_list<FieldInfo> fields, size_t byteSize)
	: id(id), fields(fields), byteSize(byteSize)
{
	messageInfos[id] = this;
}

size_t getMaxPackedMessageSize()
{
	size_t size = 0;
	for (auto& pair : messageInfos)
		size = std::max(size, pair.second->getMaxPackedLength());

	return size;
}

size_t getMaxMessageSize()
{
	size_t size = 0;
	for (auto& pair : messageInfos)
		size = std::max(size, pair.second->byteSize);

	return size;
}
