﻿#include "Packet.h"

Packet::Packet(const std::shared_ptr<uint8_t[]>& data, const size_t length, const Address& address)
    : data(data), length(length), address(address)
{
}

std::shared_ptr<uint8_t[]> Packet::get() const
{
    return data;
}

size_t Packet::getLength() const
{
    return length;
}

Address Packet::getAddress() const
{
    return address;
}
