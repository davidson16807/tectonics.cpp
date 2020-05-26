#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace genes
{
    inline std::int8_t encode_fraction(const float input, const int bit_count = 4)
    {
        return input * (1 << bit_count);
    }
    inline std::int8_t encode_portion(const float input)
    {
        return input;
    }
    inline std::int8_t encode_ranged(const float input, float lo, float hi, const int bit_count = 4)
    {
        return ((input-lo) / (hi-lo)) * (1 << bit_count);
    }

    inline float decode_fraction(const std::int8_t input, const int bit_count = 4)
    {
        return input >> bit_count;
    }
    inline float decode_portion(const std::int8_t input)
    {
        return input;
    }
    inline float decode_ranged(const std::int8_t input, float lo, float hi, const int bit_count = 4)
    {
        return (hi-lo) * (input >> bit_count) + lo;
    }
}