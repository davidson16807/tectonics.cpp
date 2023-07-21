#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <models/genes/coding.hpp>    // encode_*(), decode_*()

namespace genes
{
    // 2 bytes
    struct CorneousStructure
    {
        /*
        a "corneous structure" is a term used to represent any thick nonrepeating surface structure, 
        made of a structural material such as chitin or keratin.
        It can serve as ornamentation, weaponry, armor, or digging apparatus.
        Examples include antlers, tusks, talons, horns, spikes, 
        claws (crabs, cats, velociraptors), and various plates (stegasaurus, armadillos, etc.)
        */
        float base_width;
        float top_width;
        float base_depth;
        float top_depth;
        float length;

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            *output++ = encode_fraction(base_width);  
            *output++ = encode_fraction(top_width );  
            *output++ = encode_fraction(base_depth);   
            *output++ = encode_fraction(top_depth );     
            *output++ = encode_fraction(length    );     
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            base_width = decode_fraction(*input++);
            top_width  = decode_fraction(*input++);
            base_depth = decode_fraction(*input++);
            top_depth  = decode_fraction(*input++);
            length     = decode_fraction(*input++);
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            return std::fill_n(output, 5, 1);
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            return std::fill_n(output, 5, 4);
        }
        static constexpr unsigned int bit_count = 5*4;
        static constexpr unsigned int attribute_count = 5;
    };
}
