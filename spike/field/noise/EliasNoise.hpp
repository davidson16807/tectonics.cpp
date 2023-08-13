#pragma once

// C libraries

// std libraries

// 3rd-party libraries
#include <glm/common.hpp>
#include <glm/geometric.hpp>

namespace field
{

	/*
	`EliasNoise` implements the terrain generation algorithm discussed by
    Hugo Elias here: http://freespace.virgin.net/hugo.elias/models/m_landsp.htm
	It maps ℝ³→𝕊→[0,1] where 𝕊 indicates the unit spherical domain.
    The algorithm was originally designed for terrain generation on a sphere,
     but it works for any scalar field on the surface of a spheroid. 
    It does this by iteratively incrementing values on grid cells that fall within a random region on a unit sphere.
    Regions are defined by whether consine similarity to a random point on a unit sphere falls within a random threshold.
    Regions may have smooth boundaries, depending on a "region_transition_width" parameter that defines
     the width of a region's transition zone. 
    `RegionThreshold` and `RegionCenters` may be any container that contains scalars and vectors (respectively),
    but for data locality concerns, we highly recommend using `series::UnitIntervalNoise<scalar>` 
    and `series::NormalizedVectorInterleave<Noise<scalar>>`.
	*/
	template<typename T, typename RegionThreshold, typename RegionCenter>
	struct EliasNoise
	{
		RegionCenter region_center;
		RegionThreshold region_threshold;
		T region_transition_width;
		uint region_count;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit EliasNoise(
			const RegionCenter& region_center,
			const RegionThreshold& region_threshold,
			const uint region_count, 
			const T region_transition_width=T(0.03)
		):
			region_center(region_center),
			region_threshold(region_threshold),
			region_transition_width(region_transition_width),
			region_count(region_count)
		{}

		using value_type = T;

		template<int L, glm::qualifier Q>
		T operator()(const glm::vec<L,T,Q> position) const {
			glm::vec<L,T,Q> normalized(glm::normalize(position));
			T region_mod(0);
			T out(0);
			T region_threshold_i;
			for (uint i = 0; i < region_count; ++i)
			{
				region_mod = glm::dot(normalized, region_center[i]);
				region_threshold_i = region_threshold[i];
				glm::smoothstep(
					region_threshold_i - region_transition_width/T(2), 
					region_threshold_i + region_transition_width/T(2),  
					region_mod);
				out += region_mod;
			}
			return out;
		}
	};

	template<typename T, typename RegionThreshold, typename RegionCenter>
	constexpr inline auto elias_noise(
		const RegionCenter& region_center,
		const RegionThreshold& region_threshold,
		const uint region_count, 
		const T region_transition_width=T(0.03))
	{
		return EliasNoise<T, RegionThreshold,RegionCenter>(region_center, region_threshold, region_count, region_transition_width);
	}

}

