#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <models/gene/coding.hpp>    // encode_*(), decode_*()

namespace gene
{
    /*
    a "Toxin" is any chemical used by an organism 
    to either attack (venom) or defend (poison) against another.
    */
    // 4 bytes
    struct Toxins
    {
        float coagulance; // whether the toxins act collectively as a coagulant or anticoagulant (e.g. snake venom)
        float acidity; // whether the toxins act as an acid (e.g. bees, ants)
        float neurotoxicity; // whether the toxin disrupts signaling between neurons (e.g. tetanus, botulitum, black widow venom)
        float necrotoxicity; // whether the toxin causes deterioration of cell walls (e.g. brown recluse venom)

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            *output++ = encode_ranged  (coagulance, -1.0f, 1.0f);
            *output++ = encode_ranged  (acidity,    -1.0f, 1.0f);
            *output++ = encode_fraction(neurotoxicity);
            *output++ = encode_fraction(necrotoxicity);
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            coagulance    = decode_ranged  (*input++, -1.0f, 1.0f);
            acidity       = decode_ranged  (*input++, -1.0f, 1.0f);
            neurotoxicity = decode_fraction(*input++);
            necrotoxicity = decode_fraction(*input++);
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            return std::fill_n(output, 4, 1);
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            return std::fill_n(output, 4, 4);
        }
        static constexpr unsigned int bit_count = 4*4;
        static constexpr unsigned int attribute_count = 4;
    };
}
