#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>      // *vec3
#include <glm/geometric.hpp> // glm::cross

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
	template <typename scalar, glm::qualifier=glm::defaultp>
	struct Universals
	{
		using vec3 = glm::vec<3,scalar,precision>;

		const scalar combined_mass;
		const vec3 initial_position;
		const vec3 initial_velocity;
		const scalar time_offset;

		constexpr Universals(
			const scalar combined_mass,
			const vec3& initial_position,
			const vec3& initial_velocity
		) noexcept : 
			combined_mass(combined_mass),
			initial_position(initial_position),
			initial_velocity(initial_velocity),
			time_offset(0)
		{}

		constexpr Universals(
			const scalar combined_mass,
			const State<scalar>& state
		) noexcept : 
			combined_mass(combined_mass),
			initial_position(state.position),
			initial_velocity(state.velocity),
			time_offset(0)
		{}

		constexpr Universals(
			const scalar combined_mass,
			const scalar time_offset,
			const vec3& initial_position,
			const vec3& initial_velocity
		) noexcept : 
			combined_mass(combined_mass),
			initial_position(initial_position),
			initial_velocity(initial_velocity),
			time_offset(time_offset)
		{}

		constexpr Universals(
			const scalar combined_mass,
			const State<scalar>& state,
			const scalar time_offset
		) noexcept : 
			combined_mass(combined_mass),
			initial_position(state.position),
			initial_velocity(state.velocity),
			time_offset(time_offset)
		{}

		vec3 angular_momentum_vector() const noexcept {
			return glm::cross(initial_position, initial_velocity);
		}

		constexpr State<scalar> state() const noexcept {
			return State<scalar>(initial_position, initial_velocity);
		}

		constexpr Universals<scalar> advance(const scalar time_step) const noexcept
		{
			return Universals<scalar>(combined_mass, time_offset+time_step, initial_position, initial_velocity);
		}

	};

}

