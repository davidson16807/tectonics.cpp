#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <models/genes/coding.hpp>    // encode_*(), decode_*()
#include <models/genes/segments/BodySegment.hpp>
#include <models/genes/materials/ClosedFluidSystemComposition.hpp>
#include <models/genes/materials/PhotoreceptorMaterials.hpp>
#include <models/genes/materials/Mineralization.hpp>

namespace genes
{
    /*
    Most arthropods seem to have ~7 segments, 
    but their common ancestor may have had up to 11 (Kukalova-Peck, 1992).
    We should probably set this number to something that helps us 
    conserve memory and reason with memory allocation and cache lines
    */
    const int BODY_SEGMENT_COUNT = 8;
    

    // ~1.2 KB
    /*
    a "Body" describes all attributes of an organism within a single life cycle stage

    It attempts to be:
    * properly partial (represents a proper subset of all possible organisms, i.e. no illegal state space)
    It does not attempt to be:
    * total (does not describe all possible organisms)
    * unique (allows multiple ways to express the same organism)
    * unambiguous (an instance can describe more than one real life organism, e.g. armadillos vs turtles)

    A body data structure describes:
    * segmentation patterns (radial symmetry, segment count, etc.)
    * segment specializations
    * materials that can be synthesized by the body

    See [designing-an-evolutionary-model.md](designing-an-evolutionary-model.md) for more information
    */
    struct Body
    {
        // arms, legs, flippers, wings, antenna, eye stalks, facial structures
        std::array<BodySegment, BODY_SEGMENT_COUNT> segments;

        // basic dimensions, in meters, log2 scale
        float log2_total_length;
        float log2_max_width_as_multiple_of_length; 
        float log2_max_height_as_multiple_of_length;

        float morulation_factor;
        float gastrulation_factor;
        float mesoderm_formation_factor;
        float axial_segment_count;
        float radial_segment_count;
        float angular_segment_count;
        float angular_segment_axial_offset; // for segment whorling or alternation
        float metamorphic_dissolution_factor; // indicating breakdown of tissue from previous stage, such as in a chrysalis
        // float gametogenesis_factor; // indicating formation of haploid cells during this stage
        // float meiosis_factor; // indicating formation of diploid cells during this stage

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

        /*
        NOTE: I would normally create encode() and decode() as pure functions outside the class,
        but some classes in the genotype/phenotype pipeline use encapsulation 
        to prevent underflow/overflow and prevent generation of illegal state space.
        These classes require encode() and decode() as methods, 
        and I design the other classes to follow suit to create consistent design.

        So I'm refining by principle for avoiding methods: 
        You should never declare a pure function as a method unless you are willing 
        to reason with that function's behavior while not knowing
        which attributes within the class are touched.
        This is because the attributes that are touched are not clearly stated in a function signature.
        */
        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            /* 
            Pelagibacter ubique is among the smallest known free-living bacteria, 
            on the order of dex -6.5 m at smallest.
            The blue whale is on the order of dex 1.5 m long.
            A redwood on the order of dex 2 m tall.
            Wayne Douglas Barlowe's Emperor Sea Strider is a large yet realistic animal
            that is possible under certain alien conditions, which is dex 2.5m tall.
            The world's longest siphonophore is 40m long yet as thin "as a broomhandle", maybe 2.5cm.
            An ediacarian cyclomedusa could measure 35mm in diameter and 0.5mm in relief (Ruidong 2010).
            */
            output = encode_container(segments.begin(), segments.end(), output);
            *output++ = encode_ranged  (log2_total_length, -6.5f*3.3f, 2.5f*3.3f, 16);
            *output++ = encode_ranged  (log2_max_width_as_multiple_of_length,  -10, 6, 16);
            *output++ = encode_ranged  (log2_max_height_as_multiple_of_length, -10, 6, 16);
            *output++ = encode_fraction(morulation_factor             );
            *output++ = encode_fraction(gastrulation_factor           );
            *output++ = encode_fraction(mesoderm_formation_factor     );
            *output++ = encode_fraction(axial_segment_count           );
            *output++ = encode_fraction(radial_segment_count          );
            *output++ = encode_fraction(angular_segment_count         );
            *output++ = encode_fraction(angular_segment_axial_offset  );
            *output++ = encode_fraction(metamorphic_dissolution_factor);
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            input = decode_container(input, segments.begin(), segments.end());
            log2_total_length                     = decode_ranged(*input++, -6.5f*3.3f, 2.5f*3.3f, 16);
            log2_max_width_as_multiple_of_length  = decode_ranged(*input++, -10, 6, 16);
            log2_max_height_as_multiple_of_length = decode_ranged(*input++, -10, 6, 16);
            morulation_factor              = decode_fraction(*input++);
            gastrulation_factor            = decode_fraction(*input++);
            mesoderm_formation_factor      = decode_fraction(*input++);
            axial_segment_count            = decode_fraction(*input++);
            radial_segment_count           = decode_fraction(*input++);
            angular_segment_count          = decode_fraction(*input++);
            angular_segment_axial_offset   = decode_fraction(*input++);
            metamorphic_dissolution_factor = decode_fraction(*input++);
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            for (int i = 0; i < BODY_SEGMENT_COUNT; ++i)
            {
                output = BodySegment::getMutationRates(output);
            }
            output = std::fill_n(output, 3, 1);
            output = std::fill_n(output, 8, 1);
            return output;
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            for (int i = 0; i < BODY_SEGMENT_COUNT; ++i)
            {
                output = BodySegment::getAttributeSizes(output);
            }
            output = std::fill_n(output, 3, 16);
            output = std::fill_n(output, 8, 4);
            return output;
        }
        static constexpr unsigned int bit_count = 
            BodySegment                 ::bit_count * BODY_SEGMENT_COUNT +
            3*16 + 8*4;
        static constexpr unsigned int attribute_count = 
            BodySegment                 ::attribute_count * BODY_SEGMENT_COUNT +
            3 + 8;
    };
}
