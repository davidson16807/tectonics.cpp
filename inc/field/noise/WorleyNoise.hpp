#pragma once

// C libraries

// std libraries

// 3rd-party libraries
#include <glm/common.hpp>
#include <glm/geometric.hpp>

namespace field
{

	/*
	Given a `SquareVectorNoise`: ℕ→ℝ⁴, `WorleyNoise4` maps ℝ⁴→ℝ. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `SquareVectorNoise`.
	*/
	template<typename SquareVectorNoise>
	struct WorleyNoise4
	{
		SquareVectorNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit WorleyNoise4(const SquareVectorNoise& noise): noise(noise) {}
		WorleyNoise4(const WorleyNoise4& Worley): noise(Worley.noise) {}

		using vector_type = typename SquareVectorNoise::value_type;
		using value_type = typename vector_type::value_type;

		template<typename T, glm::qualifier Q>
		value_type operator()(const glm::vec<4,T,Q> V) const {
			using vec = glm::vec<4,T,Q>;
			using ivec = glm::vec<4,int,Q>;
		    ivec index = ivec(V);
		    T nearest_distance = std::numeric_limits<T>::max();
		    ivec neighbor_index = ivec(0);
		    vec  neighbor_position = vec(0);

		    for (int x = -1; x <= 1; x++)
		    {
		        for (int y = -1; y <= 1; y++)
		        {
		            for (int z = -1; z <= 1; z++)
		            {   
		                for (int w = -1; w <= 1; w++)
		                {   
		                    neighbor_index = index + ivec(x, y, z, w);
		                    neighbor_position = vec(neighbor_index) + noise(neighbor_index);
		                    nearest_distance = std::min(nearest_distance, glm::distance(V, neighbor_position));
		                }
		            }
		        }
		    }

		    return nearest_distance;
		}
	};

	template<typename SquareVectorNoise>
	constexpr inline auto worley_noise4(const SquareVectorNoise noise)
	{
		return WorleyNoise4<SquareVectorNoise>(noise);
	}

	/*
	Given a `SquareVectorNoise`: ℕ→ℝ³, `WorleyNoise3` maps ℝ³→ℝ. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `SquareVectorNoise`.
	*/
	template<typename SquareVectorNoise>
	struct WorleyNoise3
	{
		SquareVectorNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit WorleyNoise3(const SquareVectorNoise& noise): noise(noise) {}
		WorleyNoise3(const WorleyNoise3& Worley): noise(Worley.noise) {}

		using vector_type = typename SquareVectorNoise::value_type;
		using value_type = typename vector_type::value_type;

		template<typename T, glm::qualifier Q>
		value_type operator()(const glm::vec<3,T,Q> V) const {
			using vec = glm::vec<3,T,Q>;
			using ivec = glm::vec<3,int,Q>;
		    ivec index = ivec(V);
		    T nearest_distance = std::numeric_limits<T>::max();
		    ivec neighbor_index = ivec(0);
		    vec  neighbor_position = vec(0);

		    for (int x = -1; x <= 1; x++)
		    {
		        for (int y = -1; y <= 1; y++)
		        {
		            for (int z = -1; z <= 1; z++)
		            {   
	                    neighbor_index = index + ivec(x, y, z);
	                    neighbor_position = vec(neighbor_index) + noise(neighbor_index);
	                    nearest_distance = std::min(nearest_distance, glm::distance(V, neighbor_position));
		            }
		        }
		    }
		    
		    return nearest_distance;
		}
	};

	template<typename SquareVectorNoise>
	constexpr inline auto worley_noise3(const SquareVectorNoise noise)
	{
		return WorleyNoise3<SquareVectorNoise>(noise);
	}

	/*
	Given a `SquareVectorNoise`: ℕ→ℝ², `WorleyNoise2` maps ℝ²→ℝ. 
	For each point, `WorleyNoise` returns the distance to the closest point 
	of a set of procedurally generated points, given by `SquareVectorNoise`.
	*/
	template<typename SquareVectorNoise>
	struct WorleyNoise2
	{
		SquareVectorNoise noise;

		/*
		`region_transition_width` is the width of the transition zone for a region
		`region_count` is the number of regions where we increment grid cell values
		*/
		explicit WorleyNoise2(const SquareVectorNoise& noise): noise(noise) {}
		WorleyNoise2(const WorleyNoise2& Worley): noise(Worley.noise) {}

		using vector_type = typename SquareVectorNoise::value_type;
		using value_type = typename vector_type::value_type;

		template<typename T, glm::qualifier Q>
		value_type operator()(const glm::vec<2,T,Q> V) const {
			using vec = glm::vec<2,T,Q>;
			using ivec = glm::vec<2,int,Q>;

		    ivec index = ivec(V);

		    T nearest_distance = std::numeric_limits<T>::max();
		    ivec neighbor_index = ivec(0);
		    vec  neighbor_position = vec(0);

		    for (int x = -1; x <= 1; x++)
		    {
		        for (int y = -1; y <= 1; y++)
		        {
                    neighbor_index = index + ivec(x, y);
                    neighbor_position = vec(neighbor_index) + noise(neighbor_index);
                    nearest_distance = std::min(nearest_distance, glm::distance(V, neighbor_position));
		        }
		    }

		    return nearest_distance;
		}
	};

	template<typename SquareVectorNoise>
	constexpr inline auto worley_noise2(const SquareVectorNoise noise)
	{
		return WorleyNoise2<SquareVectorNoise>(noise);
	}

}

