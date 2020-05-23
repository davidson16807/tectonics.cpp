#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace genetics
{

    // 148 bytes, 37 words
    struct BodySegment: GenericSegment
    {
        // 12 bytes: attributes from GenericSegment

        // 120 bytes
        Appendage appendage;

        // 4 bytes: scaling
        Scaling scaling;

        // 4 bytes: pigmentation
        Pigmentation pigmentation;

        // 2 bytes: hard structure
        HardStructure hard_structure;
        /*
        NOTE: if pigmentation appears on a body segment, then the organism will always display it no matter what,
        this means it is best suited for situations that don't require the organism's involvement.
        such as for protection against the elements or ambush (camoflage, deterrence, etc) 
        */

        /*
        BODY SCALING SYSTEM:
        total body length                       meters, log scale
        total body width                        max body segment width found within the body
        total body height                       max body segment height found within the body
        body segment length                     relative portion of total body length
        body segment height                     multiple of total body length, log scale
        body segment width                      multiple of total body length, log scale
        appendage length                        multiple of total body length, log scale
        appendage width                         max appendage segment width found within the appendage
        appendage height                        max appendage segment height found within the appendage
        appendage segment length                relative portion of appendage length
        appendage segment height                fraction of body segment height
        appendage segment width                 fraction of body segment length
        appendage segment insertion             fraction of parent appendage segment
        appendage segment thickness attributes  proportion of half the appendage segment width or height, whichever is smallest
        appendage segment vessel radius         proportion of half the appendage segment width or height, whichever is smallest
        appendage segment vessel thickness      fraction of corresponding vessel radius
        appendage segment coverage              fraction of appendage segment surface area
        */

        // 2 bytes: closed fluid systems
        /*
        a "closed fluid system" is any fluid system that's closed to the environment 
        (if it's open to the environment, you're having a bad day)
        a closed fluid system is broken into two subsystems, as we see in animals:
        * a "lymphohydraulic" system that either transports waste or hydraulic power for motion, and...
        * a "nonhydraulic" system that either transports waste or nutrients
        the composition of fluid within the system (e.g. whether it contains hemoglobin) is defined by 
        `blood_*` attributes within the `Body` class. 
        Another attribute, `nonhydraulic_vessel_blood_concentration_multiplier`,
        determines the extent to which the nonhydraulic system specializes in nutrient transport.
        Some animals such as insects have a shared hemolymphatic system, and we need to represent this some how.
        However the hydraulic component cannot significantly specialize in nutrient transport,
        since the pumping that's needed for locomotion will likely interfere with the pumping that's needed for 
        reliable nutrient transport, thereby nullifying any gain you get out of specialization. 
        This is not always the case (for instance blood is used to provide the hydraulic power that's needed to 
        generate erections), however it is probably true enough to prevent blood from providing hydraulic power 
        to large limbs within organisms of a sufficient size. As we see in the real world,
        nature seems to start off with a lymphatic system and then evolve the circulatory system as 
        a specialized subcomponent with higher concentrations of oxygen transporters (see Choi 2012)
        */
        std::uint8_t:4  closed_system_hydraulic_pump_radius;    
        std::uint8_t:4  closed_system_hydraulic_pump_thickness; 
        std::uint8_t:4  closed_system_nonhydraulic_pump_radius;    
        std::uint8_t:4  closed_system_nonhydraulic_pump_thickness; 

        // 3 bytes: open fluid system
        /*
        a open fluid system is an open system that transports air or fluid from the outside
        examples include the respiratory system of vertebrates and the water vascular system of star fish
        these are collectively called "respiratory" here, despite that term being overly specific
        open fluid pumps function as diaphrams
        open fluid pores function as nostrils and spiracles
        open fluid circulatory exchanges function as lungs or gills, transporting nutrients to the circulatory system
        */
        std::uint8_t:4  open_system_pump_radius; 
        std::uint8_t:4  open_system_pump_thickness; 
        std::uint8_t:4  open_system_pore_radius; 

        // 1 bytes: miscellaneous
        std::uint8_t:4  neuron_bundle_thickness; 
        std::uint8_t:4  fat_thickness;
    };

}
