#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <models/genes/coding.hpp>    // encode_*(), decode_*()

namespace genes
{
    /*
    A "body segment" refers to one of the compartmentalized regions 
    of a segmented, motile, multicellular organism.
    For brevity, we will call such an organism an "animal",
    even though the term is more restrictive in some ways (suggesting heterotrophism)
    and less restrictive in others (allowing non-segmented or non-motile organisms)
    
    The `GenericSegment` data structure is designed for use in a genetic algorithm. 
    To aid in that use case, attributes must be designed in such a way that 
    it is impossible for there to exist a combination of attribute values
    that could not represent an animal in the real world, 
    i.e. there are no illegal regions of state space. s.

    Also to support use in a genetic algorithm, the `GenericSegment` data structure is 
    designed to foster evolutionary tendencies seen in the real world.
    The simulation should not produce a preponderance of a certain kind of body plan.
    For instance, it should not consistently produce 6 legged creatures for all cicrcumstances, or flying creatures.
    Any real-life region of parameter space needs its own set of trade offs that make it superior for certain kinds of niche,
    and when in doubt, it should err on the side of creating the very best adapted body shapes for a niche,
    even if it is not as inclined to represent historical baggage, 
    since that could also produce preponderances from initial conditions.
    It's easier to accomplish this if some characteristics serve multiple purposes
    allowing us to easily jump between niches.
    This also mimics what we see in the real world.
    Also, if a parameter is easy to modify in the real world, 
    it should be easy to modify in our simulation,
    and if a parameter takes a long time to emerge, 
    it may either have a slow gradient descent factor, 
    or it may require some interaction to exist between multiple parameters,
    but in the latter case it should be reflective of the real world.
    For instance: 
    * Body scale is easy to evolve, so it should be represented 
    * Wings are implied any time a limb has appropriate dimensions. 
      We avoid Spore-style, teleological leg and arm part enums 
      in favor of generic parts that can function as any of those
      Exceptions are only made where this is not possible, as in the case of sensory organs.
    
    The `GenericSegment` data structure is designed to describe as many organisms as possible.
    It generally tries to support any body plan that could be seen in vertebrates and arthropods. 
    This results in some design choices that might at first not seem obvious: 
    * It allows one heart to exist per segment, since we see this body plan in insects.
    * 


    The `GenericSegment` data structure is designed to be safe to modify.
    It doesn't try to be memory efficient, since that's 
    It also doesn't try to make parameters easy to use when writing fitness functions.
    To make it easier to work with, we transform it to other data structures.
    */

    // 24 nybbles, 12 bytes, 3 words, same size as a vec3
    struct GenericSegment
    {
        float length;
        float width;
        float height;

        float endoskeleton_radius;
        float exoskeleton_thickness;

        /*
        Terminology:
        extensor muscle:    a set of muscles that pull the segment towards the segment's parent
        flexor muscle:      a set of muscles that pull the segment away from the segment's parent
        muscle insertion:   region along the segment at which the muscle attaches to the bone or exoskeleton 
        muscle origin:      region along the segment's parent at which the muscle attaches to the bone or exoskeleton 
        */
        float extensor_muscle_relaxed_radius;
        float extensor_muscle_insertion;
        float extensor_muscle_origin;
        float extensor_to_flexor_muscle_ratio;

        /*
        "vessels" are any tubular structure used for fluid transport or hydraulics
        extensor/flexor vessels are lymphohydraulic
        nonhydraulic vessels may serve as blood vessels or lymph vessels, depending on 
        Its functions include nutrient transport, waste removal, temperature regulation, hydraulic motion, and pneumatic motion
        */
        float hydraulic_closed_system_vessel_radius;
        float hydraulic_closed_system_vessel_thickness;
        // float extensor_to_flexor_closed_system_vessel_ratio;

        float open_system_vessel_radius;
        float open_system_vessel_thickness;
        // float extensor_to_flexor_open_system_vessel_ratio;

        float nonhydraulic_closed_system_vessel_radius;
        float nonhydraulic_closed_system_vessel_thickness;

        // All attributes here describe the motion of the joint that connects the segment to its *parent*
        float joint_vertical_range_min;
        float joint_vertical_range_max;
        float joint_horizontal_range_min;
        float joint_horizontal_range_max;

