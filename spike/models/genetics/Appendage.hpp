#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace genetics
{
    // Most arthropods seem to have ~7 segments, but their common ancestor may have had up to 11 (Kukalova-Peck, 1992).
    // We should probably set this number to something that helps us conserve memory and line up with cache sizes.
    // setting APPENDAGE_SEGMENT_COUNT = 8 allows us to easily reason with memory alignment and cache lines
    const int APPENDAGE_SEGMENT_COUNT = 8;

    // 120 bytes
    struct Appendage
    {
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

        // 12*8 bytes: appendage segments
        AppendageSegment[APPENDAGE_SEGMENT_COUNT] segments;

        // 4 bytes: scaling
        Scaling scaling;

        // 4 bytes: pigmentation
        Pigmentation pigmentation;

        // 2 bytes: hard structure
        HardStructure hard_structure;

        /*
        NOTE: if pigmentation appears on an appendage, then the organism can choose whether or not to display it,
        this means it can be useful in specialized circumstances without being detrimental in others
        however it requires intentional use from the organism, 
        so it is not for routine use or protection against ambush (camoflage, deterrence, etc) 
        It can be used for pack signaling, hunting lures, advertisement, or startle displays.
        */

        // 3 bytes: basic dimensions
        std::uint8_t   length;
        std::uint8_t   width;
        std::uint8_t   height;

        // 3 bytes: eyes
        /*
        all units of distance are relative to width or height of distal-most appendage segment, whichever is smaller
        */
        // Simulates a growth factor triggered by photoreceptors, 
        // causing either a prominence (positive values) or depression (negative values) of given size.
        // This allows a directionality or amplification of the signal.
        // and can express either compound eyes (positive) or vertebrate eyes (negative)
        // if displacement exceeds the radius of the photoreceptive region, then the region assumes a spherical shape.
        std::uint8_t:4    photoreceptor_growth_displacement;
        // coverage of segment by a region of photoreceptive cells on the very tip of the appendage
        // if displacement causes a spherical cavity, radius defines the radius of that cavity
        std::uint8_t:4    photoreceptor_region_radius;
        // simulates a protective cover of cells over the photoreceptors, which may evolve to a cornea and/or lens
        // if displacement is negative, then the protective layer of skin either fuses or delaminates and fills with liquid,
        // causing a a vitreous body
        std::uint8_t:4    photoreceptor_cover_thickness;
        // the extinction coefficient of the material that makes up the cover
        std::uint8_t:4    photoreceptor_cover_extinction_coefficient;
        // the extinction coefficient of the fluid that fills any covered depression
        std::uint8_t:4    photoreceptor_humor_extinction_coefficient;
        // simulates subdivision within the photoreceptive region, which may evolve to a compound eye
        std::uint8_t:4    photoreceptor_region_subdivision_count;

        // 3 bytes: ears
        // Simulates a growth factor triggered by photoreceptors, 
        // causing either a prominence (positive values) or depression (negative values) of given size.
        // This allows a directionality or amplification of the signal
        std::uint8_t:4    sensitive_scaling_displacement;   
        // coverage of segment by a region of scaling/feathers/hairs on the very tip of the appendage
        // if displacement causes a spherical cavity, radius defines the radius of that cavity
        std::uint8_t:4    sensitive_scaling_radius;         
        // simulates a protective cover of cells over the fine scalings, which may evolve to a tympannic membrane
        std::uint8_t:4    sensitive_scaling_cover_thickness;
        // simulates a protective cover of cells over the fine scalings, which may evolve to a tympannic membrane
        std::uint8_t:4    sensitive_scaling_cover_displacement;
        std::uint8_t:4    sensitive_scaling_cover_funnel_height;

        // detects motion within appendages, notably including johnston's organ for detecting sound in insects
        std::uint8_t:4    kinaesthetic_motion_detector;

        // 3 bytes: miscellaneous
        // chemical detection by touch or by 
        // signal can be improved either by swishing the appendage (antenna) or by moving air or fluid across it,
        // such as by placing it near an intake for a pneumatic system
        std::uint8_t:4    taste_receptor_coverage; 

        std::array<bool, APPENDAGE_SEGMENT_COUNT> segment_hometic_genes_disabled;
        bool is_fused;
    };
}
