#pragma once

// C libraries

// std libraries

// 3rd-party libraries
#include <glm/common.hpp>
#include <glm/geometric.hpp>

namespace field
{

	/*
	Given a `SquareVectorNoise`: ℕ→ℝ⁴, `SimplexNoise4` maps ℝ⁴→ℝ. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `SquareVectorNoise`.
	*/
	template<typename SquareVectorNoise>
	struct SimplexNoise4
	{
		SquareVectorNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit SimplexNoise4(const SquareVectorNoise& noise): noise(noise) {}
		SimplexNoise4(const SimplexNoise4& perlin): noise(perlin.noise) {}

		using vector_type = typename SquareVectorNoise::value_type;
		using value_type = typename vector_type::value_type;

		/*
		 * Efficient simplex indexing functions by Bill Licea-Kane, ATI. Thanks!
		 */
		template<typename T, glm::qualifier Q>
		void get_4d_simplex_index_offsets( 
		    const glm::vec<4,T,Q> P, 
		    glm::vec<4,T,Q>& offset1, 
		    glm::vec<4,T,Q>& offset2, 
		    glm::vec<4,T,Q>& offset3 
		) const {
			using vec4 = glm::vec<4,T,Q>;
			using vec3 = glm::vec<3,T,Q>;
			using vec2 = glm::vec<2,T,Q>;
			using scalar = T;

		    vec4 offset0;

		    vec3 isX = glm::step( P.yzw, P.xxx );        // See comments in 3D simplex function
		    offset0.x = isX.x + isX.y + isX.z;
		    offset0.yzw = 1.0 - isX;

		    vec2 isY = glm::step( P.zw, P.yy );
		    offset0.y += isY.x + isY.y;
		    offset0.zw += 1.0 - isY;

		    scalar isZ = glm::step( P.w, P.z );
		    offset0.z += isZ;
		    offset0.w += 1.0 - isZ;

		    // offset0 now contains the unique values 0,1,2,3 in each channel

		    offset3 = glm::clamp( offset0, 0.0, 1.0 );
		    offset2 = glm::clamp( offset0-1.0, 0.0, 1.0 );
		    offset1 = glm::clamp( offset0-2.0, 0.0, 1.0 );

		}

		template<typename T, glm::qualifier Q>
		value_type operator()(const glm::vec<4,T,Q> V) const {
			using mat4 = glm::mat<4,4,T,Q>;
			using vec4 = glm::vec<4,T,Q>;
			using scalar = T;

		    const scalar s = (std::sqrt(5.0)-1.0)/4.0;
		    const mat4 S = mat4(1.0+s, s, s, s,
		                        s, 1.0+s, s, s,
		                        s, s, 1.0+s, s,
		                        s, s, s, 1.0+s);
		    const mat4 Sinv = glm::inverse(S);
		    vec4 F  = glm::fract(S*V);
		    // simplex vertex index offsets
		    vec4 O1; vec4 O2; vec4 O3; get_4d_simplex_index_offsets(F, O1, O2, O3);
		    // simplex vertex indices
		    vec4 I0 = glm::floor(S*V); 
		    vec4 I1 = I0 + O1;
		    vec4 I2 = I0 + O2;
		    vec4 I3 = I0 + O3;
		    vec4 I4 = I0 + vec4(1,1,1,1);
		    // simplex vertex positions
		    vec4 V0 = Sinv*I0;
		    vec4 V1 = Sinv*I1;
		    vec4 V2 = Sinv*I2;
		    vec4 V3 = Sinv*I3;
		    vec4 V4 = Sinv*I4;
		    // simplex vertex position offsets
		    vec4 F0 = V-V0;
		    vec4 F1 = V-V1;
		    vec4 F2 = V-V2;
		    vec4 F3 = V-V3;
		    vec4 F4 = V-V4;
		    scalar t0 = std::max( 0.5 - glm::dot(F0, F0), 0.0); t0*=t0; t0*=t0; 
		    scalar t1 = std::max( 0.5 - glm::dot(F1, F1), 0.0); t1*=t1; t1*=t1; 
		    scalar t2 = std::max( 0.5 - glm::dot(F2, F2), 0.0); t2*=t2; t2*=t2; 
		    scalar t3 = std::max( 0.5 - glm::dot(F3, F3), 0.0); t3*=t3; t3*=t3; 
		    scalar t4 = std::max( 0.5 - glm::dot(F4, F4), 0.0); t4*=t4; t4*=t4; 
		    return 27.0 * (
		        t0 * glm::dot(glm::normalize(noise(I0)), F0) +
		        t1 * glm::dot(glm::normalize(noise(I1)), F1) +
		        t2 * glm::dot(glm::normalize(noise(I2)), F2) +
		        t3 * glm::dot(glm::normalize(noise(I3)), F3) +
		        t4 * glm::dot(glm::normalize(noise(I4)), F4)
		    );

		}
	};

