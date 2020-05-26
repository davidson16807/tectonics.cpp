#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <genes/coding.hpp>    // encode_*(), decode_*()

namespace genes
{
    
    // 4 bytes
    struct PressureSensingStructure
    {

        // Simulates a growth factor triggered by same regulator for fine hairs, 
        // causing either a prominence (positive values) or depression (negative values) of given size.
        // This allows a directionality or amplification of the signal
        float sensitive_scaling_displacement;   
        // coverage of segment by a region of fine spikes/feathers/hairs
        // if displacement causes a spherical cavity, radius defines the radius of that cavity
        float sensitive_scaling_radius;         
        // simulates a protective cover of cells over the fine scalings, which may evolve to a tympannic membrane
        float sensitive_scaling_cover_thickness;
        // simulates a protective cover of cells over the fine scalings, which may evolve to a tympannic membrane
        float sensitive_scaling_cover_displacement;
        float sensitive_scaling_cover_funnel_height;

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            *output++ = encode_fraction(displacement       );
            *output++ = encode_fraction(radius             );
            *output++ = encode_fraction(cover_thickness    );
            *output++ = encode_fraction(cover_displacement );
            *output++ = encode_fraction(cover_funnel_height);
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            displacement        = decode_fraction(*input++);
            radius              = decode_fraction(*input++);
            cover_thickness     = decode_fraction(*input++);
            cover_displacement  = decode_fraction(*input++);
            cover_funnel_height = decode_fraction(*input++);
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
    };


}
