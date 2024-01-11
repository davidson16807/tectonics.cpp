#pragma once

// C libraries

// std libraries

// 3rd-party libraries
#include <glm/common.hpp>
#include <glm/geometric.hpp>

namespace field
{

	/*
	Given a `MosaicNoise`: ℕ→ℝ⁴, `ValueNoise4` maps ℝ⁴→ℝ. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `MosaicNoise`.
	*/
	template<typename scalar, typename MosaicNoise>
	struct ValueNoise4
	{
		MosaicNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit ValueNoise4(
			const MosaicNoise& noise
		):
			noise(noise)
		{}

		using vec = glm::vec<4,scalar,glm::defaultp>;
		using value_type = typename MosaicNoise::value_type;

		template<typename point>
		value_type operator()(const point V) const {
		    point I = floor(V);
		    vec F = glm::smoothstep(vec(0), vec(1), glm::fract(V));
		    vec G = vec(0);
		    vec O = vec(0);
		    point J;
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

	template<typename scalar, typename MosaicNoise>
	constexpr inline auto value_noise4(const MosaicNoise noise)
	{
		return ValueNoise4<scalar,MosaicNoise>(noise);
	}

	/*
	Given a `MosaicNoise`: ℕ→ℝ³, `ValueNoise3` maps ℝ³→ℝ. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `MosaicNoise`.
	*/
	template<typename scalar, typename MosaicNoise>
	struct ValueNoise3
	{
		MosaicNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit ValueNoise3(
			const MosaicNoise& noise
		):
			noise(noise)
		{}

		using vec = glm::vec<3,scalar,glm::defaultp>;
		using value_type = typename MosaicNoise::value_type;

		template<typename point>
		value_type operator()(const point V) const {
		    vec I = floor(V);
		    vec F = glm::smoothstep(vec(0), vec(1), glm::fract(V));
		    vec G = vec(0);
		    vec O = vec(0);
		    point J;
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

	template<typename scalar, typename MosaicNoise>
	constexpr inline auto value_noise3(const MosaicNoise noise)
	{
		return ValueNoise3<scalar,MosaicNoise>(noise);
	}

	/*
	Given a `MosaicNoise`: ℕ→ℝ³, `ValueNoise2` maps ℝ³→ℝ. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `MosaicNoise`.
	*/
	template<typename scalar, typename MosaicNoise>
	struct ValueNoise2
	{
		MosaicNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit ValueNoise2(
			const MosaicNoise& noise
		):
			noise(noise)
		{}

		using vec = glm::vec<2,scalar,glm::defaultp>;
		using value_type = typename MosaicNoise::value_type;

		template<typename point>
		value_type operator()(const point V) const {
		    vec I = floor(V);
		    vec F = glm::smoothstep(vec(0), vec(1), glm::fract(V));
		    vec G = vec(0);
		    vec O = vec(0);
		    point J;
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

	template<typename scalar, typename MosaicNoise>
	constexpr inline auto value_noise(const MosaicNoise noise)
	{
		return ValueNoise2<scalar,MosaicNoise>(noise);
	}


}

