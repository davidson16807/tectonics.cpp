#pragma once

// C libraries

// std libraries

// 3rd-party libraries
#include <glm/vector_relational.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/noise.hpp>

#include <grid/cartesian/UnboundedIndexing.hpp>

namespace field
{

	/*
	`VectorMosaicOps` considers operations on ℝ³ that are required for other kinds of noise functions 
	that depend on `MosaicNoise`, such as `ValueNoise`, `PerlinNoise`, etc.
	It is analogous to other possible classes that can be 
	used to extend these noise functions to operate on other domains.
	As such, it can be considered an implementation of the adapter pattern for vectors.
	*/
	template<int L, typename id, typename scalar, glm::qualifier precision=glm::defaultp>
	struct VectorMosaicOps
	{

		inline glm::vec<L,id,precision> floor(const glm::vec<L,scalar,precision> grid_id) const
		{
			return glm::vec<L,id,precision>(glm::floor(grid_id));
		}

		inline glm::vec<L,scalar,precision> fract(const glm::vec<L,scalar,precision> grid_id) const
		{
			return glm::fract(grid_id);
		}

		inline auto add(const glm::vec<L,id,precision> grid_id, glm::vec<L,scalar,precision> offset) const
		{
			return glm::vec<L,scalar,precision>(grid_id)+glm::vec<L,scalar,precision>(offset);
		}

		inline auto add(const glm::vec<L,id,precision> grid_id, glm::vec<L,id,precision> offset) const
		{
			return grid_id+offset;
		}

		inline scalar distance(const glm::vec<L,scalar,precision> grid_id1, glm::vec<L,scalar,precision> grid_id2) const
		{
			return glm::distance(grid_id1, grid_id2);
		}
	};

	template<int L, typename id, typename scalar, glm::qualifier precision=glm::defaultp>
	constexpr inline auto vector_mosaic_ops()
	{
		return VectorMosaicOps<L,id,scalar,precision>();
	}

}
