#pragma once

// Standard libraries
#include <cmath>

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL // length2
#include <glm/vec3.hpp>     // *vec3
#include <glm/gtx/norm.hpp> // length2

// in-house libraries
#include <math/lerp.hpp>

#include "property/UniversalVariableProperties.hpp"

#include "Elements.hpp"
#include "State.hpp"

namespace orbit {

template <typename Tfloat>
struct Trancendentals
{
	Trancendentals(){}
	Tfloat C(const Tfloat y){
		const Tfloat abs_y = std::abs(y);
		const Tfloat sqrt_abs_y = std::sqrt(abs_y)
		return (y > Tfloat(0.0)? 1.0-std::cos(sqrt_abs_y) : std::cosh(sqrt_abs_y)-1.0) / abs_y;
	}
	Tfloat S(const Tfloat y){
		const Tfloat abs_y = std::abs(y);
		const Tfloat sqrt_abs_y = std::sqrt(abs_y)
		const Tfloat sqrt_abs_y3 = sqrt_abs_y*sqrt_abs_y*sqrt_abs_y;

		return y > (Tfloat(0.0)? sqrt_abs_y - std::sin(sqrt_abs_y) : std::sinh(sqrt_abs_y) - sqrt_abs_y) / sqrt_abs_y3;
	}
}
template <typename Tfloat>
struct UniversalProperties
{
	const Trancendentals<Tfloat> trancendentals;
	UniversalProperties(
		const Trancendentals<Tfloat> trancendentals,
		const glm::vec<3,Tfloat,glm::defaultp> vernal_equinox_direction, 
		const glm::vec<3,Tfloat,glm::defaultp> north_pole_direction, 
		const Tfloat gravitational_parameter
	) :
		trancendentals(trancendentals)
	{}

	// "a"
	Tfloat get_semi_major_axis_from_state(
		const glm::vec<3,Tfloat,glm::defaultp> position, 
		const glm::vec<3,Tfloat,glm::defaultp> velocity
	) const	{
		return 1.0 / (2.0 / glm::length(position) - glm::length2(velocity) / gravitational_parameter);
	}

	glm::vec<3,Tfloat,glm::defaultp> get_position_from_semi_major_axis_and_state(
		const glm::vec<3,Tfloat,glm::defaultp> position, 
		const glm::vec<3,Tfloat,glm::defaultp> velocity,
		const Tfloat semi_major_axis, 
		const Tfloat time_offset
	) const	{
		return position * (1.0 - C(x*x/semi_major_axis) * x*x/glm::length2(position)) + 
		       velocity * (time_offset - S(x*x/semi_major_axis) * x*x*x/std::sqrt(gravitational_parameter));
	}

	glm::vec<3,Tfloat,glm::defaultp> get_velocity_from_semi_major_axis_and_state(
		const glm::vec<3,Tfloat,glm::defaultp> position, 
		const glm::vec<3,Tfloat,glm::defaultp> velocity,
		const Tfloat semi_major_axis
	) const	{
		return x*std::sqrt(gravitational_parameter)/
	}


};

}