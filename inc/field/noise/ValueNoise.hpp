#pragma once

// C libraries

// std libraries

// 3rd-party libraries
#include <glm/common.hpp>
#include <glm/geometric.hpp>

namespace field
{

	/*
	Given a `SquareNoise`: ℕ→ℝ⁴, `ValueNoise4` maps ℝ⁴→ℝ. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `SquareNoise`.
	*/
	template<typename SquareNoise>
	struct ValueNoise4
	{
		SquareNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit ValueNoise4(
			const SquareNoise& noise
		):
			noise(noise)
		{}

		using value_type = typename SquareNoise::value_type;

		template<typename T, glm::qualifier Q>
		value_type operator()(const glm::vec<4,T,Q> V) const {
			using vec = glm::vec<4,T,Q>;
		    vec I = glm::floor(V);
		    vec F = glm::smoothstep(vec(0), vec(1), glm::fract(V));
		    vec G = vec(0);
		    vec O = vec(0);
		    vec J = vec(0);
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
		                    a += noise(J) * G.x * G.y * G.z * G.w;
		                }
		            }
		        }
		    }

		    return a;
		}
	};

	template<typename SquareNoise>
	constexpr inline auto value_noise4(const SquareNoise noise)
	{
		return ValueNoise4<SquareNoise>(noise);
	}

	/*
	Given a `SquareNoise`: ℕ→ℝ³, `ValueNoise3` maps ℝ³→ℝ. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `SquareNoise`.
	*/
	template<typename SquareNoise>
	struct ValueNoise3
	{
		SquareNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit ValueNoise3(
			const SquareNoise& noise
		):
			noise(noise)
		{}

		using value_type = typename SquareNoise::value_type;

		template<typename T, glm::qualifier Q>
		value_type operator()(const glm::vec<3,T,Q> V) const {
			using vec = glm::vec<3,T,Q>;
		    vec I = glm::floor(V);
		    vec F = glm::smoothstep(vec(0), vec(1), glm::fract(V));
		    vec G = vec(0);
		    vec O = vec(0);
		    vec J = vec(0);
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
	                    a += noise(J) * G.x * G.y * G.z;
		            }
		        }
		    }

		    return a;
		}
	};

	template<typename SquareNoise>
	constexpr inline auto value_noise3(const SquareNoise noise)
	{
		return ValueNoise3<SquareNoise>(noise);
	}

	/*
	Given a `SquareNoise`: ℕ→ℝ³, `ValueNoise2` maps ℝ³→ℝ. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `SquareNoise`.
	*/
	template<typename SquareNoise>
	struct ValueNoise2
	{
		SquareNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit ValueNoise2(
			const SquareNoise& noise
		):
			noise(noise)
		{}

		using value_type = typename SquareNoise::value_type;

		template<typename T, glm::qualifier Q>
		value_type operator()(const glm::vec<2,T,Q> V) const {
			using vec = glm::vec<2,T,Q>;
		    vec I = glm::floor(V);
		    vec F = glm::smoothstep(vec(0), vec(1), glm::fract(V));
		    vec G = vec(0);
		    vec O = vec(0);
		    vec J = vec(0);
		    value_type a(0);
		    for (int i = 0; i <= 1; ++i)
		    {
		        for (int j = 0; j <= 1; ++j)
		        {
                    O = vec(i,j);
                    J = I + O;
                    G = glm::mix(vec(1)-F, F, O);
                    a += noise(J) * G.x * G.y;
		        }
		    }

		    return a;
		}
	};

	template<typename SquareNoise>
	constexpr inline auto value_noise2(const SquareNoise noise)
	{
		return ValueNoise2<SquareNoise>(noise);
	}


}

