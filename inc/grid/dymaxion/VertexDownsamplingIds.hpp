#pragma once

// C libraries
#include <cmath>     /* std::floor */

// std libraries
#include "Indexing.hpp"

namespace dymaxion
{

	/*
	`VertexDownsamplingIds` maps each memory id of a dymaxion::Grid to those of another dymaxion::Grid of coarser resolution.
	Unlike most other grids, downsampling for `dymaxion::Grid`s strictly uses in-order traversal!
	*/

	template<typename id, typename id2, typename scalar> 
	struct VertexDownsamplingIds
	{ 
		const Indexing<id,id2,scalar> fine; 
		const Indexing<id,id2,scalar> coarse; 
		const id factor;
		constexpr inline explicit VertexDownsamplingIds(
			const Indexing<id,id2,scalar> fine,
			const Indexing<id,id2,scalar> coarse
		): 
			fine(fine),
			coarse(coarse),
			factor(fine.vertices_per_square_side/coarse.vertices_per_square_side)
		{} 
	    using size_type = std::size_t; 
		using value_type = id2; 
		constexpr inline size_type size() const { return fine.vertex_count; } 
		constexpr value_type call(const size_type memory_id) const 
		{
			return coarse.memory_id(
				fine.grid_id(memory_id)/factor
			);
		} 
		constexpr inline value_type operator()(const size_type memory_id ) const { return call(memory_id); } 
		constexpr inline value_type operator[](const size_type memory_id ) const { return call(memory_id); } 
	};
	template<typename id, typename id2, typename scalar>
	constexpr inline VertexDownsamplingIds<id,id2,scalar> downsampling(const Indexing<id,id2,scalar> coarse)
	{
		return VertexDownsamplingIds<id,id2,scalar>(coarse);
	}

}

