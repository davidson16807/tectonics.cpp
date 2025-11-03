#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>     // *vec3
#include <glm/mat3x3.hpp>     // *mat3

// in-house libraries
#include <math/special.hpp>

#include <model/orbit/Properties.hpp>

#include "Elements.hpp"
#include "State.hpp"

namespace orbit {

template <typename scalar>
struct ElementsAndState
{

	using vec3 = glm::vec<3,scalar,glm::defaultp>;
	using vec4 = glm::vec<4,scalar,glm::defaultp>;
	using mat4 = glm::mat<4,4,scalar,glm::defaultp>;

	const Properties<scalar> properties;

	ElementsAndState(Properties<scalar> properties): 
		properties(properties)
	{}

	Elements<scalar> get_elements_from_state(const State<scalar>& state, const scalar combined_mass)
	{
		vec3 angular_momentum_vector       = properties.get_angular_momentum_vector_from_position_and_velocity      (state.position, state.velocity);
		vec3 eccentricity_vector           = properties.get_eccentricity_vector_from_position_and_velocity          (state.position, state.velocity, combined_mass);
		vec3 node_vector                   = properties.get_node_vector_from_momentum_vector                        (angular_momentum_vector);
		scalar inclination                 = properties.get_inclination_from_momentum_vector                        (angular_momentum_vector);
		scalar longitude_of_ascending_node = properties.get_longitude_of_ascending_node_from_node_vector            (node_vector);
		scalar argument_of_periapsis       = properties.get_argument_of_periapsis_from_node_and_eccentricity        (node_vector, eccentricity_vector);
		scalar true_anomaly                = properties.get_true_anomaly_from_position_and_eccentricity             (state.position, eccentricity_vector);
		// scalar argument_of_latitude        = properties.get_argument_of_latitude_from_position_and_node             (position, node_vector);
		// scalar true_longitude              = properties.get_true_longitude                                          (longitude_of_ascending_node, argument_of_latitude);
		scalar eccentricity                = glm::length                                                            (eccentricity_vector);
		scalar eccentric_anomaly           = properties.get_eccentric_anomaly_from_true_anomaly                     (true_anomaly, eccentricity);
		scalar mean_anomaly                = properties.get_mean_anomaly_from_eccentric_anomaly                     (eccentric_anomaly, eccentricity);
		scalar semi_latus_rectum           = properties.get_semi_latus_rectum_from_momentum_vector                  (angular_momentum_vector, combined_mass);
		scalar semi_major_axis             = properties.get_semi_major_axis_from_semi_latus_rectum_and_eccentricity (semi_latus_rectum, eccentricity);
		return Elements<scalar>(
			semi_major_axis,
			eccentricity,
			inclination,
			longitude_of_ascending_node,
			argument_of_periapsis,
			mean_anomaly
		);
	}

	State<scalar> get_state_from_elements(const Elements<scalar>& elements, const scalar combined_mass)
	{

		const vec3 I = properties.vernal_equinox_direction;
		// const vec3 J = glm::cross(properties.north_pole_direction, properties.vernal_equinox_direction);
		const vec3 K = properties.north_pole_direction;

		scalar eccentric_anomaly = properties.solve_eccentric_anomaly_from_mean_anomaly(elements.mean_anomaly, elements.eccentricity);
		scalar true_anomaly      = properties.get_true_anomaly_from_eccentric_anomaly  (eccentric_anomaly, elements.eccentricity);
		scalar semi_latus_rectum = properties.get_semi_latus_rectum_from_semi_major_axis_and_eccentricity (elements.semi_major_axis, elements.eccentricity);
		vec3 perifocal_position  = properties.get_perifocal_position(semi_latus_rectum, elements.eccentricity, true_anomaly);
		vec3 perifocal_velocity  = properties.get_perifocal_velocity(semi_latus_rectum, elements.eccentricity, true_anomaly, combined_mass);

		// NOTE: we use mat4x4 since it is the only thing that rotate() works with
        mat4 perifocal_to_reference_matrix(scalar(1.0));
        perifocal_to_reference_matrix = glm::rotate(perifocal_to_reference_matrix, elements.longitude_of_ascending_node, K);
        perifocal_to_reference_matrix = glm::rotate(perifocal_to_reference_matrix, elements.inclination,                 I);
        perifocal_to_reference_matrix = glm::rotate(perifocal_to_reference_matrix, elements.argument_of_periapsis,       K);

        vec4 position = perifocal_to_reference_matrix * vec4(perifocal_position, scalar(1.0));
		vec4 velocity = perifocal_to_reference_matrix * vec4(perifocal_velocity, scalar(1.0));
		// NOTE: we convert to vec4 since it is the only thing glm allows to multiply with mat4
        return State<scalar>(position, velocity);

	}

};

}

