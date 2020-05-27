#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <models/genes/coding.hpp>    // encode_*(), decode_*()

namespace genes
{
    /*
    "Mineralization" describes when the biochemistry of an organism 
    extracts minerals from solution for use in hardening structures
    Structures that can be hardened this way include bone, exoskeletons, 
    and Corneous structures.

    Minerals must be common enough in the environment to used by organisms.
    This limits the number of possible biominerals that could exist on an organism.
    */
    // 4 bytes
    struct Mineralization
    {
        float silica;
        float calcium_carbonate;
        float hydroxyapatite;
        float graphene;
        float metal;

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            *output++ = encode_fraction(silica           );
            *output++ = encode_fraction(calcium_carbonate);
            *output++ = encode_fraction(hydroxyapatite   );
            *output++ = encode_fraction(graphene         );
            *output++ = encode_fraction(metal            );
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            silica            = decode_fraction(*input++);
            calcium_carbonate = decode_fraction(*input++);
            hydroxyapatite    = decode_fraction(*input++);
            graphene          = decode_fraction(*input++);
            metal             = decode_fraction(*input++);
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
    };
}
