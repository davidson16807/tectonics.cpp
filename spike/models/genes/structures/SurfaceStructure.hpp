#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <academics/units.hpp>    // encode_*(), decode_*()
#include <models/genes/coding.hpp>    // encode_*(), decode_*()

namespace genes
{
    /*
    "SurfaceStructure" is any thin repeating surface structiure,
    made of a structural material such as chitin or keratin.
    It can serve as insolation, heat resistance, waterproofing, 
    protection from dessication, ornamentation, or armor.
    Examples include hair, feathers, spines, and (obviously) scales
    */
    struct SurfaceStructure
    {
        float coverage;
        float length;
        float width;
        float thickness;
        float feathery_texture;
        float log_elastic_modulus;

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            *output++ = encode_fraction(coverage        );
            *output++ = encode_fraction(length          );
            *output++ = encode_fraction(width           );
            *output++ = encode_fraction(thickness       );
            *output++ = encode_fraction(feathery_texture);
            *output++ = encode_ranged  (log_elastic_modulus, SurfaceStructure::MIN_ELASTIC_MODULUS, SurfaceStructure::MAX_ELASTIC_MODULUS);
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            coverage            = decode_fraction(*input++);
            length              = decode_fraction(*input++);
            width               = decode_fraction(*input++);
            thickness           = decode_fraction(*input++);
            feathery_texture    = decode_fraction(*input++);
            log_elastic_modulus = decode_ranged  (*input++, SurfaceStructure::MIN_ELASTIC_MODULUS, SurfaceStructure::MAX_ELASTIC_MODULUS);
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            return std::fill_n(output, 6, 1);
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            return std::fill_n(output, 6, 4);
        }

        static constexpr unsigned int bit_count = 6*4;

    private:
        // Bonser (1995), for feather and hair
        // log GPa
        static constexpr float MAX_ELASTIC_MODULUS = log2(3.0*units::GIGAPASCAL);
        // elastic modulus of hair scaled to approximate procupine quills,
        // given young's modulii for hair and quills from Fortier (2012)
        // log GPa
        static constexpr float MIN_ELASTIC_MODULUS = log2(1.0*units::GIGAPASCAL / 8.0);

    };


}
