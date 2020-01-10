#include "MessageInfo.h"

std::unordered_map<uint32_t, struct MessageInfo*> g_messageInfos;

MessageInfo::MessageInfo(uint32_t id, std::initializer_list<FieldInfo> fields, size_t byteSize, size_t alignment)
	: id(id), fields(fields), byteSize(byteSize), alignment(alignment)
{
	g_messageInfos[id] = this;
}
