#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <genes/coding.hpp>    // encode_*(), decode_*()

namespace genes
{

    /*
    a "Photopigment" is any compound within a photoreceptor responsible for light perception
    */
    // 4 bytes
    struct Photopigment
    {
        float log_concentration;
        float gene_disabled;

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            *output++ = encode_fraction(log_concentration);
            *output++ = encode_fraction(gene_disabled);
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            log_concentration = decode_fraction(*input++);
            gene_disabled     = decode_fraction(*input++);
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
    };
}
