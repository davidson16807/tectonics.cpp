#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace genes
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
}