#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace gene
{
    inline unsigned int encode_fraction(const float input, const int bit_count = 4)
    {
        return std::clamp(int(input * (1<<bit_count)), 0, (1<<bit_count)-1);
    }
    inline unsigned int encode_portion(const float input, const int bit_count = 4)
    {
        return std::clamp(int(input), 0, (1<<bit_count)-1);
    }
    inline unsigned int encode_ranged(const float input, float lo, float hi, const int bit_count = 4)
    {
        return std::clamp(int(round(((input-lo) / (hi-lo)) * float(1 << bit_count))), 0, (1<<bit_count)-1);
    }
    template<typename TInputIterator, typename TOutputIterator>
    inline TOutputIterator encode_container(TInputIterator iterator, TInputIterator end, TOutputIterator output)
    {
        for (; iterator != end; ++iterator)
        {
            output = iterator->encode(output);
        }
        return output;
    }

    inline float decode_fraction(const unsigned int input, const int bit_count = 4)
    {
        return input / float(1<<bit_count);
    }
    inline float decode_portion(const unsigned int input)
    {
        return input;
    }
    inline float decode_ranged(const unsigned int input, float lo, float hi, const int bit_count = 4)
    {
        return (hi-lo) * (input / float(1<<bit_count)) + lo;
    }
    template<typename TInputIterator, typename TOutputIterator>
    inline TInputIterator decode_container(TInputIterator input, TOutputIterator iterator, TOutputIterator end)
    {
        for (; iterator != end; ++iterator)
        {
            input = iterator->decode(input);
        }
        return input;
    }
}