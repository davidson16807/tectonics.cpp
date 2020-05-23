#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace genetics
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
    Also note: we do not use color systems like RGB,
    because are they are not as likely to reproduce correct evolutionary behavior
    */
    // 4 bytes
    struct Pigmentation
    {
        std::uint8_t:4  brown;
        std::uint8_t:4  black;
        std::uint8_t:2  blue;
        std::uint8_t:2  green;
        std::uint8_t:2  purple;
        std::uint8_t:2  red;
        std::uint8_t:2  orange;
        std::uint8_t:2  yellow;
        std::uint8_t:2  uv;
        std::uint8_t:2  irridescence;
        std::uint8_t:2  reflector;
        std::uint8_t:2  bioluminesence;
        std::uint8_t:2  color_changing;
        std::uint8_t:2  photosynthetic;
    };
}
