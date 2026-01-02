#pragma once

// C libraries

// std libraries

// 3rd-party libraries
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/component_wise.hpp>

// in-house libraries
#include <grid/cartesian/OrthantIndexing.hpp>

namespace field
{

	/*
	Given a `MosaicVectorNoise`: ℕ→ℝ⁴, `PerlinNoise` maps ℝ⁴→ℝ. 
	For each point, `PerlinNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `MosaicVectorNoise`.
	*/
	template<int L, typename scalar, typename MosaicVectorNoise, typename MosaicOps, glm::qualifier precision=glm::defaultp>
	struct PerlinNoise
	{
		using vector = glm::vec<L,scalar,precision>;

		MosaicVectorNoise noise;
		MosaicOps ops;
	    cartesian::OrthantIndexing<L,int,precision> indexing;

	public:

		using vector_type = typename MosaicVectorNoise::value_type;
		using value_type = scalar;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit PerlinNoise(
			const MosaicVectorNoise& noise, 
			const MosaicOps& ops
		): 
			noise(noise), 
			ops(ops),
			indexing()
		{}

		PerlinNoise(const PerlinNoise& perlin): 
			noise(perlin.noise), 
			ops(perlin.ops),
			indexing() 
		{}

		template<typename vector2>
		scalar operator()(const vector2& V) const {
		    auto I = ops.floor(V);
		    vector F = ops.fract(V);
		    scalar f(0);
		    scalar weight(0);
		    scalar weight_total(0);
		    for (int i = 0; i < indexing.size; ++i)
		    {
                auto O = indexing.grid_id(i);
                weight = ops.weight(V,I,O);
                weight_total += weight;
                f += glm::dot(glm::normalize(noise(ops.add(I,O))), F-vector(O)) * weight;
		    }
		    return f / weight_total;
		}
	};

	template<int L, typename scalar, typename MosaicVectorNoise, typename MosaicOps, glm::qualifier precision=glm::defaultp>
	constexpr inline auto perlin_noise(const MosaicVectorNoise noise, const MosaicOps ops)
	{
		return PerlinNoise<L, scalar,MosaicVectorNoise,MosaicOps,precision>(noise, ops);
	}

}

