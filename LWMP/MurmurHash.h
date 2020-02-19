#pragma once

inline uint32_t getMurmurHash(const uint8_t* data, size_t length, const uint32_t seed)
{
	const uint32_t m = 0x5bd1e995;
	const int32_t r = 24;

	uint32_t hash = seed ^ length;

	while (length >= 4)
	{
		uint32_t k = *(uint32_t*)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		hash *= m;
		hash ^= k;

		data += 4;
		length -= 4;
	}

	switch (length)
	{
	case 3: hash ^= data[2] << 16;
	case 2: hash ^= data[1] << 8;
	case 1: hash ^= data[0];
		hash *= m;
	}

	hash ^= hash >> 13;
	hash *= m;
	hash ^= hash >> 15;

	return hash;
}