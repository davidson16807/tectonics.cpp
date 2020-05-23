#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace genetics
{
    // Most arthropods seem to have ~7 segments, 
    // but their common ancestor may have had up to 11 (Kukalova-Peck, 1992).
    // We should probably set this number to something that helps us 
    // conserve memory and reason with memory allocation and cache lines
    const int MAX_SEGMENT_COUNT = 8;
    
    // Rainbow shrimp have 12-16 photoreceptor types, the largest number known, 
    // but most have no more than 4 (e.g. RGB and UV)
    const int MAX_PHOTORECEPTOR_COUNT = 8;

    const int OXYGEN_TRANSPORTER_COUNT = 8;
    enum OxygenTransporter
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
        coboglobin_concentration
    };

    // ~1.2 KB
    struct Body
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

        // 148*8 bytes: appendage segments
        BodySegment[MAX_SEGMENT_COUNT] segments;

        // 12 bytes: basic dimensions, in meters
        float length;
        float width;
        float height;

        // 4 bytes
        std::uint8_t:4[MAX_PHOTORECEPTOR_COUNT] photoreceptor_wavelengths;

        // 4 bytes
        /*
        oxygen transporters
        concentrations express mass percentages within nonhydraulic vessels
        as a log scale ratio to homoglobin concentration in mammals
        */
        std::uint8_t:4[OXYGEN_TRANSPORTER_COUNT] blood_hemoglobin_concentrations;

        // 2 bytes
        bool[MAX_SEGMENT_COUNT] segment_hometic_genes_disabled;
        bool[MAX_PHOTORECEPTOR_COUNT] photoreceptor_genes_disabled;

        // 1 bytes
        /*
        blood concentrated subsystem multiplier, log scale,
        equal to blood concentration within nonhydraulic vessels 
        values near 1 indicate no distinction between hydraulic and nonhydraulic vessels,
        such as the hemolymphatic system seen in insects
        */
        std::uint8_t:4    nonhydraulic_vessel_blood_concentration_multiplier;

    };
}
