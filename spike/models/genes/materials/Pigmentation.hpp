#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <models/genes/coding.hpp>    // encode_*(), decode_*()

namespace genes
{
    /*
    "Pigmentation" is a color assigned to any visible surface on an organism.
    It can serve a number of purposes:
      * camoflage
      * adaptive camoflage
      * warning coloration
      * batesian mimicry (resemblance to warning coloration in another species)
      * advertising
      * sexual selection coloration
      * startle coloration (e.g. moth eyes)
      * motion dazzle
      * hunting lure (angler fish)
      * sun burn protection
    We do not use color systems like RGB,
    because are they are not as likely to reproduce correct evolutionary behavior

    If pigmentation appears on an appendage, 
    then the organism can choose whether or not to display it,
    this means it can be useful in specialized circumstances without being detrimental in others
    however it requires intentional use from the organism, 
    so it is not for default use or protection against ambush (camoflage, deterrence, etc) 
    It can be used for pack signaling, hunting lures, advertisement, or startle displays.
    (possible wrinkle to consider: some appendages hide body pigmentation in their default use,
    such as the elytra of fireflies)

    If pigmentation appears on a body segment, then the organism will always display it no matter what,
    this means it is best suited for situations that don't require the organism's involvement.
    such as for protection against the elements or ambush (camoflage, deterrence, etc) 
    */
    // 4 bytes
    struct Pigmentation
    {
        float brown;
        float black;
        float blue;
        float green;
        float purple;
        float red;
        float orange;
        float yellow;
        float uv;
        float irridescence;
        float reflector;
        float bioluminesence;
        float color_changing;
        float photosynthetic;

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            *output++ = encode_fraction(brown         );
            *output++ = encode_fraction(black         );
            *output++ = encode_fraction(blue          );
            *output++ = encode_fraction(green         );
            *output++ = encode_fraction(purple        );
            *output++ = encode_fraction(red           );
            *output++ = encode_fraction(orange        );
            *output++ = encode_fraction(yellow        );
            *output++ = encode_fraction(uv            );
            *output++ = encode_fraction(irridescence  );
            *output++ = encode_fraction(reflector     );
            *output++ = encode_fraction(bioluminesence);
            *output++ = encode_fraction(color_changing);
            *output++ = encode_fraction(photosynthetic);
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            brown          = decode_fraction(*input++);
            black          = decode_fraction(*input++);
            blue           = decode_fraction(*input++);
            green          = decode_fraction(*input++);
            purple         = decode_fraction(*input++);
            red            = decode_fraction(*input++);
            orange         = decode_fraction(*input++);
            yellow         = decode_fraction(*input++);
            uv             = decode_fraction(*input++);
            irridescence   = decode_fraction(*input++);
            reflector      = decode_fraction(*input++);
            bioluminesence = decode_fraction(*input++);
            color_changing = decode_fraction(*input++);
            photosynthetic = decode_fraction(*input++);
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            return std::fill_n(output, 14, 1);
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            return std::fill_n(output, 14, 4);
        }
        static constexpr unsigned int bit_count = 14*4;
        static constexpr unsigned int attribute_count = 14;
    };
}
