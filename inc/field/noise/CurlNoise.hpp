#pragma once

// C libraries

// std libraries

// 3rd-party libraries
#include <glm/common.hpp>
#include <glm/geometric.hpp>

namespace field
{

	/*
	Given a `SmoothVectorNoise`: ℕ→ℝ³, `CurlNoise4` maps ℝ⁴→ℝ⁴. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `SmoothVectorNoise`.
	*/
	template<typename SmoothVectorNoise>
	struct CurlNoise4
	{
		SmoothVectorNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit CurlNoise4(
			const SmoothVectorNoise& noise
		):
			noise(noise)
		{}

		using value_type = typename SmoothVectorNoise::value_type;

		template<typename T, glm::qualifier Q>
		value_type operator()(const glm::vec<4,T,Q> V) const {
			using vec4 = glm::vec<4,T,Q>;
			using vec3 = glm::vec<3,T,Q>;

		    const T e = .1;
		    vec3 dx = vec3( e   , 0.0 , 0.0 );
		    vec3 dy = vec3( 0.0 , e   , 0.0 );
		    vec3 dz = vec3( 0.0 , 0.0 , e   );

		    vec3 V_x0 = noise( V - dx );
		    vec3 V_x1 = noise( V + dx );
		    vec3 V_y0 = noise( V - dy );
		    vec3 V_y1 = noise( V + dy );
		    vec3 V_z0 = noise( V - dz );
		    vec3 V_z1 = noise( V + dz );

		    T x = V_y1.z - V_y0.z - V_z1.y + V_z0.y;
		    T y = V_z1.x - V_z0.x - V_x1.z + V_x0.z;
		    T z = V_x1.y - V_x0.y - V_y1.x + V_y0.x;

		    const T divisor = 1.0 / ( 2.0 * e );
		    return vec4( x , y , z, 0 ) * divisor;

		}
	};

	template<typename SmoothVectorNoise>
	constexpr inline auto curl_noise4(const SmoothVectorNoise noise)
	{
		return CurlNoise4<SmoothVectorNoise>(noise);
	}

	/*
	Given a `SmoothVectorNoise`: ℕ→ℝ³, `CurlNoise3` maps ℝ³→ℝ³. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `SmoothVectorNoise`.
	*/
	template<typename SmoothVectorNoise>
	struct CurlNoise3
	{
		SmoothVectorNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit CurlNoise3(
			const SmoothVectorNoise& noise
		):
			noise(noise)
		{}

		using value_type = typename SmoothVectorNoise::value_type;

		template<typename T, glm::qualifier Q>
		value_type operator()(const glm::vec<3,T,Q> V) const {
			using vec3 = glm::vec<3,T,Q>;

		    const T e = .1;
		    vec3 dx = vec3( e   , 0.0 , 0.0 );
		    vec3 dy = vec3( 0.0 , e   , 0.0 );
		    vec3 dz = vec3( 0.0 , 0.0 , e   );

		    vec3 V_x0 = noise( V - dx );
		    vec3 V_x1 = noise( V + dx );
		    vec3 V_y0 = noise( V - dy );
		    vec3 V_y1 = noise( V + dy );
		    vec3 V_z0 = noise( V - dz );
		    vec3 V_z1 = noise( V + dz );

		    T x = V_y1.z - V_y0.z - V_z1.y + V_z0.y;
		    T y = V_z1.x - V_z0.x - V_x1.z + V_x0.z;
		    T z = V_x1.y - V_x0.y - V_y1.x + V_y0.x;

		    const T divisor = 1.0 / ( 2.0 * e );
		    return vec3( x , y , z ) * divisor;

		}
	};

	template<typename SmoothVectorNoise>
	constexpr inline auto curl_noise3(const SmoothVectorNoise noise)
	{
		return CurlNoise3<SmoothVectorNoise>(noise);
	}

}

