#pragma once

#include <boost/preprocessor.hpp>
#include <list>
#include <unordered_map>

typedef size_t varint_t;

extern std::unordered_map<uint32_t, struct MessageInfo*> g_messageInfos;

struct FieldInfo
{
	const off_t offset;
	const size_t byteSize;
	const bool isVariantInSize;
};

struct MessageInfo
{
	const uint32_t id;
	const std::list<FieldInfo> fields;
	const size_t byteSize;
	const size_t alignment;

	MessageInfo(uint32_t id, std::initializer_list<FieldInfo> fields, size_t byteSize, size_t alignment);
};

#define DEFINE_MESSAGE_INFO() \
	enum { ID = __COUNTER__ }; \
	static const MessageInfo INFO;

#define MESSAGE_INFO_FOR_EACH_MACRO(r, data, elem) \
	{offsetof(data, elem), sizeof(data::elem), std::is_same<decltype(data::elem), varint_t>::value},

#define DECLARE_MESSAGE_INFO(type, ...) \
	const MessageInfo type::INFO \
	( \
		type::ID, { BOOST_PP_SEQ_FOR_EACH(MESSAGE_INFO_FOR_EACH_MACRO, type, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) }, sizeof(type), alignof(type) \
	);
