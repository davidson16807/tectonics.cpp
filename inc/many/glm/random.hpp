#pragma once

#include <cmath>        // M_PI
#include <random>       // uniform distribution

#include <glm/vec3.hpp>          // *vec3
#include <glm/common.hpp>        // floor
#include <glm/geometric.hpp>     // distance
#include <glm/trigonometric.hpp> // sin

#include <many/types.hpp>     	 // floats
#include <many/common.hpp>       // smoothstep
#include <many/arithmetic.hpp>   // smoothstep
#include <many/glm/types.hpp>    // *vec3s
#include <many/glm/geometric.hpp>// dot

namespace many
{
	/*
	"get_random_point_on_unit_sphere" returns a random point that is evenly distributed on a unit sphere
	see here for an explanation: http://corysimon.github.io/articles/uniformdistn-on-sphere/
	*/
	template <typename Tgenerator>
	glm::vec3 get_random_point_on_unit_sphere(Tgenerator& generator)
	{
		std::uniform_real_distribution<float> distribution(0.0, 1.0);
        double theta = 2 * M_PI * distribution(generator);
        double phi = std::acos(1 - 2 * distribution(generator));
        return glm::vec3(
	        std::sin(phi) * std::cos(theta),
	        std::sin(phi) * std::sin(theta),
	        std::cos(phi)
    	);
	}
	/*
	"noise()" is a pure function that generates a seemingly random number for a given seed. 
	Given the same seed, noise() will always generate the same output. 
	The output should be uniformly distribution to an approximation across all values of seed.
	*/
	float noise(const float seed, const float a = 10.f, const float b=10000.f) {
		return glm::fract(std::sin(seed*a)*b);
	}
	template <int L, typename T, glm::qualifier Q>
	glm::vec<L,T,Q> noise(const glm::vec<L,T,Q> seed, const float a = 10.f, const float b=10000.f) {
		return glm::fract(glm::sin(seed*a)*b);
	}
	// TODO: rename to "get_angular_noise"
	/*
	"get_elias_noise()" generates a scalar raster
	whose values are randomly distributed when projected onto a unit sphere
	The values within the raster fall within [0,1].
	This is particularly useful when generating a heightmap on a globe.
	
    "get_elias_noise()" uses an implementation of the terrain generation algorithm discussed by
    Hugo Elias here: http://freespace.virgin.net/hugo.elias/models/m_landsp.htm
    The algorithm was originally designed for terrain generation on a sphere,
     but it works for any scalar field on the surface of a spheroid. 
    It does this by iteratively incrementing values on grid cells that fall within a random region on a unit sphere.
    Regions are defined by whether consine similarity to a random point on a unit sphere falls within a random threshold.
    Regions may have smooth boundaries, depending on a "region_transition_width" parameter that defines
     the width of a region's transition zone. 
    */
	template <typename T, typename Tgenerator>
	void get_elias_noise(
		const many::series<glm::vec3>& positions, 
		Tgenerator& generator, 
		many::series<T>& out, 
		// NOTE: "region_count" is the number of regions where we increment grid cell values
		uint region_count = 1000,
		// NOTE: "region_transition_width" is the width of the transition zone for a region
		T region_transition_width = T(0.03)
	){
		many::series<T> region_mod(out.size(), T(0));
		T region_threshold(0);
		glm::vec3 region_center(0);
		std::uniform_real_distribution<float> distribution(0.0, 1.0);

		many::fill(out, T(0));
		for (uint i = 0; i < region_count; ++i)
		{
			region_threshold = distribution(generator);
			region_center = many::get_random_point_on_unit_sphere(generator);
			many::dot(positions, region_center, 	 region_mod);
			many::smoothstep(
				region_threshold - region_transition_width/T(2), 
				region_threshold + region_transition_width/T(2),  
				region_mod, 									 region_mod);
			many::add(out, region_mod, 							 out);
		}
	}

	template <typename T, typename Tgenerator>
	many::series<T> get_elias_noise(
		const many::series<glm::vec3>& positions, 
		Tgenerator& generator,
		// NOTE: "region_count" is the number of regions where we increment grid cell values
		uint region_count = 1000,
		// NOTE: "region_transition_width" is the width of the transition zone for a region
		T region_transition_width = T(0.03)
	){
		many::series<T> out(positions.size());
		get_elias_noise(positions, generator, out, region_count, region_transition_width);
		return out;
	}

	template <typename T>
	void get_perlin_noise(
		const many::series<glm::vec3>& positions, 
		many::series<T>& out,
		glm::vec3 K = glm::vec3(1.6, 80., 7.0)
	){
		assert(out.size() == positions.size());
		glm::vec3  V(0);
		glm::vec3  I(0);
		glm::vec3  F(0);
		glm::vec3  G(0);
		for (uint i = 0; i < positions.size(); ++i)
		{
			V = positions[i];
			I = glm::floor(V);
			F = glm::fract(V);

		    float o   = noise(dot(K, I));
		    float x   = noise(dot(K, I + glm::vec3(1, 0, 0)));
		    float y   = noise(dot(K, I + glm::vec3(0, 1, 0)));
		    float xy  = noise(dot(K, I + glm::vec3(1, 1, 0)));

		    float z   = noise(dot(K, I + glm::vec3(0, 0, 1)));
		    float xz  = noise(dot(K, I + glm::vec3(1, 0, 1)));
		    float yz  = noise(dot(K, I + glm::vec3(0, 1, 1)));
		    float xyz = noise(dot(K, I + glm::vec3(1, 1, 1)));

		    G = glm::smoothstep(0.f, 1.f, F);

		    out[i] = o    *      G.x  *      G.y  *      G.z  
		           + x    * (1.f-G.x) *      G.y  *      G.z  
		           + y    *      G.x  * (1.f-G.y) *      G.z 
		           + xy   * (1.f-G.x) * (1.f-G.y) *      G.z 
		           + z    *      G.x  *      G.y  * (1.f-G.z)
		           + xz   * (1.f-G.x) *      G.y  * (1.f-G.z)
		           + yz   *      G.x  * (1.f-G.y) * (1.f-G.z)
		           + xyz  * (1.f-G.x) * (1.f-G.y) * (1.f-G.z);
		}
	}

	template <typename T>
	void get_worley_noise(
		const many::series<glm::vec3>& positions, 
		many::series<T>& out
	){
		assert(out.size() == positions.size());
		glm::vec3  V(0.);
		glm::vec3  I(0.);
		glm::vec3  F(0.);

		glm::vec3  neighbor_offset;
		glm::vec3  neighbor_id;
		glm::vec3  neighbor_position;
		float neighbor_distance;

		glm::vec3 nearest_id;
		glm::vec3  nearest_position;
		float nearest_distance;

		for (uint id = 0; id < positions.size(); ++id)
		{
			V = positions[id];
			I = glm::floor(V);
			F = glm::fract(V);

			nearest_id = I;
			nearest_position = I + noise(I);
			nearest_distance = glm::distance(nearest_id, V);
			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					for (int k = -1; k <= 1; ++k)
					{
						neighbor_offset = glm::vec3(i,j,k);
						neighbor_id = I + neighbor_offset;
						neighbor_position = glm::vec3(neighbor_id) + noise(glm::vec3(neighbor_id));
						neighbor_distance = glm::distance(neighbor_position, V);
						if (neighbor_distance < nearest_distance)
						{
							nearest_id = neighbor_id;
							nearest_position = neighbor_position;
							nearest_distance = neighbor_distance;
						}
					}
				}
			}
		}
	}
}