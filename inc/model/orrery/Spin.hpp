#pragma once

// 3rd party libraries

#include <math/special.hpp>                  // math::floormod

#include <glm/vec3.hpp>       // *vec3
#include <glm/geometric.hpp>  // *vec3

#include <glm/gtc/matrix_transform.hpp> // glm::rotate

namespace orrery {

	template <typename scalar, typename duration>
	class Spin
	{

		static constexpr scalar pi = 3.14159265358979323848264338327950288419716;
		static constexpr scalar turn = scalar(2) * pi;
		static constexpr scalar s0 = scalar(0);
		static constexpr scalar s1 = scalar(1);

	public:

		using vec3 = glm::vec<3,scalar,glm::defaultp>;
		using mat3 = glm::mat<3,3,scalar,glm::defaultp>;
		using mat4 = glm::mat<4,4,scalar,glm::defaultp>;

		// the position of the north pole of the spin axis in local coordinates 
		// this is useful to model true polar wander, and to support different "up" axis standards
		vec3 north_pole_in_local_space;
		// the position of the north pole of the spin axis in global coordinates when time_offset = 0 and initial_nutation_phase_in_radians = 0
		vec3 initial_north_pole_in_global_space; 
		// the position in global coordinates about which the north pole rotates
		vec3 precessional_north_pole_in_global_space;
		// 
		scalar mean_axial_tilt_in_radians;
		// the amount by which precessional_north_pole_in_global_space differs differs during nutation
		// when precessional_north_pole_in_global_space is measured as radians from initial_north_pole_in_global_space
		scalar nutation_amplitude_in_radians; 
		// the phase of nutation in radians when time_offset = 0
		duration initial_nutation_phase_in_radians;
		// the time it takes to 
		duration nutation_period; 
		duration precession_period;
		duration spin_period;
		duration time_offset;
		bool force_congruence;

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

		// constructor that assumes defaults for nutation
		Spin(
			const vec3 north_pole_in_local_space,
			const vec3 initial_north_pole_in_global_space,
			const vec3 precessional_north_pole_in_global_space,
			const scalar mean_axial_tilt_in_radians,
			const duration precession_period,
			const duration spin_period,
			const duration time_offset,
			const bool force_congruence = true
		) : 
			north_pole_in_local_space(north_pole_in_local_space),
			initial_north_pole_in_global_space(initial_north_pole_in_global_space),
			precessional_north_pole_in_global_space(precessional_north_pole_in_global_space),
			mean_axial_tilt_in_radians(mean_axial_tilt_in_radians),
			nutation_amplitude_in_radians(0),
			initial_nutation_phase_in_radians(0),
			nutation_period(0),
			precession_period(precession_period),
			spin_period(spin_period),
			time_offset(time_offset),
			force_congruence(force_congruence)
		{}

		// constructor that assumes defaults for axial tilt, precession, and nutation
		Spin(
			const vec3 north_pole_in_local_space,
			const vec3 initial_north_pole_in_global_space,
			const duration spin_period,
			const duration time_offset,
			const bool force_congruence = true
		) : 
			north_pole_in_local_space(north_pole_in_local_space),
			initial_north_pole_in_global_space(initial_north_pole_in_global_space),
			precessional_north_pole_in_global_space(initial_north_pole_in_global_space),
			mean_axial_tilt_in_radians(0),
			nutation_amplitude_in_radians(0),
			initial_nutation_phase_in_radians(0),
			nutation_period(0),
			precession_period(0),
			spin_period(spin_period),
			time_offset(time_offset),
			force_congruence(force_congruence)
		{}

		// default constructor, needed for ECS component lists
		Spin() : 
			north_pole_in_local_space(),
			initial_north_pole_in_global_space(),
			precessional_north_pole_in_global_space(),
			mean_axial_tilt_in_radians(0),
			nutation_amplitude_in_radians(0),
			initial_nutation_phase_in_radians(0),
			nutation_period(0),
			precession_period(0),
			spin_period(0),
			time_offset(0),
			force_congruence(false)
		{}

		mat3 inertial_for_fixed(const duration time_step) const
		{

			const scalar period = 
				(nutation_period > s0? nutation_period : s1) *
				(precession_period > s0? precession_period : s1) *
				spin_period;

			const scalar time = force_congruence? 
				math::floormod(time_step+time_offset, period) 
			  : time_step+time_offset;

			// TODO: apply time_offset to time_step, the code below assumes the result is stored in time_offset
			vec3 nutation_north_pole_in_global_space = glm::normalize(
				glm::cross(
					initial_north_pole_in_global_space,
					precessional_north_pole_in_global_space
				));

	        mat4 rotation(scalar(1));

	        scalar precession_phase = precession_period > s0? turn * time/precession_period : s0;
	        scalar nutation_phase = nutation_period > s0? initial_nutation_phase_in_radians + turn*time/nutation_period : s0;

			// NOTE: we use mat4x4 since it is the only thing that rotate() works with
	        rotation = glm::rotate(rotation, precession_phase, precessional_north_pole_in_global_space);
	        rotation = glm::rotate(rotation, mean_axial_tilt_in_radians + nutation_amplitude_in_radians * std::sin(nutation_phase), nutation_north_pole_in_global_space);
	        rotation = glm::rotate(rotation, (turn * time/spin_period), north_pole_in_local_space);
	        return rotation;

		}

		mat3 fixed_for_inertial(const duration time_step) const
		{
	        return glm::transpose(inertial_for_fixed(time_step));
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

