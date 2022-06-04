#pragma once

// C libraries
#include <cmath>

// 3rd party libraries
#include <glm/vec2.hpp>       	 // *vec3
#include <glm/vec3.hpp>       	 // *vec3

namespace rasters
{

	/*
	*/

    template<typename Tid=std::uint16_t>
	class SpheroidLayering
	{
        const Tfloat epsilon_for_integer_cast(0.1); // ensures that integer casts do not produce numbers that are one value lower than expected
		const Tfloat profile_bottom_radius;
		const Tfloat profile_height;
        const Tfloat layer_count;

	public:
		~SpheroidLayering()
		{
		}
		explicit SpheroidLayering(const Tfloat profile_bottom_radius, const Tfloat profile_height, const Tid layer_count):
			profile_bottom_radius(profile_bottom_radius),
			profile_height(profile_height),
			layer_count(layer_count)
        {

        }

		glm::vec<3,Tfloat,glm::defaultp> layer_position(const glm::vec<3,Tfloat,glm::defaultp> direction, const Tid layer_id) const {
			return glm::normalize(direction) * (profile_bottom_radius + profile_height * (Tfloat(layer_id)/layer_count));
		}

		Tid layer_id(const glm::vec<3,Tfloat,glm::defaultp> layer_position) const {
			return std::round(layer_count * (glm::length(layer_position) - profile_bottom_radius) / profile_height) + epsilon_for_integer_cast;
		}

	};
}