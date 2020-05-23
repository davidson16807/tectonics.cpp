#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace genetics
{
    // 2 bytes
    struct HardStructure
    {
        /*
        a "hard structure" is a term used to represent any thick nonrepeating surface structure, 
        made of a structural material such as chitin or keratin.
        It can serve as ornamentation, weaponry, armor, or digging apparatus.
        Examples include antlers, tusks, talons, horns, spikes, 
        claws (crabs, cats, velociraptors), and various plates (stegasaurus, armadillos, etc.)
        */
        std::uint8_t:4  hard_structure_width;
        std::uint8_t:4  hard_structure_depth;
        std::uint8_t:4  hard_structure_length;
        std::uint8_t:4  hard_structure_tapering;
    };
}
