#pragma once

#include <boost/preprocessor.hpp>
#include <forward_list>
#include <unordered_map>

#define DEFINE_MESSAGE_INFO() \
	enum { ID = __COUNTER__ }; \
	static const MessageInfo INFO;

#define MESSAGE_INFO_FOR_EACH_MACRO(r, data, elem) \
	{offsetof(data, elem), sizeof(data::elem), std::is_same<decltype(data::elem), varint_t>::value},

#define DECLARE_MESSAGE_INFO(type, ...) \
	const MessageInfo type::INFO \
	( \
		type::ID, { BOOST_PP_SEQ_FOR_EACH(MESSAGE_INFO_FOR_EACH_MACRO, type, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) }, sizeof(type) \
	);

extern std::unordered_map<uint32_t, struct MessageInfo*> messageInfos;

struct FieldInfo
{
	const off_t offset;
	const size_t byteSize;
	const bool isVarInt;
};

struct MessageInfo
{
	const uint32_t id;
	const std::forward_list<FieldInfo> fields;
	const size_t byteSize;

	size_t getMaxPackedLength() const;

	MessageInfo(uint32_t id, std::initializer_list<FieldInfo> fields, size_t byteSize);
};

size_t getMaxPackedMessageSize();
size_t getMaxMessageSize();