#pragma once

#include <cmath>        // M_PI
#include <random>       // uniform distribution

#include <glm/vec3.hpp> // *vec3

namespace rasters
{
	namespace {
		/*
		"get_random_point_on_unit_sphere" returns a random point that is evenly distributed on a unit sphere
		see here for an explanation: http://corysimon.github.io/articles/uniformdistn-on-sphere/
		*/
		template <class Tgenerator>
		glm::vec3 get_random_point_on_unit_sphere(Tgenerator& generator)
		{
			std::uniform_real_distribution<float> distribution(0.0, 1.0);
	        double theta = 2 * M_PI * distribution(generator);
	        double phi = acos(1 - 2 * distribution(generator));
	        return glm::vec3(
		        sin(phi) * cos(theta),
		        sin(phi) * sin(theta),
		        cos(phi)
        	);
		}
	}
	/*
	"random()" generates a random scalar raster on the surface of a unit spheroid 
	The values within the raster fall within [0,1].
	
    "random()" uses an implementation of the terrain generation algorithm discussed by
    Hugo Elias here: http://freespace.virgin.net/hugo.elias/models/m_landsp.htm
    The algorithm was originally designed for terrain generation on a sphere,
     but it works for any scalar field on the surface of a spheroid. 
    It does this by iteratively incrementing values on grid cells that fall within a random region.
    Regions are defined by whether consine similarity to a random point on a unit sphere falls within a random threshold.
    Regions may have smooth boundaries, depending on a "region_transition_width" parameter that defines
     the width of a region's transition zone. 
    */
	template <class T, class Tgenerator>
	void random(
		const SpheroidGrid& grid, 
		Tgenerator& generator, 
		tmany<T>& out, 
		// NOTE: "region_count" is the number of regions where we increment grid cell values
		uint region_count = 1000,
		// NOTE: "region_transition_width" is the width of the transition zone for a region
		T region_transition_width = T(0.03)
	){
		tmany<T> region_mod(out.size(), T(0));
		T region_threshold(0);
		glm::vec3 region_center(0);
		std::uniform_real_distribution<float> distribution(0.0, 1.0);

		many::fill(out, T(0));
		for (uint i = 0; i < region_count; ++i)
		{
			region_threshold = distribution(generator);
			region_center = rasters::get_random_point_on_unit_sphere(generator);
			many::dot(grid.vertex_positions, region_center, 	 region_mod);
			many::smoothstep(
				region_threshold - region_transition_width/T(2), 
				region_threshold + region_transition_width/T(2),  
				region_mod, 									 region_mod);
			many::add(out, region_mod, 							 out);
		}
	}


	template <class T, class Tgenerator>
	tmany<T> random(
		const SpheroidGrid& grid, 
		Tgenerator& generator
	){
		tmany<T> out(grid.vertex_positions.size());
		random(grid, generator, out);
		return out;
	}
}