#pragma once
#include <cstdint>

#pragma pack(push, 1)

struct Address
{
	uint32_t address{};
	uint16_t port{};

	Address();
	Address(uint32_t address, uint16_t port);
	Address(const char* address, uint16_t port);

	static struct Address fromSocketAddress(struct sockaddr_in& socketAddress);
	struct sockaddr_in toSocketAddress() const;
};

#pragma pack(pop)

const Address ANY_ADDRESS((uint32_t)0, (uint16_t)0);