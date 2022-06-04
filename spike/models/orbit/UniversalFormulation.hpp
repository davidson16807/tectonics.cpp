#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>     // *vec3
#include <glm/mat3x3.hpp>     // *mat3

// in-house libraries
#include <math/lerp.hpp>

#include "property/UniversalProperties.hpp"

#include "Elements.hpp"
#include "State.hpp"

namespace orbit {

template <typename Tfloat>
struct UniversalVariableFormulation
{
	const property::UniversalVariableProperties<Tfloat> properties;
	UniversalVariableFormulation(property::UniversalVariableProperties<Tfloat> properties):
		properties(properties)
	{}
	Universal<Tfloat> make(const glm::vec<3,Tfloat,glm::defaultp> position, const glm::vec<3,Tfloat,glm::defaultp> velocity)
	{
		return Universal<Tfloat>(position, velocity, 0.0);
	}
	Universal<Tfloat> make(
		const Tfloat semi_major_axis,
		const Tfloat eccentricity,
		const Tfloat inclination,
		const Tfloat longitude_of_ascending_node,
		const Tfloat argument_of_periapsis,
		const Tfloat mean_anomaly
	){ 
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
	Universal<Tfloat> tare(const Universal<Tfloat>& orbit){
		
	}
};

}