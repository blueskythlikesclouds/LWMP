#pragma once

#include <stdexcept>

class Sink
{
    uint8_t* data;
    size_t length;
    size_t pos;

public:
    Sink(uint8_t* data, const size_t length) : data(data), length(length), pos(0)
    {
    }

    void put(uint8_t data, size_t bits)
    {
        if (pos > length)
            throw std::runtime_error("Sink exceeded max length");

        this->data[pos++] = data;
    }

    size_t position() const
    {
        return pos;
    }
};
