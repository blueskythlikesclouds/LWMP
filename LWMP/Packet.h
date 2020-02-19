#pragma once

#include "Address.h"
#include <memory>

class Packet
{
protected:
    std::shared_ptr<uint8_t[]> data;
    size_t length;
    Address address;

public:
    Packet(std::shared_ptr<uint8_t[]> data, size_t length, const Address& address);

    std::shared_ptr<uint8_t[]> get() const;
    size_t getLength() const;
    Address getAddress() const;
};
