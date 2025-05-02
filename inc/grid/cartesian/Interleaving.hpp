#pragma once

namespace cartesian
{

	/*
	"Interleaving" reflects an invertible function between a tuple of integers and a 1d integer that represents them both.
	It is useful for instance to condense 2d arrays down to 1d arrays.
	*/
    template<typename id>
	class Interleaving
	{
        const id elements_per_block;

	public:

		explicit Interleaving(const id elements_per_block):
            elements_per_block(elements_per_block)
        {

        }

		id interleaved_id(const id block_id, const id element_id) const {
			return block_id * elements_per_block + element_id;
		}

		id block_id(const id interleaved_id) const {
			return interleaved_id / elements_per_block;
		}

		id element_id(const id interleaved_id) const {
			return interleaved_id % elements_per_block;
		}

		id element_count(const id block_count) const {
			return block_count * elements_per_block;
		}

	};
}