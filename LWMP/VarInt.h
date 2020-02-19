#pragma once

#define READ_BITS(type, count) \
    reader.template read<type>(count)

#define WRITE_BITS(type, value, count) \
    writer.template write<type>((type)value, count)

typedef long VarInt;
typedef unsigned long VarUInt;

constexpr size_t VAR_INT_BIT_COUNT = 7;
constexpr size_t VAR_INT_BIT_MASK = (1 << VAR_INT_BIT_COUNT) - 1;

template<typename Source>
static VarUInt readVarUInt(brcpp::bitreader<Source>& reader)
{
    VarUInt value = 0;
    size_t shiftCount = 0;

    do
    {
        value |= READ_BITS(uint8_t, VAR_INT_BIT_COUNT) << shiftCount;
        shiftCount += VAR_INT_BIT_COUNT;
    } while (READ_BITS(bool, 1));

    return value;
}

template<typename Source>
static VarInt readVarInt(brcpp::bitreader<Source>& reader)
{
    const VarUInt value = readVarUInt(reader);

    if (value & 1)
        return ~(value >> 1);

    return value >> 1;
}

template<typename Sink>
static void writeVarUInt(brcpp::bitwriter<Sink>& writer, VarUInt value)
{
    do
    {
        WRITE_BITS(uint8_t, value & VAR_INT_BIT_MASK, VAR_INT_BIT_COUNT);
        value >>= VAR_INT_BIT_COUNT;
        WRITE_BITS(bool, value != 0, 1);
    } while (value != 0);
}

template<typename Sink>
static void writeVarInt(brcpp::bitwriter<Sink>& writer, VarInt value)
{
    constexpr size_t SIGN_BIT = (sizeof(VarUInt) * 8) - 1;

    if (_bittest(&value, SIGN_BIT))
        value = ~(value << 1);
    else
        value = value << 1;

    writeVarUInt(writer, (VarUInt)value);
}

#undef WRITE_BITS
#undef READ_BITS