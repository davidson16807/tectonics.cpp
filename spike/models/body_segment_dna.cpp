#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

namespace biomechanics
{
	/*
	A "body segment" refers to one of the compartmentalized regions 
	of a segmented, motile, multicellular organism.
	For brevity, we will call such an organism an "animal",
	even though the term is more restrictive in some ways (suggesting heterotrophism)
	and less restrictive in others (allowing non-segmented or non-motile organisms)
	
	The `body_segment_dna` data structure is designed for use in a genetic algorithm. 
	To aid in that use case, attributes must be designed in such a way that 
	it is impossible for there to exist a combination of attribute values
	that could not represent an animal in the real world, 
	i.e. there are no illegal regions of state space. 
	
	For example:
	* If a position must lie somewhere along a segment, it must be indicated by a fraction
	* If a constraint exists so that a part cannot be bigger than another, it must be indicated by a fraction
	* If a number must lie in a certain range, it must be represented using a function 
	  that transforms it to a number between -∞ and +∞, 
	  such as logit() for fractions, or log() for positive numbers.

	Also to support use in a genetic algorithm, the `body_segment_dna` data structure is 
	designed to mimic evolutionary tendencies seen in the real world.
	It makes it easy to develop some characteristics by allowing structures to serve multiple purposes.
	For instance: 
	* Wings are implied whenever a limb has appropriate dimensions. 
      We avoid Spore-style leg and arm part enums in favor of more generic, 
      more abstract real value measurements. 
      Sometimes this cannot always be done, as in the case of sensory organs.
	* "Horn mass" is used to represent any structural material along the surface, such as chitin, keratin.
	  It serves multiple functions just like we see in the real world, for biological ornament, combat, or digging.
	
	The `body_segment_dna` data structure is designed to describe as many animals as possible.
	It generally tries to support any body plan that could be seen in vertebrates and arthropods. 
	This results in some design choices that might at first not seem obvious: 
	* It allows one heart to exist per segment, since we see this body plan in insects.
	* 


	The `body_segment_dna` data structure is designed to be memory efficient and safe to modify
	Unfortunately, this makes it difficult to work with.
	To make it easier to work with, we transform it to a separate "body_segment" data structure.
	Unlike `body_segment_dna`, it is possible for the `body_segment` data structure to
	take on values that do not represent real world animals, 
	so we simply close off modification and prevent its conversion back to `body_segment_dna`

	so a commutative diagram of its morphisms is:
	body_segment_dna -> body_segment
	*/
	struct body_segment_dna
	{
		/*
		TERMINOLOGY:
		parent:             the segment that serves as a segment's parent node in a tree where the torso is the root node
		adductor muscle:    a set of muscles that pull the segment towards the segment's parent
		abductor muscle:    a set of muscles that pull the segment away from the segment's parent
		muscle insertion:   region along the segment at which the muscle attaches to the bone or exoskeleton 
		muscle origin:      region along the segment's parent at which the muscle attaches to the bone or exoskeleton 
		fraction of length: a fraction indicating a region along the length of a segment
		fraction of bone:   a fraction indicating a thickness relative to the maximum dimension of the bone
		logit:              the logit function, used to represent a number between between 0 and 1 using a float between -∞ and +∞
		log:                the natural logarithm, used to represent positive numbers using a float between -∞ and +∞
		membrane:           a portion of skin that stretches from one segment to another, forming wings or webbing
		*/
		float log_length;
		float endoskeleton_log_width;
		float endoskeleton_log_thickness;
		float adductor_muscle_log_thickness;
		float adductor_muscle_insertion_logit_fraction_of_length;
		float adductor_muscle_origin_logit_fraction_of_length;
		float abductor_muscle_log_thickness;
		float abductor_muscle_insertion_logit_fraction_of_length;
		float abductor_muscle_origin_logit_fraction_of_length;
		float membrane_origin_logit_fraction_of_length;
		float membrane_insertion_logit_fraction_of_length;
		float fat_log_mass;
		float heart_log_radius;    
		float heart_log_thickness; 
		float neuron_mass;    
		float exoskeleton_thickness;
		float hair_log_thickness;
		float horn_logit_fraction_of_length;
		float horn_logit_fraction_of_width;
		float horn_logit_fraction_of_thickness;
	};
	/*
	A "body segment" refers to one of the compartmentalized regions 
	of a segmented, motile, multicellular organism.
	For brevity, we will call such an organism an "animal",
	even though the term is more restrictive in some ways (suggesting heterotrophism)
	and less restrictive in others (allowing non-segmented or non-motile organisms)
	
	The `body_segment` class is designed to represent the same things 
	as a `body_segment_dna`, but in a way that makes it easier to use. 
	
	*/
	class body_segment
	{
		/*
		TERMINOLOGY:
		parent:             the segment that serves as a segment's parent node in a tree where the torso is the root node
		adductor muscle:    a set of muscles that pull the segment towards the segment's parent
		abductor muscle:    a set of muscles that pull the segment away from the segment's parent
		muscle insertion:   region along the segment at which the muscle attaches to the bone or exoskeleton 
		muscle origin:      region along the segment's parent at which the muscle attaches to the bone or exoskeleton 
		fraction of length: a fraction indicating a region along the length of a segment
		fraction of bone:   a fraction indicating a thickness relative to the maximum dimension of the bone
		logit:              the logit function, used to represent a number between between 0 and 1 using a float between -∞ and +∞
		membrane:           a portion of skin that stretches from one segment to another, forming wings or webbing
		*/
		float log_length;
		float endoskeleton_log_width;
		float endoskeleton_log_thickness;
		float adductor_muscle_log_thickness;
		float adductor_muscle_insertion_logit_fraction_of_length;
		float adductor_muscle_origin_logit_fraction_of_length;
		float abductor_muscle_log_thickness;
		float abductor_muscle_insertion_logit_fraction_of_length;
		float abductor_muscle_origin_logit_fraction_of_length;
		float membrane_origin_logit_fraction_of_length;
		float membrane_insertion_logit_fraction_of_length;
		float fat_log_mass;
		float heart_log_radius;
		float heart_log_thickness;
		float exoskeleton_thickness;
		~body_segment()
		{
		}
		explicit body_segment(const body_segment_dna& dna) :
		{
		}
	};
}
