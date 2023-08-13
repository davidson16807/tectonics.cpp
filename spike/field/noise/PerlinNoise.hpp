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
	Given a `SquareNoise`: ℕ→ℝ⁴, `PerlinNoise4` maps ℝ⁴→ℝ. 
	For each point, `PerlinNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `SquareNoise`.
	*/
	template<typename SquareNoise>
	struct PerlinNoise4
	{
		SquareNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit PerlinNoise4(const SquareNoise& noise): noise(noise) {}
		PerlinNoise4(const PerlinNoise4& perlin): noise(perlin.noise) {}

		using value_type = typename SquareNoise::value_type;

		template<typename T, glm::qualifier Q>
		value_type operator()(const glm::vec<4,T,Q> position) const {
			using vec = glm::vec<4,T,Q>;
		    vec V(position);
		    vec I = glm::floor(V);
		    vec F = glm::smoothstep(vec(0), vec(1), V-I);
		    vec G(0);
		    vec O(0);
		    vec J(0);
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
		                    J = I + O;
		                    G = glm::mix(vec(1)-F, F, O);
		                    a += glm::dot(glm::normalize(noise(J)-vec(0.5)), V-J) * G.x * G.y * G.z * G.w;
		                }
		            }
		        }
		    }
		    return a;
		}
	};

	template<typename SquareNoise>
	constexpr inline auto perlin_noise4(const SquareNoise noise)
	{
		return PerlinNoise4<SquareNoise>(noise);
	}

	/*
	Given a `SquareNoise`: ℕ→ℝ³, `PerlinNoise3` maps ℝ³→ℝ. 
	For each point, `PerlinNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `SquareNoise`.
	*/
	template<typename SquareNoise>
	struct PerlinNoise3
	{
		SquareNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit PerlinNoise3(const SquareNoise& noise): noise(noise) {}
		PerlinNoise3(const PerlinNoise3& perlin): noise(perlin.noise) {}

		using value_type = typename SquareNoise::value_type;

		template<typename T, glm::qualifier Q>
		value_type operator()(const glm::vec<3,T,Q> position) const {
			using vec = glm::vec<3,T,Q>;
		    vec V(position);
		    vec I = glm::floor(V);
		    vec F = glm::smoothstep(vec(0), vec(1), V-I);
		    vec G(0);
		    vec O(0);
		    vec J(0);
		    value_type a(0);
		    for (int i = 0; i <= 1; ++i)
		    {
		        for (int j = 0; j <= 1; ++j)
		        {
		            for (int k = 0; k <= 1; ++k)
		            {
	                    O = vec(i,j,k);
	                    J = I + O;
	                    G = glm::mix(vec(1)-F, F, O);
	                    a += glm::dot(glm::normalize(noise(J)-vec(0.5)), V-J) * G.x * G.y * G.z;
		            }
		        }
		    }
		    return a;
		}
	};

	template<typename SquareNoise>
	constexpr inline auto perlin_noise3(const SquareNoise noise)
	{
		return PerlinNoise3<SquareNoise>(noise);
	}

	/*
	Given a `SquareNoise`: ℕ→ℝ², `PerlinNoise2` maps ℝ²→ℝ. 
	For each point, `PerlinNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `SquareNoise`.
	*/
	template<typename SquareNoise>
	struct PerlinNoise2
	{
		SquareNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit PerlinNoise2(const SquareNoise& noise): noise(noise) {}
		PerlinNoise2(const PerlinNoise2& perlin): noise(perlin.noise) {}

		using value_type = typename SquareNoise::value_type;

		template<typename T, glm::qualifier Q>
		value_type operator()(const glm::vec<2,T,Q> position) const {
			using vec = glm::vec<2,T,Q>;
		    vec V(position);
		    vec I = glm::floor(V);
		    vec F = glm::smoothstep(vec(0), vec(1), V-I);
		    vec G(0);
		    vec O(0);
		    vec J(0);
		    value_type a(0);
		    for (int i = 0; i <= 1; ++i)
		    {
		        for (int j = 0; j <= 1; ++j)
		        {
                    O = vec(i,j);
                    J = I + O;
                    G = glm::mix(vec(1)-F, F, O);
                    a += glm::dot(glm::normalize(noise(J)-vec(0.5)), V-J) * G.x * G.y;
		        }
		    }
		    return a;
		}
	};

	template<typename SquareNoise>
	constexpr inline auto perlin_noise2(const SquareNoise noise)
	{
		return PerlinNoise2<SquareNoise>(noise);
	}

}

