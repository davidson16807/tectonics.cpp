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
        static constexpr unsigned int bit_count = 22*4;
        static constexpr unsigned int attribute_count = 22;
    };
}
