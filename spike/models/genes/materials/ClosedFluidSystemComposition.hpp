#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <genes/coding.hpp>    // encode_*(), decode_*()

namespace genes
{
    struct Body
    {
        // blood, lymph, hydraulic systems, etc.
        std::array<ClosedFluidSystemComponent, ClosedFluidSystemComponents::COUNT> components;

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
            for (auto component = components.begin(); 
                 component != components.end(); ++component)
            {
                *output++ = encode_fraction(*component);
            }
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            for (auto component = components.begin(); 
                 component != components.end(); ++component)
            {
                *component = decode_fraction(*input++);
            }
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            output = std::fill_n(output, ClosedFluidSystemComponents::COUNT, 1); // wavelengths
            return output;
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            output = std::fill_n(output, ClosedFluidSystemComponents::COUNT, 4); // wavelengths
            return output;
        }
    };
}
