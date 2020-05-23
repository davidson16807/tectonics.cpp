#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace genetics
{
    /*
    "Scaling" is any thin repeating surface structiure,
    made of a structural material such as chitin or keratin.
    It can serve as insolation, heat resistance, waterproofing, 
    protection from dessication, ornamentation, or armor.
    Examples include hair, feathers, spines, and (obviously) scales
    */
    // 4 bytes
    struct Scaling
    {
        std::uint8_t:4  coverage;
        std::uint8_t:4  length;
        std::uint8_t:4  width;
        std::uint8_t:4  thickness;
        std::uint8_t:4  flexibility;
        std::uint8_t:4  feathery_texture;
        std::uint8_t:8  padding;
    };
}
