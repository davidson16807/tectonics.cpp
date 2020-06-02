#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <models/genes/materials/Pigmentation.hpp>    // encode_*(), decode_*()
// #include <models/genes/materials/Metabolism.hpp>    // encode_*(), decode_*()
#include <models/genes/coding.hpp>    // encode_*(), decode_*()

namespace genes
{
    struct Endosymbiont
    {
        Pigmentation pigmentation;
        // Metabolism metabolism;
        bool exists;

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
            // output = metabolism  .encode(output);
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            input = pigmentation.encode(input);
            // input = metabolism  .encode(input);
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            output = Pigmentation::getMutationRates(output);
            // output = Metabolism  ::getMutationRates(output);
            return output;
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            output = Pigmentation::getAttributeSizes(output);
            // output = Metabolism  ::getAttributeSizes(output);
            return output;
        }
        static constexpr unsigned int bit_count = 
            Pigmentation::bit_count;
            // Metabolism  ::bit_count;
        static constexpr unsigned int attribute_count = 
            Pigmentation::attribute_count;
            // Metabolism  ::attribute_count;
    };
}
