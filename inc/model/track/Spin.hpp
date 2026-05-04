#pragma once

// 3rd party libraries

#include <math/special.hpp>                  // math::floormod

#include <glm/vec3.hpp>       // *vec3
#include <glm/geometric.hpp>  // *vec3

#include <glm/gtc/matrix_transform.hpp> // glm::rotate

namespace track {

	template <typename scalar, typename duration>
	class Spin
	{

		static constexpr scalar pi = 3.14159265358979323848264338327950288419716;
		static constexpr scalar turn = scalar(2) * pi;

	public:

		using vec3 = glm::vec<3,scalar,glm::defaultp>;
		using mat3 = glm::mat<3,3,scalar,glm::defaultp>;
		using mat4 = glm::mat<4,4,scalar,glm::defaultp>;

		// the position of the north pole of the spin axis in local coordinates 
		// this is useful to model true polar wander, and to support different "up" axis standards
		const vec3 north_pole_in_local_space;
		// the position of the north pole of the spin axis in global coordinates when time_offset = 0 and initial_nutation_phase_in_radians = 0
		const vec3 initial_north_pole_in_global_space; 
		// the position in global coordinates about which the north pole rotates
		const vec3 precessional_north_pole_in_global_space;
		// 
		const scalar mean_axial_tilt_in_radians;
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
		const bool force_congruence;

		Spin(
			const vec3 north_pole_in_local_space,
			const vec3 initial_north_pole_in_global_space,
			const vec3 precessional_north_pole_in_global_space,
			const scalar mean_axial_tilt_in_radians,
			const scalar nutation_amplitude_in_radians,
			const scalar initial_nutation_phase_in_radians,
			const duration nutation_period,
			const duration precession_period,
			const duration spin_period,
			const duration time_offset,
			const bool force_congruence = true
		) : 
			north_pole_in_local_space(north_pole_in_local_space),
			initial_north_pole_in_global_space(initial_north_pole_in_global_space),
			precessional_north_pole_in_global_space(precessional_north_pole_in_global_space),
			mean_axial_tilt_in_radians(mean_axial_tilt_in_radians),
			nutation_amplitude_in_radians(nutation_amplitude_in_radians),
			initial_nutation_phase_in_radians(initial_nutation_phase_in_radians),
			nutation_period(nutation_period),
			precession_period(precession_period),
			spin_period(spin_period),
			time_offset(time_offset),
			force_congruence(force_congruence)
		{}

		mat3 orientation(const duration time_step) const
		{

			const scalar phase = math::floormod(time_step+time_offset, nutation_period*precession_period*spin_period);

			// TODO: apply time_offset to time_step, the code below assumes the result is stored in time_offset
			vec3 nutation_north_pole_in_global_space = glm::normalize(
				glm::cross(
					initial_north_pole_in_global_space,
					precessional_north_pole_in_global_space
				));

			// NOTE: we use mat4x4 since it is the only thing that rotate() works with
	        mat4 matrix1(scalar(1));
	        matrix1 = glm::rotate(matrix1, 
	        	mean_axial_tilt_in_radians + nutation_amplitude_in_radians * std::sin(initial_nutation_phase_in_radians + turn*phase/nutation_period),
	        	nutation_north_pole_in_global_space);
	        matrix1 = glm::rotate(matrix1, (turn * phase/precession_period), precessional_north_pole_in_global_space);
	        // TODO: use matrix1 to calculate north_pole_in_global_space

	        mat4 matrix2(scalar(1));
	        matrix2 = glm::rotate(matrix2, (turn * phase/spin_period), north_pole_in_local_space);
	        // TODO: multiply by a rotation matrix that moves north_pole_in_local_space to point at north_pole_in_global_space

	        return matrix2;
		}

		// Spin<scalar,duration> advance(const duration time_step) const
		// {
		// 	Spin<scalar,duration>(axis_radians, spin_period, time_offset+time_step);
		// }

		// Spin<scalar,duration> tare() const
		// {
		// 	return Spin<scalar,duration>(axis_radians, spin_period, time_offset);
		// }

	};

}

