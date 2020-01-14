#pragma once
#include <memory>
#include "Address.h"

struct Packet
{
	std::shared_ptr<uint8_t[]> data;
	size_t length;
	Address address;
};
