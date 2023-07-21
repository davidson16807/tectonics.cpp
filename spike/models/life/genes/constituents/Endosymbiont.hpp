#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <models/genes/biochemistry/Pigmentation.hpp>    // encode_*(), decode_*()
#include <models/genes/biochemistry/Metabolism.hpp>    // encode_*(), decode_*()
#include <models/genes/coding.hpp>    // encode_*(), decode_*()

namespace genes
{
    struct Endosymbiont
    {
        Pigmentation pigmentation;
        Metabolism metabolism;
        float exists;

        /*
        Converts the genome of a standalone organism to a 
        compressed variant meant for endosymbionts
        */
        /*
        void convert(Genome& genome)
        {
            exists = true;
            pigmentation = genome.pigmentation_synthesis;
            // metabolism   = genome.metabolism;
        }
        */

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            output = pigmentation.encode(output);
            output = metabolism  .encode(output);
            *output++ = encode_fraction(exists);
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            input = pigmentation.decode(input);
            input = metabolism  .decode(input);
            exists = decode_fraction(*input++);
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            output = Pigmentation::getMutationRates(output);
            output = Metabolism  ::getMutationRates(output);
            output = std::fill_n(output, 1, 1);
            return output;
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            output = Pigmentation::getAttributeSizes(output);
            output = Metabolism  ::getAttributeSizes(output);
            output = std::fill_n(output, 1, 4);
            return output;
        }
        static constexpr unsigned int bit_count = 
            Pigmentation::bit_count +
            Metabolism  ::bit_count +
            4;
        static constexpr unsigned int attribute_count = 
            Pigmentation::attribute_count +
            Metabolism  ::attribute_count +
            1;
    };
}
