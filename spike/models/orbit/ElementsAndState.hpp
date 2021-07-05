#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>     // *vec3
#include <glm/mat3x3.hpp>     // *mat3

// in-house libraries
#include "property/Properties.hpp"

#include "Elements.hpp"
#include "State.hpp"

namespace orbit {

template <typename Tfloat>
struct ElementsAndState
{
	const property::Properties<Tfloat> properties;
	ElementsAndState(property::Properties<Tfloat> properties): 
		properties(properties)
	{}
	Elements<Tfloat> get_elements_from_state(const State<Tfloat>& state)
	{
		glm::vec<3,Tfloat,glm::defaultp> angular_momentum_vector  = properties.get_angular_momentum_vector_from_position_and_velocity      (state.position, state.velocity);
		glm::vec<3,Tfloat,glm::defaultp> eccentricity_vector      = properties.get_eccentricity_vector_from_position_and_velocity          (state.position, state.velocity);
		glm::vec<3,Tfloat,glm::defaultp> node_vector              = properties.get_node_vector_from_momentum_vector                        (angular_momentum_vector);
		Tfloat inclination                 = properties.get_inclination_from_momentum_vector                        (angular_momentum_vector);
		Tfloat longitude_of_ascending_node = properties.get_longitude_of_ascending_node_from_node_vector            (node_vector);
		Tfloat argument_of_periapsis       = properties.get_argument_of_periapsis_from_node_and_eccentricity        (node_vector, eccentricity_vector);
		Tfloat true_anomaly                = properties.get_true_anomaly_from_position_and_eccentricity             (state.position, eccentricity_vector);
		// Tfloat argument_of_latitude        = properties.get_argument_of_latitude_from_position_and_node             (position, node_vector);
		// Tfloat true_longitude              = properties.get_true_longitude                                          (longitude_of_ascending_node, argument_of_latitude);
		Tfloat eccentricity                = glm::length                                                            (eccentricity_vector);
		Tfloat eccentric_anomaly           = properties.solve_eccentric_anomaly_from_true_anomaly                   (true_anomaly, eccentricity);
		Tfloat mean_anomaly                = properties.get_mean_anomaly_from_eccentric_anomaly                     (eccentric_anomaly, eccentricity);
		Tfloat semi_latus_rectum           = properties.get_semi_latus_rectum_from_momentum_vector                  (angular_momentum_vector);
		Tfloat semi_major_axis             = properties.get_semi_major_axis_from_semi_latus_rectum_and_eccentricity (semi_latus_rectum, eccentricity);
		return Elements<Tfloat>(
			semi_major_axis,
			eccentricity,
			inclination,
			longitude_of_ascending_node,
			argument_of_periapsis,
			mean_anomaly
		);
	}
	State<Tfloat> get_state_from_elements(const Elements<Tfloat>& elements)
	{
		const glm::vec<3,Tfloat,glm::defaultp> I = properties.vernal_equinox_direction;
		// const glm::vec<3,Tfloat,glm::defaultp> J = glm::cross(properties.north_pole_direction, properties.vernal_equinox_direction);
		const glm::vec<3,Tfloat,glm::defaultp> K = properties.north_pole_direction;

		Tfloat eccentric_anomaly = properties.solve_eccentric_anomaly_from_mean_anomaly        (elements.mean_anomaly, elements.eccentricity);
		Tfloat true_anomaly          = properties.get_true_anomaly_from_eccentric_anomaly      (eccentric_anomaly, elements.eccentricity);
		Tfloat semi_latus_rectum = properties.get_semi_latus_rectum_from_semi_major_axis_and_eccentricity (elements.semi_major_axis, elements.eccentricity);
		glm::vec<3,Tfloat,glm::defaultp> perifocal_position = properties.get_perifocal_position(semi_latus_rectum, elements.eccentricity, true_anomaly);
		glm::vec<3,Tfloat,glm::defaultp> perifocal_velocity = properties.get_perifocal_velocity(semi_latus_rectum, elements.eccentricity, true_anomaly);

		// NOTE: we use mat4x4 since it is the only thing that rotate() works with
        glm::mat<4,4,Tfloat,glm::defaultp> perifocal_to_reference_matrix(Tfloat(1.0));
        perifocal_to_reference_matrix = glm::rotate(perifocal_to_reference_matrix, elements.longitude_of_ascending_node, K);
        perifocal_to_reference_matrix = glm::rotate(perifocal_to_reference_matrix, elements.inclination,                 I);
        perifocal_to_reference_matrix = glm::rotate(perifocal_to_reference_matrix, elements.argument_of_periapsis,       K);

        glm::vec<4,Tfloat,glm::defaultp> position = perifocal_to_reference_matrix * glm::vec<4,Tfloat,glm::defaultp>(perifocal_position, Tfloat(1.0));
		glm::vec<4,Tfloat,glm::defaultp> velocity = perifocal_to_reference_matrix * glm::vec<4,Tfloat,glm::defaultp>(perifocal_velocity, Tfloat(1.0));
		// NOTE: we convert to vec4 since it is the only thing glm allows to multiply with mat4
        return State<Tfloat>(position, velocity);
	}
};

}