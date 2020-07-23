#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <models/gene/coding.hpp>    // encode_*(), decode_*()

namespace gene
{
    
    // 4 bytes
    struct PhotoreceptorStructure
    {

        // 3 bytes: eyes
        /*
        all units of distance are relative to width or height of distal-most appendage segment, whichever is smaller
        */
        // Simulates a growth factor triggered by photoreceptors, 
        // causing either a prominence (positive values) or depression (negative values) of given size.
        // This allows a directionality or amplification of the signal.
        // and can express either compound eyes (positive) or vertebrate eyes (negative)
        // if displacement exceeds the radius of the photoreceptive region, then the region assumes a spherical shape.
        float growth_displacement;
        // coverage of segment by a region of photoreceptive cells on the very tip of the appendage
        // if displacement causes a spherical cavity, radius defines the radius of that cavity
        float region_radius;
        // simulates a protective cover of cells over the photoreceptors, which may evolve to a cornea and/or lens
        // if displacement is negative, then the protective layer of skin either fuses or delaminates and fills with liquid,
        // causing a a vitreous body
        float cover_thickness;
        // simulates subdivision within the photoreceptive region, which may evolve to a compound eye
        float region_subdivision_count;

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            *output++ = encode_fraction(growth_displacement     );
            *output++ = encode_fraction(region_radius           );
            *output++ = encode_fraction(cover_thickness         );
            *output++ = encode_fraction(region_subdivision_count);
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            growth_displacement      = decode_fraction(*input++);
            region_radius            = decode_fraction(*input++);
            cover_thickness          = decode_fraction(*input++);
            region_subdivision_count = decode_fraction(*input++);
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
