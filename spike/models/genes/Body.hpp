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
    a "Body" describes all attributes of an organism that contribute to fitness.

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
        // blood, lymph, hydraulic systems, etc.
        ClosedFluidSystemComposition closed_fluid_system_composition;
        // materials of the lens, retina, cornea, vitreous humor, etc.
        PhotoreceptorMaterials photoreceptor_materials;
        // minerals within endoskeleton
        Mineralization interior_mineralization;

        // basic dimensions, in meters
        float length;
        float width;
        float height;

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
            for (auto segment = segments.begin(); 
                 segment != segments.end(); ++segment)
            {
                output = segment->encode(output);
            }
            output = closed_fluid_system_composition.encode(output);
            output = photoreceptor_materials.encode(output);
            output = interior_mineralization.encode(output);

            *output++ = encode_range   (length, bit_count=16);
            *output++ = encode_fraction(width,  bit_count=16);
            *output++ = encode_fraction(height, bit_count=16);

            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            for (auto segment = segments.begin(); 
                 segment != segments.end(); ++segment)
            {
                input = segment->decode(input);
            }
            input  = closed_fluid_system_composition.decode(input);
            input  = photoreceptor_materials.decode(input);
            input  = interior_mineralization.decode(input);

            length = decode_range   (*input++, bit_count=16);
            width  = decode_fraction(*input++, bit_count=16);
            height = decode_fraction(*input++, bit_count=16);

            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            for (auto segment = segments.begin(); 
                 segment != segments.end(); ++segment)
            {
                output = BodySegment::getMutationRates(output);
            }
            output = closed_fluid_system_composition.getMutationRates(output);
            output = photoreceptor_materials.getMutationRates(output);
            output = interior_mineralization.getMutationRates(output);
            output = std::fill_n(output, 3, 1);

            return output;
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            // segments
            for (auto segment = segments.begin(); 
                 segment != segments.end(); ++segment)
            {
                output = BodySegment::getAttributeSizes(output);
            }
            output = closed_fluid_system_composition.getAttributeSizes(output);
            output = photoreceptor_materials.getAttributeSizes(output);
            output = interior_mineralization.getAttributeSizes(output);
            output = std::fill_n(output, 3, 16);
            return output;
        }
        static constexpr unsigned int bit_count = 
            BodySegment                 ::bit_count * BODY_SEGMENT_COUNT +
            ClosedFluidSystemComposition::bit_count +
            PhotoreceptorMaterials      ::bit_count +
            Mineralization              ::bit_count +
            3*16;
    };
}
