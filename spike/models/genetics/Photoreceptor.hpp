#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace genetics
{
    /*
    "Photoreceptor" is a photosensitive chemical used to treat 
    */
    // 1 byte
    struct Photoreceptor
    {
        std::uint8_t:4  wavelength;
        std::uint8_t:4  length;
    };
}
