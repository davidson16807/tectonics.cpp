#pragma once

// C libraries

// std libraries

// 3rd-party libraries
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/noise.hpp>

namespace field
{

	/*
	Given a `MosaicVectorNoise`: ℕ→ℝ⁴, `PerlinNoise4` maps ℝ⁴→ℝ. 
	For each point, `PerlinNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `MosaicVectorNoise`.
	*/
	template<typename MosaicVectorNoise>
	struct PerlinNoise4
	{
		MosaicVectorNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit PerlinNoise4(const MosaicVectorNoise& noise): noise(noise), ops(ops) {}
		PerlinNoise4(const PerlinNoise4& perlin): noise(perlin.noise), ops(perlin.ops) {}

		using vector_type = typename MosaicVectorNoise::value_type;
		using value_type = typename vector_type::value_type;

		value_type operator()(const auto position) const {
			using vec = glm::vec<4,T,Q>;
		    vec V(position);
		    vec I = ops.floor(V);
		    vec F = ops.fract(V);
		    vec F01 = glm::smoothstep(vec(0), vec(1), F);
		    vec G(0);
		    vec O(0);
		    value_type a(0);
		    for (int i = 0; i <= 1; ++i)
		    {
		        for (int j = 0; j <= 1; ++j)
		        {
		            for (int k = 0; k <= 1; ++k)
		            {
		                for (int l = 0; l <= 1; ++l)
		                {
		                    O = vec(i,j,k,l);
		                    G = glm::mix(vec(1)-F01, F01, O);
		                    a += glm::dot(glm::normalize(noise(ops.add(I,O))), F-O) 
		                    	* G.x * G.y * G.z * G.w;
		                }
		            }
		        }
		    }
		    return a;
		}
	};

	template<typename MosaicVectorNoise>
	constexpr inline auto perlin_noise4(const MosaicVectorNoise noise)
	{
		return PerlinNoise4<MosaicVectorNoise>(noise);
	}

	/*
	Given a `MosaicVectorNoise`: ℕ→ℝ³, `PerlinNoise3` maps ℝ³→ℝ. 
	For each point, `PerlinNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `MosaicVectorNoise`.
	*/
	template<typename MosaicVectorNoise>
	struct PerlinNoise3
	{
		MosaicVectorNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit PerlinNoise3(const MosaicVectorNoise& noise): noise(noise), ops(ops) {}
		PerlinNoise3(const PerlinNoise3& perlin): noise(perlin.noise), ops(perlin.ops) {}

		using vector_type = typename MosaicVectorNoise::value_type;
		using value_type = typename vector_type::value_type;

		value_type operator()(const auto position) const {
			using vec = glm::vec<3,T,Q>;
		    vec V(position);
		    vec I = ops.floor(V);
		    vec F = ops.fract(V);
		    vec F01 = glm::smoothstep(vec(0), vec(1), F);
		    vec G(0);
		    vec O(0);
		    value_type a(0);
		    for (int i = 0; i <= 1; ++i)
		    {
		        for (int j = 0; j <= 1; ++j)
		        {
		            for (int k = 0; k <= 1; ++k)
		            {
	                    O = vec(i,j,k);
	                    G = glm::mix(vec(1)-F01, F01, O);
	                    a += glm::dot(glm::normalize(noise(ops.add(I,O))), F-O) 
	                    	* G.x * G.y * G.z;
		            }
		        }
		    }
		    return a;
		}
	};

	template<typename MosaicVectorNoise>
	constexpr inline auto perlin_noise3(const MosaicVectorNoise noise)
	{
		return PerlinNoise3<MosaicVectorNoise>(noise);
	}

	/*
	Given a `MosaicVectorNoise`: ℕ→ℝ², `PerlinNoise2` maps ℝ²→ℝ. 
	For each point, `PerlinNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `MosaicVectorNoise`.
	*/
	template<typename MosaicVectorNoise>
	struct PerlinNoise2
	{
		MosaicVectorNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit PerlinNoise2(const MosaicVectorNoise& noise): noise(noise), ops(ops) {}
		PerlinNoise2(const PerlinNoise2& perlin): noise(perlin.noise), ops(perlin.ops) {}

		using vector_type = typename MosaicVectorNoise::value_type;
		using value_type = typename vector_type::value_type;

		value_type operator()(const auto V) const {
			using vec = glm::vec<2,T,Q>;
		    auto I = ops.floor(V);
		    vec F = ops.fract(V);
		    vec F01 = glm::smoothstep(vec(0), vec(1), F);
		    vec G(0);
		    vec O(0);
		    value_type a(0);
		    for (int i = 0; i <= 1; ++i)
		    {
		        for (int j = 0; j <= 1; ++j)
		        {
                    O = vec(i,j);
                    G = glm::mix(vec(1)-F01, F01, O);
                    a += glm::dot(glm::normalize(noise(ops.add(I,O))), F-O) 
                    	* G.x * G.y;
		        }
		    }
		    return a;
		}
	};

	template<typename MosaicVectorNoise>
	constexpr inline auto perlin_noise2(const MosaicVectorNoise noise)
	{
		return PerlinNoise2<MosaicVectorNoise>(noise);
	}

}