	template<typename SquareVectorNoise>
	constexpr inline auto simplex_noise4(const SquareVectorNoise noise)
	{
		return SimplexNoise4<SquareVectorNoise>(noise);
	}

	/*
	Given a `SquareVectorNoise`: ℕ→ℝ³, `SimplexNoise3` maps ℝ³→ℝ. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `SquareVectorNoise`.
	*/
	template<typename SquareVectorNoise>
	struct SimplexNoise3
	{
		SquareVectorNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit SimplexNoise3(const SquareVectorNoise& noise): noise(noise) {}
		SimplexNoise3(const SimplexNoise3& perlin): noise(perlin.noise) {}

		using vector_type = typename SquareVectorNoise::value_type;
		using value_type = typename vector_type::value_type;

		/*
		 * Efficient simplex indexing functions by Bill Licea-Kane, ATI. Thanks!
		 */
		template<typename T, glm::qualifier Q>
		void get_3d_simplex_index_offsets( 
		    const glm::vec<3,T,Q> P, 
		    glm::vec<3,T,Q>& offset1, 
		    glm::vec<3,T,Q>& offset2 
		) const {
			using vec3 = glm::vec<3,T,Q>;
			using vec2 = glm::vec<2,T,Q>;
			using scalar = T;

		    vec3 offset0;

		    vec2 isX = glm::step( P.yz(), P.xx() ); // P.x >= P.y ? 1.0 : 0.0;  P.x >= P.z ? 1.0 : 0.0;
		    offset0.x  = isX.x + isX.y;    // Accumulate all P.x >= other channels in offset.x
		    offset0.y = 1.0 - isX.x;       // Accumulate all P.x <  other channels in offset.yz
		    offset0.z = 1.0 - isX.y;       // Accumulate all P.x <  other channels in offset.yz

		    scalar isY = glm::step( P.z, P.y );  // P.y >= P.z ? 1.0 : 0.0;
		    offset0.y += isY;              // Accumulate P.y >= P.z in offset.y
		    offset0.z += 1.0 - isY;        // Accumulate P.y <  P.z in offset.z

		    // offset0 now contains the unique values 0,1,2 in each channel
		    // 2 for the channel greater than other channels
		    // 1 for the channel that is less than one but greater than another
		    // 0 for the channel less than other channels
		    // Equality ties are broken in favor of first x, then y
		    // (z always loses ties)

		    offset2 = glm::clamp( offset0, 0.0, 1.0 );
		    // offset2 contains 1 in each channel that was 1 or 2
		    offset1 = glm::clamp( offset0-1.0, 0.0, 1.0 );
		    // offset1 contains 1 in the single channel that was 1

		}

