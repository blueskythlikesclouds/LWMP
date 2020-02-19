#pragma once

#include "Sink.h"

class BitWriter
{
    std::shared_ptr<Sink> sink;
    brcpp::bitwriter<Sink> writer;

public:
    BitWriter(uint8_t* data, size_t length)
        : sink(std::make_shared<Sink>(data, length)), writer(sink)
    {
        
    }

    void write(const bool value)
    {
        writer.write<bool>(value, 1);
    }

    template<typename T>
    void write(const T value)
    {
        writer.write<T>(value, sizeof(T) * 8);
    }

    void writeVarUInt(const VarUInt value)
    {
        ::writeVarUInt(writer, value);
    }

    void writeVarInt(const VarInt value)
    {
        ::writeVarInt(writer, value);
    }

    void flush()
    {
        writer.flush();
    }

    size_t getPosition() const
    {
        return sink->position();
    }
};
