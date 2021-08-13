#pragma once

namespace rasters
{

	/*
	"Interleaving" reflects an invertible function between a tuple of integers and a 1d integer that represents them both.
	It is useful for instance to condense 2d arrays down to 1d arrays.
	*/
    template<typename Tid=std::uint16_t>
	class Interleaving
	{
        const Tid elements_per_block;

	public:
		~Interleaving()
		{
		}
		explicit Interleaving(const Tid elements_per_block):
            elements_per_block(elements_per_block)
        {

        }

		Tid interleaved_id(const Tid block_id, const Tid element_id) const {
			return block_id * elements_per_block + element_id;
		}

		Tid block_id(const Tid interleaved_id) const {
			return interleaved_id / elements_per_block;
		}
		Tid element_id(const Tid interleaved_id) const {
			return interleaved_id % elements_per_block;
		}


	};
}