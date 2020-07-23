#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <models/gene/coding.hpp>    // encode_*(), decode_*()

namespace gene
{
    /*
    "TubeStructure" is any tube like structure used to trasnport a fluid or gas
    It can serve in nutrient transport, waste removal, hydraulics, pneumatics
    Examples include blood vessels, lymph vessels, 
    the respiratory tract, and the water vascular system of echinoderms
    */
    struct TubeStructure
    {
        float radius;
        float wall_thickness;

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            *output++ = encode_fraction(radius        );
            *output++ = encode_fraction(wall_thickness);
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            radius         = decode_fraction(*input++);
            wall_thickness = decode_fraction(*input++);
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            return std::fill_n(output, 2, 1);
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            return std::fill_n(output, 2, 4);
        }
        static constexpr unsigned int bit_count = 2*4;
        static constexpr unsigned int attribute_count = 2;
    };


}
