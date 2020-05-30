#pragma once

#include <models/genes/segments/GenericSegment.hpp>// encode_*(), decode_*()

namespace genes
{

    // 24 nybbles, 12 bytes, 3 words, same size as a vec3
    struct AppendageSegment: GenericSegment
    {
	    /*
	    Things we do not model on basic segments:
	    * suckers and newt's feet, because we lack clear understanding of what components go into it,
	      and any single attribute we create for it will have too little precision to reproduce evolutionary behavior
	    * ligament thickness, in the interest of space, since any value is likely inconsequential,
	      the user does not care, and the value would evolve to whatever was appropriate under a steady state assumption
	    * neuron bundle thickness, in the interest of space, since we assume whatever value is appropriate
	      the user does not care, and the value would evolve to whatever was appropriate under a steady state assumption
	    * sensing organs, in the interest of space, since they would virtually always evolve on the tips of appendages
	      see `Appendage` for how we model them
	    * fat thickness, despite possible user interest, because it does not fit easily into scaling rules, 
	      and attempting to model it inspite of scaling rules may cause unrealistic proportions 
	      where legs exceed the length of body segments and bleed into one another.
	      fat does not occur as much on appendages compared to the torso.
	      fat in our model serves to prevent heat loss so if cold adapted animals cannot acquire fat on their legs,
	      then their only other solution is to reduce surface area by shortening their legs.
	      this results in stumpy looking chubby animals that are cold tolerant, 
	      which is both consistent with reality and hilarious, not to mention adorable.
	    */
	    
        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            *output++ = encode_portion (length_as_portion_of_total                  );
            *output++ = encode_fraction(width_as_fraction_of_max                    );
            *output++ = encode_fraction(height_as_fraction_of_max                   );
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
            length_as_portion_of_total                  = decode_portion (*input++ );
            width_as_fraction_of_max                    = decode_fraction(*input++ );
            height_as_fraction_of_max                   = decode_fraction(*input++ );
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
    };
}
