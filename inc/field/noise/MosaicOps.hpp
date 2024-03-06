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

		using ivec = glm::vec<L,id,precision>;
		using vec = glm::vec<L,scalar,precision>;

		inline ivec floor(const vec grid_id) const
		{
			return ivec(glm::floor(grid_id));
		}

		inline vec fract(const vec grid_id) const
		{
			return glm::fract(grid_id);
		}

		inline auto add(const ivec grid_id, vec offset) const
		{
			return vec(grid_id)+vec(offset);
		}

		inline auto add(const ivec grid_id, ivec offset) const
		{
			return grid_id+offset;
		}

		inline scalar distance(const vec grid_id1, vec grid_id2) const
		{
			return glm::distance(grid_id1, grid_id2);
		}

		template<typename T>
		inline scalar weight(const glm::vec<L,T,precision> position, const ivec origin, ivec offset) const
		{
			vec2 F = fract(position);
			vec2 F01 = glm::smoothstep(vec2(0), vec2(1), F);
			return math::prod(glm::mix(vec2(1)-F01, F01, vec2(offset)));
		}

	};

	template<int L, typename id, typename scalar, glm::qualifier precision=glm::defaultp>
	constexpr inline auto vector_mosaic_ops()
	{
		return VectorMosaicOps<L,id,scalar,precision>();
	}

}

