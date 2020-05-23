#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace genetics
{
    /*
    A "body segment" refers to one of the compartmentalized regions 
    of a segmented, motile, multicellular organism.
    For brevity, we will call such an organism an "animal",
    even though the term is more restrictive in some ways (suggesting heterotrophism)
    and less restrictive in others (allowing non-segmented or non-motile organisms)
    
    The `GenericSegmentGenotype` data structure is designed for use in a genetic algorithm. 
    To aid in that use case, attributes must be designed in such a way that 
    it is impossible for there to exist a combination of attribute values
    that could not represent an animal in the real world, 
    i.e. there are no illegal regions of state space. s.

    Also to support use in a genetic algorithm, the `GenericSegmentGenotype` data structure is 
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
    
    The `GenericSegmentGenotype` data structure is designed to describe as many organisms as possible.
    It generally tries to support any body plan that could be seen in vertebrates and arthropods. 
    This results in some design choices that might at first not seem obvious: 
    * It allows one heart to exist per segment, since we see this body plan in insects.
    * 


    The `GenericSegmentGenotype` data structure is designed to be memory efficient and safe to modify
    Unfortunately, this makes it difficult to work with.
    To make it easier to work with, we transform it to other data structures.
    */

    // 24 nybbles, 12 bytes, 3 words, same size as a vec3
    struct GenericSegmentGenotype
    {

        // 3 nybbles: basic dimensions
        std::uint8_t:4 length;
        std::uint8_t:4 width;
        std::uint8_t:4 height;

        // 2 nybbles: support structures
        std::uint8_t:4  endoskeleton_radius;
        std::uint8_t:4  exoskeleton_thickness;

        // 4 nybbles: musculature
        /*
        Terminology:
        extensor muscle:    a set of muscles that pull the segment towards the segment's parent
        flexor muscle:      a set of muscles that pull the segment away from the segment's parent
        muscle insertion:   region along the segment at which the muscle attaches to the bone or exoskeleton 
        muscle origin:      region along the segment's parent at which the muscle attaches to the bone or exoskeleton 
        */
        std::uint8_t:4  extensor_muscle_relaxed_radius;
        std::uint8_t:4  extensor_muscle_insertion;
        std::uint8_t:4  extensor_muscle_origin;
        std::uint8_t:4  extensor_to_flexor_muscle_ratio;

        // 8 nybbles: vessels
        /*
        "vessels" are any tubular structure used for fluid transport or hydraulics
        extensor/flexor vessels are lymphohydraulic
        nonhydraulic vessels may serve as blood vessels or lymph vessels, depending on 
        Its functions include nutrient transport, waste removal, temperature regulation, hydraulic motion, and pneumatic motion
        */
        std::uint8_t:4  extensor_closed_system_vessel_radius;
        std::uint8_t:4  extensor_closed_system_vessel_thickness;
        std::uint8_t:4  extensor_to_flexor_closed_system_vessel_ratio;

        std::uint8_t:4  extensor_open_system_vessel_max_radius;
        std::uint8_t:4  extensor_open_system_vessel_max_thickness;
        std::uint8_t:4  extensor_to_flexor_open_system_vessel_ratio;

        std::uint8_t:4  nonhydraulic_closed_system_vessel_radius;
        std::uint8_t:4  nonhydraulic_closed_system_vessel_thickness;

        // 4 nybbles: joint ranges
        // All attributes here describe the motion of the joint that connects the segment to its *parent*
        std::uint8_t:4  joint_vertical_range_min;
        std::uint8_t:4  joint_vertical_range_max;
        std::uint8_t:4  joint_horizontal_range_min;
        std::uint8_t:4  joint_horizontal_range_max;

        // 3 nybbles: hard structures
        /*
        a "membrane" is any thin region of skin streched across segments
        It can serve as a way to increase surface area for heat dissipation, swimming, flying, or startle displays.
        Examples include flying squirrel membranes, fish fins, dimetrodon fins, and bat wings.
        Terminology:
        muscle insertion:   region along the segment at which the muscle attaches to the bone or exoskeleton 
        muscle origin:      region along the segment's parent at which the muscle attaches to the bone or exoskeleton 
        */
        std::uint8_t:4  membrane_origin;
        std::uint8_t:4  membrane_insertion;
        std::uint8_t:4  membrane_thickness;


    private:
        // Represent pressures up to 6e12 Pascals with a precision of 12%.
        // This value was chosen to reflect the pressure of Jupiter's core. 
        constexpr float LOG_REF_TEMPERATURE = 17.;
        // Represent temperatures up to 32768 Kelvin with a precision of 12%.
        // This value was chosen to reflect the temperature of Jupiter's core. 
        constexpr float LOG_REF_PRESSURE    = 6.;
        // Represent grain sizes down to 1μm with a precision of 12%.
        // This value was chosen to reflect the size of clay particles. 
        constexpr float LOG_REF_GRAIN_SIZE  = 13.;

        constexpr float TEMPERATURE_MAX = exp2(255./LOG_REF_TEMPERATURE);
        constexpr float PRESSURE_MAX    = exp2(255./LOG_REF_PRESSURE);
        constexpr float GRAIN_SIZE_MAX  = exp2(-0. /LOG_REF_GRAIN_SIZE);

        constexpr float TEMPERATURE_MIN = exp2(0./LOG_REF_TEMPERATURE);
        constexpr float PRESSURE_MIN    = exp2(0./LOG_REF_PRESSURE);
        constexpr float GRAIN_SIZE_MIN  = exp2(-255./LOG_REF_GRAIN_SIZE);

    public:
        ~GenericSegmentGenotype()
        {
        }

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
        explicit GenericSegmentGenotype():
            length(),
            width(),
            height(),
            endoskeleton_radius(),
            exoskeleton_thickness(),
            extensor_muscle_relaxed_radius(),
            extensor_muscle_insertion(),
            extensor_muscle_origin(),
            extensor_to_flexor_muscle_ratio(),
            extensor_closed_system_vessel_radius(),
            extensor_closed_system_vessel_thickness(),
            extensor_to_flexor_closed_system_vessel_ratio(),
            extensor_open_system_vessel_max_radius(),
            extensor_open_system_vessel_max_thickness(),
            extensor_to_flexor_open_system_vessel_ratio(),
            nonhydraulic_closed_system_vessel_radius(),
            nonhydraulic_closed_system_vessel_thickness(),
            joint_vertical_range_min(),
            joint_vertical_range_max(),
            joint_horizontal_range_min(),
            joint_horizontal_range_max(),
            membrane_origin(),
            membrane_insertion(),
            membrane_thickness(),
        {
        }
        explicit GenericSegmentGenotype(const GenericSegmentPhenotype& phenotype, ) :
            length                                       (std::uint8_t(std::clamp(phenotype.length                                       /16.,  0., 16.))),
            width                                        (std::uint8_t(std::clamp(phenotype.width                                        /16.,  0., 16.))),
            height                                       (std::uint8_t(std::clamp(phenotype.height                                       /16.,  0., 16.))),
            endoskeleton_radius                          (std::uint8_t(std::clamp(phenotype.endoskeleton_radius                          /16.,  0., 16.))),
            exoskeleton_thickness                        (std::uint8_t(std::clamp(phenotype.exoskeleton_thickness                        /16.,  0., 16.))),
            extensor_muscle_relaxed_radius               (std::uint8_t(std::clamp(phenotype.extensor_muscle_relaxed_radius               /16.,  0., 16.))),
            extensor_muscle_insertion                    (std::uint8_t(std::clamp(phenotype.extensor_muscle_insertion                    /16.,  0., 16.))),
            extensor_muscle_origin                       (std::uint8_t(std::clamp(phenotype.extensor_muscle_origin                       /16.,  0., 16.))),
            extensor_to_flexor_muscle_ratio              (std::uint8_t(std::clamp(phenotype.extensor_to_flexor_muscle_ratio              /16.,  0., 16.))),
            extensor_closed_system_vessel_radius         (std::uint8_t(std::clamp(phenotype.extensor_closed_system_vessel_radius         /16.,  0., 16.))),
            extensor_closed_system_vessel_thickness      (std::uint8_t(std::clamp(phenotype.extensor_closed_system_vessel_thickness      /16.,  0., 16.))),
            extensor_to_flexor_closed_system_vessel_ratio(std::uint8_t(std::clamp(phenotype.extensor_to_flexor_closed_system_vessel_ratio/16.,  0., 16.))),
            extensor_open_system_vessel_max_radius       (std::uint8_t(std::clamp(phenotype.extensor_open_system_vessel_max_radius       /16.,  0., 16.))),
            extensor_open_system_vessel_max_thickness    (std::uint8_t(std::clamp(phenotype.extensor_open_system_vessel_max_thickness    /16.,  0., 16.))),
            extensor_to_flexor_open_system_vessel_ratio  (std::uint8_t(std::clamp(phenotype.extensor_to_flexor_open_system_vessel_ratio  /16.,  0., 16.))),
            nonhydraulic_closed_system_vessel_radius     (std::uint8_t(std::clamp(phenotype.nonhydraulic_closed_system_vessel_radius     /16.,  0., 16.))),
            nonhydraulic_closed_system_vessel_thickness  (std::uint8_t(std::clamp(phenotype.nonhydraulic_closed_system_vessel_thickness  /16.,  0., 16.))),
            joint_vertical_range_min                     (std::uint8_t(std::clamp(phenotype.joint_vertical_range_min                     /16.,  0., 16.))),
            joint_vertical_range_max                     (std::uint8_t(std::clamp(phenotype.joint_vertical_range_max                     /16.,  0., 16.))),
            joint_horizontal_range_min                   (std::uint8_t(std::clamp(phenotype.joint_horizontal_range_min                   /16.,  0., 16.))),
            joint_horizontal_range_max                   (std::uint8_t(std::clamp(phenotype.joint_horizontal_range_max                   /16.,  0., 16.))),
            membrane_origin                              (std::uint8_t(std::clamp(phenotype.membrane_origin                              /16.,  0., 16.))),
            membrane_insertion                           (std::uint8_t(std::clamp(phenotype.membrane_insertion                           /16.,  0., 16.))),
            membrane_thickness                           (std::uint8_t(std::clamp(phenotype.membrane_thickness                           /16.,  0., 16.))),
        {
        }
        void decompress(GenericSegmentPhenotype& output)
        {
            output.length                                        = std::uint8_t(std::clamp(length                                       /16.,  0., 16.));
            output.width                                         = std::uint8_t(std::clamp(width                                        /16.,  0., 16.));
            output.height                                        = std::uint8_t(std::clamp(height                                       /16.,  0., 16.));
            output.endoskeleton_radius                           = std::uint8_t(std::clamp(endoskeleton_radius                          /16.,  0., 16.));
            output.exoskeleton_thickness                         = std::uint8_t(std::clamp(exoskeleton_thickness                        /16.,  0., 16.));
            output.extensor_muscle_relaxed_radius                = std::uint8_t(std::clamp(extensor_muscle_relaxed_radius               /16.,  0., 16.));
            output.extensor_muscle_insertion                     = std::uint8_t(std::clamp(extensor_muscle_insertion                    /16.,  0., 16.));
            output.extensor_muscle_origin                        = std::uint8_t(std::clamp(extensor_muscle_origin                       /16.,  0., 16.));
            output.extensor_to_flexor_muscle_ratio               = std::uint8_t(std::clamp(extensor_to_flexor_muscle_ratio              /16.,  0., 16.));
            output.extensor_closed_system_vessel_radius          = std::uint8_t(std::clamp(extensor_closed_system_vessel_radius         /16.,  0., 16.));
            output.extensor_closed_system_vessel_thickness       = std::uint8_t(std::clamp(extensor_closed_system_vessel_thickness      /16.,  0., 16.));
            output.extensor_to_flexor_closed_system_vessel_ratio = std::uint8_t(std::clamp(extensor_to_flexor_closed_system_vessel_ratio/16.,  0., 16.));
            output.extensor_open_system_vessel_max_radius        = std::uint8_t(std::clamp(extensor_open_system_vessel_max_radius       /16.,  0., 16.));
            output.extensor_open_system_vessel_max_thickness     = std::uint8_t(std::clamp(extensor_open_system_vessel_max_thickness    /16.,  0., 16.));
            output.extensor_to_flexor_open_system_vessel_ratio   = std::uint8_t(std::clamp(extensor_to_flexor_open_system_vessel_ratio  /16.,  0., 16.));
            output.nonhydraulic_closed_system_vessel_radius      = std::uint8_t(std::clamp(nonhydraulic_closed_system_vessel_radius     /16.,  0., 16.));
            output.nonhydraulic_closed_system_vessel_thickness   = std::uint8_t(std::clamp(nonhydraulic_closed_system_vessel_thickness  /16.,  0., 16.));
            output.joint_vertical_range_min                      = std::uint8_t(std::clamp(joint_vertical_range_min                     /16.,  0., 16.));
            output.joint_vertical_range_max                      = std::uint8_t(std::clamp(joint_vertical_range_max                     /16.,  0., 16.));
            output.joint_horizontal_range_min                    = std::uint8_t(std::clamp(joint_horizontal_range_min                   /16.,  0., 16.));
            output.joint_horizontal_range_max                    = std::uint8_t(std::clamp(joint_horizontal_range_max                   /16.,  0., 16.));
            output.membrane_origin                               = std::uint8_t(std::clamp(membrane_origin                              /16.,  0., 16.));
            output.membrane_insertion                            = std::uint8_t(std::clamp(membrane_insertion                           /16.,  0., 16.));
            output.membrane_thickness                            = std::uint8_t(std::clamp(membrane_thickness                           /16.,  0., 16.));
        }
    };
}