        /*
        a "membrane" is any thin region of skin streched across segments
        It can serve as a way to increase surface area for heat dissipation, swimming, flying, or startle displays.
        Examples include flying squirrel membranes, fish fins, dimetrodon fins, and bat wings.
        Terminology:
        muscle insertion:   region along the segment at which the muscle attaches to the bone or exoskeleton 
        muscle origin:      region along the segment's parent at which the muscle attaches to the bone or exoskeleton 
        */
        float membrane_origin;
        float membrane_insertion;
        float membrane_thickness;

        float homeotic_gene_disabled;

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            *output++ = encode_portion (length                                      );
            *output++ = encode_fraction(width                                       );
            *output++ = encode_fraction(height                                      );
            *output++ = encode_fraction(endoskeleton_radius                         );
            *output++ = encode_fraction(exoskeleton_thickness                       );
            *output++ = encode_fraction(extensor_muscle_relaxed_radius              );
            *output++ = encode_fraction(extensor_muscle_insertion                   );
            *output++ = encode_fraction(extensor_muscle_origin                      );
            *output++ = encode_fraction(extensor_to_flexor_muscle_ratio             );
            *output++ = encode_fraction(hydraulic_closed_system_vessel_radius       );
            *output++ = encode_fraction(hydraulic_closed_system_vessel_thickness    );
            *output++ = encode_fraction(open_system_vessel_radius                   );
            *output++ = encode_fraction(open_system_vessel_thickness                );
            *output++ = encode_fraction(nonhydraulic_closed_system_vessel_radius    );
            *output++ = encode_fraction(nonhydraulic_closed_system_vessel_thickness );
            *output++ = encode_ranged  (joint_vertical_range_min,  -M_PI, M_PI      );
            *output++ = encode_ranged  (joint_vertical_range_max,  -M_PI, M_PI      );
            *output++ = encode_ranged  (joint_horizontal_range_min,-M_PI, M_PI      );
            *output++ = encode_ranged  (joint_horizontal_range_max,-M_PI, M_PI      );
            *output++ = encode_fraction(membrane_origin                             );
            *output++ = encode_fraction(membrane_insertion                          );
            *output++ = encode_fraction(membrane_thickness                          );
            *output++ = encode_fraction(homeotic_gene_disabled                      );
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            length                                      = decode_portion (*input++ );
            width                                       = decode_fraction(*input++ );
            height                                      = decode_fraction(*input++ );
            endoskeleton_radius                         = decode_fraction(*input++ );
            exoskeleton_thickness                       = decode_fraction(*input++ );
            extensor_muscle_relaxed_radius              = decode_fraction(*input++ );
            extensor_muscle_insertion                   = decode_fraction(*input++ );
            extensor_muscle_origin                      = decode_fraction(*input++ );
            extensor_to_flexor_muscle_ratio             = decode_fraction(*input++ );
            hydraulic_closed_system_vessel_radius       = decode_fraction(*input++ );
            hydraulic_closed_system_vessel_thickness    = decode_fraction(*input++ );
            open_system_vessel_radius                   = decode_fraction(*input++ );
            open_system_vessel_thickness                = decode_fraction(*input++ );
            nonhydraulic_closed_system_vessel_radius    = decode_fraction(*input++ );
            nonhydraulic_closed_system_vessel_thickness = decode_fraction(*input++ );
            joint_vertical_range_min                    = decode_ranged  (*input++, -M_PI, M_PI );
            joint_vertical_range_max                    = decode_ranged  (*input++, -M_PI, M_PI );
            joint_horizontal_range_min                  = decode_ranged  (*input++, -M_PI, M_PI );
            joint_horizontal_range_max                  = decode_ranged  (*input++, -M_PI, M_PI );
            membrane_origin                             = decode_fraction(*input++ );
            membrane_insertion                          = decode_fraction(*input++ );
            membrane_thickness                          = decode_fraction(*input++ );
            homeotic_gene_disabled                      = decode_fraction(*input++ );
            return input;
        }

        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            return std::fill_n(output, 22, 1);
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            return std::fill_n(output, 22, 4);
        }
        static constexpr unsigned int bit_count = 22*4;
        static constexpr unsigned int attribute_count = 22;

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
    };
}
