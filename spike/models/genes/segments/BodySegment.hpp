#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <genes/coding.hpp>    // encode_*(), decode_*()

namespace genes
{

    // 37 words
    struct BodySegment: GenericSegment
    {
        // : attributes from GenericSegment
        Appendage appendage;

        // : surface structure: hair, spines, feathers, scales
        SurfaceStructure surface_structure;

        // : pigmentation
        Pigmentation pigmentation;

        // : corneous structure
        CorneousStructure corneous_structure;

        // mineralization for dermal bone
        Mineralization exterior_mineralization;


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

        // : closed fluid systems
        TubeStructure closed_system_hydraulic_pump;    
        TubeStructure closed_system_nonhydraulic_pump;    

        // : open fluid system
        /*
        a open fluid system is an open system that transports air or fluid from the outside
        examples include the respiratory system of vertebrates and the water vascular system of star fish
        these are collectively called "respiratory" here, despite that term being overly specific
        open fluid pumps function as diaphrams
        open fluid pores function as nostrils and spiracles
        open fluid circulatory exchanges function as lungs or gills, transporting nutrients to the circulatory system
        */
        TubeStructure open_system_pump; 
        TubeStructure open_system_pore; 

        // : miscellaneous
        float neuron_bundle_thickness; 
        float fat_thickness;
        float fused_to_parent_factor;

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            output = appendage                      .encode(output);
            output = surface_structure              .encode(output);
            output = pigmentation                   .encode(output);
            output = corneous_structure             .encode(output);
            output = exterior_mineralization        .encode(output);
            output = closed_system_hydraulic_pump   .encode(output);
            output = closed_system_nonhydraulic_pump.encode(output);
            output = open_system_pump               .encode(output);
            output = open_system_pore               .encode(output);
            *output++ = encode_fraction(neuron_bundle_thickness);
            *output++ = encode_fraction(fat_thickness          );
            *output++ = encode_fraction(fused_to_parent_factor );
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            input = appendage                      .decode(input);
            input = surface_structure              .decode(input);
            input = pigmentation                   .decode(input);
            input = corneous_structure             .decode(input);
            input = exterior_mineralization        .decode(input);
            input = closed_system_hydraulic_pump   .decode(input);
            input = closed_system_nonhydraulic_pump.decode(input);
            input = open_system_pump               .decode(input);
            input = open_system_pore               .decode(input);
            neuron_bundle_thickness = decode_fraction(*input++);
            fat_thickness           = decode_fraction(*input++);
            fused_to_parent_factor  = decode_fraction(*input++);
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            output = Appendage        ::getMutationRates(output);
            output = SurfaceStructure ::getMutationRates(output);
            output = Pigmentation     ::getMutationRates(output);
            output = CorneousStructure::getMutationRates(output);
            output = Mineralization   ::getMutationRates(output);
            output = TubeStructure    ::getMutationRates(output);
            output = TubeStructure    ::getMutationRates(output);
            output = TubeStructure    ::getMutationRates(output);
            output = TubeStructure    ::getMutationRates(output);
            return std::fill_n(output, 3, 1);
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            output = Appendage        ::getAttributeSizes(output);
            output = SurfaceStructure ::getAttributeSizes(output);
            output = Pigmentation     ::getAttributeSizes(output);
            output = CorneousStructure::getAttributeSizes(output);
            output = Mineralization   ::getAttributeSizes(output);
            output = TubeStructure    ::getAttributeSizes(output);
            output = TubeStructure    ::getAttributeSizes(output);
            output = TubeStructure    ::getAttributeSizes(output);
            output = TubeStructure    ::getAttributeSizes(output);
            return std::fill_n(output, 3, 4);
        }
    };

}
