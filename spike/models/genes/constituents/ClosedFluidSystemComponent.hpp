#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <genes/coding.hpp>    // encode_*(), decode_*()

namespace genes
{

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

    a "ClosedFluidSystemConstituent" is any component of a closed fluid system
    it mainly includes oxygen transporters, 
    and could be extended to include hormones, white blood cells, etc.
    concentrations express mass percentages within nonhydraulic vessels
    as a log scale ratio to homoglobin concentration in mammals
    */
    // 4 bytes
    struct ClosedFluidSystemConstituent
    {
        float log_concentration;
        float log_multiplier_for_nonhydraulic_systems;
        float gene_disabled;
        /*
        equal to blood concentration within nonhydraulic vessels 
        values near 1 indicate no distinction between hydraulic and nonhydraulic vessels,
        such as the hemolymphatic system seen in insects
        */

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            *output++ = encode_fraction(log_concentration);
            *output++ = encode_fraction(log_multiplier_for_nonhydraulic_systems);
            *output++ = encode_fraction(gene_disabled);
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            log_concentration                       = decode_fraction(*input++);
            log_multiplier_for_nonhydraulic_systems = decode_fraction(*input++);
            gene_disabled                           = decode_fraction(*input++);
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            return std::fill_n(output, 3, 1);
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            return std::fill_n(output, 3, 4);
        }
    };

    enum ClosedFluidSystemConstituents
    {
        // red, iron based, very efficient but succeptible to CO and acidity
        hemoglobin_concentrations,
        // green, iron based, 25% efficieny of hemoglobin
        chlorocruorin_concentration,
        // colorless, iron based, effective in CO
        hemerythrin_concentration,
        // colorless, maganese based, little known
        pinnaglobin_concentration,
        // blue/colorless, copper based, more efficient in low oxygen conditions
        hemocyanin_concentration,
        // multicolor, vanadium based, effective when acidic, insoluble in water, 
        vanadium_chromagen_concentration,
        // yellow/pink, cobolt based
        coboglobin_concentration,

        COUNT
    };

}
