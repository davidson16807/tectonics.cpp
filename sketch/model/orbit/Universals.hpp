#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

#include <math/special.hpp>

#include "State.hpp"

namespace orbit {

	/*
	All methods are conceptually pure methods, 
	i.e. they are free of side effects and their output is completely
	determiend by parameters and the state of `this`.

	Output of methods are always represented in 
	whatever units of measured that are used by parameters and `this`.
	It is the developer's responsibility to ensure consistency of units.
	*/
	template <typename scalar>
	struct Universals
	{
		using vec3 = glm::vec<3,scalar,glm::defaultp>;

		const scalar gravitational_parameter;
		const vec3 initial_position;
		const vec3 initial_velocity;
		const scalar time_offset;

		Universals(
			const scalar gravitational_parameter,
			const vec3 initial_position,
			const vec3 initial_velocity
		) : 
			gravitational_parameter(gravitational_parameter),
			initial_position(initial_position),
			initial_velocity(initial_velocity),
			time_offset(0)
		{}

		Universals(
			const scalar gravitational_parameter,
			const State<scalar> state
		) : 
			gravitational_parameter(gravitational_parameter),
			initial_position(state.position),
			initial_velocity(state.velocity),
			time_offset(0)
		{}

		Universals(
			const scalar gravitational_parameter,
			const vec3 initial_position,
			const vec3 initial_velocity,
			const scalar time_offset
		) : 
			gravitational_parameter(gravitational_parameter),
			initial_position(initial_position),
			initial_velocity(initial_velocity),
			time_offset(time_offset)
		{}

		Universals(
			const scalar gravitational_parameter,
			const State<scalar> state,
			const scalar time_offset
		) : 
			gravitational_parameter(gravitational_parameter),
			initial_position(state.position),
			initial_velocity(state.velocity),
			time_offset(time_offset)
		{}

		Universals<scalar> advance(const scalar time_step) const
		{
			return Universals<scalar>(gravitational_parameter, initial_position, initial_velocity, time_offset+time_step);
		}

	};

}

