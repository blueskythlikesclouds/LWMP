#pragma once
#include <cstdint>

typedef long varint_t;

inline varint_t readVarInt(uint8_t* source, varint_t* value)
{
	*value = 0;

	uint8_t byte;
	int32_t shift = 0;

	do
	{
		byte = *source++;
		*value |= (byte & 0x7F) << shift;
		shift += 7;
	} while (byte & 0x80);

	return shift / 7;
}

inline size_t writeVarInt(uint8_t* destination, varint_t value)
{
	size_t offset = 0;
	
	do
	{
		uint8_t bits = value & 0x7F;
		value >>= 7;
		destination[offset++] = bits | (value != 0 ? 0x80 : 0);
	} while (value != 0);

	return offset;
}