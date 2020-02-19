#pragma once

class BitReader
{
protected:
    brcpp::bitreader<brcpp::memory_byte_source> reader;

public:
    BitReader(uint8_t* data, size_t length) :
        reader(std::make_unique<brcpp::memory_byte_source>(data, length))
    {
        
    }

    bool read()
    {
        return reader.read<bool>(1);
    }

    template<typename T>
    T read()
    {
        return reader.read<T>(sizeof(T) * 8);
    }

    VarUInt readVarUInt()
    {
        return ::readVarUInt(reader);
    }

    VarInt readVarInt()
    {
        return ::readVarInt(reader);
    }
};
