#pragma once

// C libraries

// std libraries

// 3rd-party libraries
#include <glm/vector_relational.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/component_wise.hpp>

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

		using ivector = glm::vec<L,id,precision>;
		using vector = glm::vec<L,scalar,precision>;

		inline ivector floor(const vector& grid_id) const
		{
			return ivector(glm::floor(grid_id));
		}

		inline vector fract(const vector& grid_id) const
		{
			return glm::fract(grid_id);
		}

		inline auto add(const ivector& grid_id, const vector& offset) const
		{
			return vector(grid_id)+vector(offset);
		}

		inline auto add(const ivector& grid_id, const ivector& offset) const
		{
			return grid_id+offset;
		}

		inline scalar distance(const vector& grid_id1, const vector& grid_id2) const
		{
			return glm::distance(grid_id1, grid_id2);
		}

		template<typename scalar2>
		inline scalar weight(const glm::vector<L,scalar2,precision>& position, const ivector& origin, const ivector& offset) const
		{
			vector F = fract(position);
			vector F01 = glm::smoothstep(vector(0), vector(1), F);
			return glm::compMul(glm::mix(vector(1)-F01, F01, vector(offset)));
		}

	};

	template<int L, typename id, typename scalar, glm::qualifier precision=glm::defaultp>
	constexpr inline auto vector_mosaic_ops()
	{
		return VectorMosaicOps<L,id,scalar,precision>();
	}

}

