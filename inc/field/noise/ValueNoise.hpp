#pragma once

// C libraries

// std libraries

// 3rd-party libraries
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/component_wise.hpp>

// in-house libraries
#include <math/glm/special.hpp>
#include <grid/cartesian/OrthantIndexing.hpp>

namespace field
{


	/*
	Given a `MosaicNoise`: 𝕋→ℝ, and `MosaicOps` that operate on 𝕋, `ValueNoise` maps 𝕋→ℝ. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `MosaicNoise`.
	*/
	template<int L, typename scalar, typename MosaicNoise, typename MosaicOps, glm::qualifier precision=glm::defaultp>
	struct ValueNoise
	{
		MosaicNoise noise;
		MosaicOps ops;
	    cartesian::OrthantIndexing<L,int,precision> indexing;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit ValueNoise(
			const MosaicNoise& noise,
			const MosaicOps& ops
		):
			noise(noise),
			ops(ops),
			indexing()
		{}

		ValueNoise(
			const ValueNoise& value
		):
			noise(value.noise),
			ops(value.ops),
			indexing()
		{}

		using vec = glm::vec<L,scalar,precision>;

		template<typename tpoint>
		scalar operator()(const tpoint V) const {
		    auto I = ops.floor(V);
		    vec F = ops.fract(V);
		    vec F01 = glm::smoothstep(vec(0), vec(1), F);
		    scalar f(0);
		    for (int i = 0; i < indexing.size; ++i)
		    {
                auto O = indexing.grid_id(i);
                f += noise(ops.add(I,O)) * math::prod(glm::mix(vec(1)-F01, F01, vec(O)));
		    }
		    return f;
		}
	};

	template<int L, typename scalar, typename MosaicNoise, typename MosaicOps, glm::qualifier precision=glm::defaultp>
	constexpr inline auto value_noise(const MosaicNoise noise, const MosaicOps ops)
	{
		return ValueNoise<L,scalar,MosaicNoise,MosaicOps,precision>(noise, ops);
	}

}

