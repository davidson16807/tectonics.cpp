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

	template<typename id, typename scalar> 
	struct VertexDownsamplingIds
	{ 
		const Indexing<id,scalar> fine; 
		const Indexing<id,scalar> coarse; 
		const int factor;
		constexpr inline explicit VertexDownsamplingIds(
			const Indexing<id,scalar> fine,
			const Indexing<id,scalar> coarse
		): 
			fine(fine),
			coarse(coarse),
			factor(fine.vertices_per_square_side/coarse.vertices_per_square_side)
		{} 
	    using size_type = std::size_t; 
		using value_type = id; 
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
	template<typename id, typename scalar>
	constexpr inline VertexDownsamplingIds<id,scalar> downsampling(const Indexing<id,scalar> coarse)
	{
		return VertexDownsamplingIds<id,scalar>(coarse);
	}

}