		template<typename T, glm::qualifier Q>
		value_type operator()(const glm::vec<3,T,Q> V) const {
			using mat3 = glm::mat<3,3,T,Q>;
			using vec3 = glm::vec<3,T,Q>;
			using scalar = T;

		    const scalar s = 1.0/3.0;
		    const mat3 S = mat3(1.0+s, s, s,
		                        s, 1.0+s, s,
		                        s, s, 1.0+s);
		    const mat3 Sinv = glm::inverse(S);
		    vec3 F  = glm::fract(S*V);
		    // simplex vertex index offsets
		    vec3 O1; vec3 O2; get_3d_simplex_index_offsets(F, O1, O2);
		    // simplex vertex indices
		    vec3 I0 = glm::floor(S*V); 
		    vec3 I1 = I0 + O1;
		    vec3 I2 = I0 + O2;
		    vec3 I3 = I0 + vec3(1,1,1);
		    // simplex vertex positions
		    vec3 V0 = Sinv*I0;
		    vec3 V1 = Sinv*I1;
		    vec3 V2 = Sinv*I2;
		    vec3 V3 = Sinv*I3;
		    // simplex vertex position offsets
		    vec3 F0 = V-V0;
		    vec3 F1 = V-V1;
		    vec3 F2 = V-V2;
		    vec3 F3 = V-V3;
		    scalar t0 = std::max( 0.5 - glm::dot(F0, F0), 0.0); t0*=t0; t0*=t0; 
		    scalar t1 = std::max( 0.5 - glm::dot(F1, F1), 0.0); t1*=t1; t1*=t1; 
		    scalar t2 = std::max( 0.5 - glm::dot(F2, F2), 0.0); t2*=t2; t2*=t2; 
		    scalar t3 = std::max( 0.5 - glm::dot(F3, F3), 0.0); t3*=t3; t3*=t3; 
		    return 32.0 * (
		        t0 * glm::dot(glm::normalize(noise(I0)), F0) +
		        t1 * glm::dot(glm::normalize(noise(I1)), F1) +
		        t2 * glm::dot(glm::normalize(noise(I2)), F2) +
		        t3 * glm::dot(glm::normalize(noise(I3)), F3)
		    );

		}
	};

	template<typename SquareVectorNoise>
	constexpr inline auto simplex_noise3(const SquareVectorNoise noise)
	{
		return SimplexNoise3<SquareVectorNoise>(noise);
	}

	/*
	Given a `SquareVectorNoise`: ℕ→ℝ³, `SimplexNoise2` maps ℝ³→ℝ. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `SquareVectorNoise`.
	*/
	template<typename SquareVectorNoise>
	struct SimplexNoise2
	{
		SquareVectorNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit SimplexNoise2(const SquareVectorNoise& noise): noise(noise) {}
		SimplexNoise2(const SimplexNoise2& perlin): noise(perlin.noise) {}

		using vector_type = typename SquareVectorNoise::value_type;
		using value_type = typename vector_type::value_type;

		template<typename T, glm::qualifier Q>
		value_type operator()(const glm::vec<2,T,Q> V) const {
			using mat2 = glm::mat<2,2,T,Q>;
			using vec2 = glm::vec<2,T,Q>;
			using scalar = T;

		    const scalar s = (std::sqrt(3.0)-1.0)/2.0;
		    const mat2 S = mat2(1.0+s, s,
		                        s, 1.0+s);
		    const mat2 Sinv = glm::inverse(S);
		    vec2 F  = glm::fract(S*V);
		    // simplex vertex indices
		    vec2 I0 = glm::floor(S*V); 
		    vec2 I1 = I0 + (F.x > F.y? vec2(1,0) : vec2(0,1));
		    vec2 I2 = I0 + vec2(1,1);
		    // simplex vertex positions
		    vec2 V0 = Sinv*I0;
		    vec2 V1 = Sinv*I1;
		    vec2 V2 = Sinv*I2;
		    // simplex vertex position offsets
		    vec2 F0 = V-V0;
		    vec2 F1 = V-V1;
		    vec2 F2 = V-V2;
		    scalar t0 = std::max( 0.5 - glm::dot(F0, F0), 0.0); t0*=t0; t0*=t0; 
		    scalar t1 = std::max( 0.5 - glm::dot(F1, F1), 0.0); t1*=t1; t1*=t1; 
		    scalar t2 = std::max( 0.5 - glm::dot(F2, F2), 0.0); t2*=t2; t2*=t2; 
		    return 30.0 * (
		        t0 * glm::dot(glm::normalize(noise(I0)), F0) +
		        t1 * glm::dot(glm::normalize(noise(I1)), F1) +
		        t2 * glm::dot(glm::normalize(noise(I2)), F2)
		    );

		}
	};

	template<typename SquareVectorNoise>
	constexpr inline auto simplex_noise2(const SquareVectorNoise noise)
	{
		return SimplexNoise2<SquareVectorNoise>(noise);
	}

}

