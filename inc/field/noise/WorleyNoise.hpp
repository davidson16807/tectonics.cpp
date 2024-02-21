#pragma once

// C libraries

// std libraries

// 3rd-party libraries
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <grid/cartesian/BoundedIndexing.hpp>

namespace field
{

	/*
	Given a `MosaicVectorNoise`: ℕ→ℝᴸ, `WorleyNoise` maps ℝᴸ→ℝ. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `MosaicVectorNoise`.
	*/
	template<int L, typename scalar, typename MosaicVectorNoise, typename MosaicOps, glm::qualifier precision=glm::defaultp>
	struct WorleyNoise
	{
		MosaicVectorNoise noise;
		MosaicOps ops;
	    cartesian::BoundedIndexing<L,int,precision> indexing;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit WorleyNoise(const MosaicVectorNoise& noise, const MosaicOps& ops): 
			noise(noise), 
			ops(ops),
			indexing(3) 
		{}

		WorleyNoise(const WorleyNoise& worley): 
			noise(worley.noise), 
			ops(worley.ops),
			indexing(3) 
		{}

		using vec = glm::vec<L,scalar,precision>;
		using ivec = glm::vec<L,int,precision>;
		using vector_type = typename MosaicVectorNoise::value_type;
		using value_type = typename vector_type::value_type;

		template<typename tpoint>
		value_type operator()(const tpoint V) const {
		    scalar nearest_distance = std::numeric_limits<scalar>::max();
		    auto I = ops.floor(V);
		    for (int i = 0; i < indexing.size; ++i)
		    {
                auto neighbor_id = ops.add(I, indexing.grid_id(i)-1);
                auto neighbor_position = ops.add(neighbor_id, noise(neighbor_id));
                nearest_distance = std::min(nearest_distance, ops.distance(V, neighbor_position));
		    }

		    return nearest_distance;
		}
	};

	template<int L, typename scalar, typename MosaicVectorNoise, typename MosaicOps, glm::qualifier precision=glm::defaultp>
	constexpr inline auto worley_noise(const MosaicVectorNoise& noise, const MosaicOps& ops)
	{
		return WorleyNoise<L,scalar,MosaicVectorNoise,MosaicOps,precision>(noise, ops);
	}

}

