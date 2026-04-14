#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

namespace track {

	template <typename scalar, typename duration>
	struct Spin
	{
		using vec3 = glm::vec<3,scalar,glm::defaultp>;
		using mat3 = glm::mat<3,3,scalar,glm::defaultp>;

		// the position of the north pole of the spin axis in local coordinates 
		// this is useful to model true polar wander, and to support different "up" axis standards
		const vec3 north_pole_in_local_space; 
		// the position of the north pole of the spin axis in global coordinates when time_offset = 0 and initial_nutation_phase_in_radians = 0
		const vec3 initial_north_pole_in_global_space; 
		// the position in global coordinates about which the north pole rotates
		const vec3 precessional_north_pole_in_global_space;
		// the amount by which precessional_north_pole_in_global_space differs differs during nutation
		// when precessional_north_pole_in_global_space is measured as radians from initial_north_pole_in_global_space
		const scalar nutation_amplitude_in_radians; 
		// the phase of nutation in radians when time_offset = 0
		const duration initial_nutation_phase_in_radians;
		// the time it takes to 
		const duration nutation_period; 
		const duration precession_period;
		const duration spin_period;
		const duration time_offset;

		Spin(
			const vec3 north_pole_in_local_space,
			const vec3 initial_north_pole_in_global_space,
			const vec3 precessional_north_pole_in_global_space,
			const scalar nutation_amplitude_in_radians,
			const duration initial_nutation_phase_in_radians,
			const duration nutation_period,
			const duration precession_period,
			const duration spin_period,
			const duration time_offset
		) : 
			north_pole_in_local_space(north_pole_in_local_space),
			initial_north_pole_in_global_space(initial_north_pole_in_global_space),
			precessional_north_pole_in_global_space(precessional_north_pole_in_global_space),
			nutation_amplitude_in_radians(nutation_amplitude_in_radians),
			initial_nutation_phase_in_radians(initial_nutation_phase_in_radians),
			nutation_period(nutation_period),
			precession_period(precession_period),
			spin_period(spin_period),
			time_offset(time_offset)
		{}

		mat3 orientation(const duration time_step) const
		{
			return glm::rotate(...);
			// NOTE: we use mat4x4 since it is the only thing that rotate() works with
	        mat4 perifocal_to_reference_matrix(scalar(1));
	        perifocal_to_reference_matrix = glm::rotate(perifocal_to_reference_matrix, elements.longitude_of_ascending_node, K);
	        perifocal_to_reference_matrix = glm::rotate(perifocal_to_reference_matrix, elements.inclination,                 I);
	        perifocal_to_reference_matrix = glm::rotate(perifocal_to_reference_matrix, elements.argument_of_periapsis,       K);
			vec4 velocity = perifocal_to_reference_matrix * vec4(perifocal_velocity, scalar(0));
		}

		Spin<scalar,duration> advance(const duration time_step) const
		{
			Spin<scalar,duration>(axis_radians, spin_period, time_offset+time_step);
		}

		Spin<scalar,duration> tare() const
		{
			return Spin<scalar,duration>(axis_radians, spin_period, math::modulus(time_offset, spin_period));
		}

	};

}
