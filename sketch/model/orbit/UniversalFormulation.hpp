#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>     // *vec3
#include <glm/mat3x3.hpp>     // *mat3

// in-house libraries
#include "property/UniversalProperties.hpp"

#include "Elements.hpp"
#include "State.hpp"

namespace orbit {

template <typename scalar>
class UniversalVariableFormulation
{
	scalar C(const scalar y) const
	{
		scalar abs_y = std::abs(y);
		return y > scalar(0)? (scalar(1) - std::cos(std::sqrt(abs_y)))  / abs_y:
		                      (std::cosh(std::sqrt(abs_y)) - scalar(1)) / abs_y;
	}
	scalar S(const scalar y) const
	{
		scalar sqrt_abs_y = std::sqrt(std::abs(y));
		scalar sqrt_abs_y3 = sqrt_abs_y*sqrt_abs_y*sqrt_abs_y;
		return y > scalar(0)? (sqrt_abs_y - std::sin(sqrt_abs_y))  / sqrt_abs_y3 :
		                      (std::sinh(sqrt_abs_y) - sqrt_abs_y) / sqrt_abs_y3;
	}
public:
	const property::UniversalProperties<scalar> properties;
	const scalar laguerre_method_n;
	const int laguerre_iteration_count;
	UniversalVariableFormulation(property::UniversalProperties<scalar> properties):
		properties(properties),
		laguerre_method_n(6),
		laguerre_iteration_count(5),
	{

	}
	Universals<scalar> make(const glm::vec<3,scalar,glm::defaultp> position, const glm::vec<3,scalar,glm::defaultp> velocity)
	{
		return Universals<scalar>(position, velocity, 0.0);
	}
	Universals<scalar> make(
		const scalar semi_major_axis,
		const scalar eccentricity,
		const scalar inclination,
		const scalar longitude_of_ascending_node,
		const scalar argument_of_periapsis,
		const scalar mean_anomaly
	){ 
		const glm::vec<3,scalar,glm::defaultp> I = properties.vernal_equinox_direction;
		// const glm::vec<3,scalar,glm::defaultp> J = glm::cross(properties.north_pole_direction, properties.vernal_equinox_direction);
		const glm::vec<3,scalar,glm::defaultp> K = properties.north_pole_direction;

		scalar eccentric_anomaly = properties.solve_eccentric_anomaly_from_mean_anomaly        (elements.mean_anomaly, elements.eccentricity);
		scalar true_anomaly      = properties.get_true_anomaly_from_eccentric_anomaly          (eccentric_anomaly, elements.eccentricity);
		scalar semi_latus_rectum = properties.get_semi_latus_rectum_from_semi_major_axis_and_eccentricity (elements.semi_major_axis, elements.eccentricity);
		glm::vec<3,scalar,glm::defaultp> perifocal_position = properties.get_perifocal_position(semi_latus_rectum, elements.eccentricity, true_anomaly);
		glm::vec<3,scalar,glm::defaultp> perifocal_velocity = properties.get_perifocal_velocity(semi_latus_rectum, elements.eccentricity, true_anomaly);

		// NOTE: we use mat4x4 since it is the only thing that rotate() works with
        glm::mat<4,4,scalar,glm::defaultp> perifocal_to_reference_matrix(scalar(1.0));
        perifocal_to_reference_matrix = glm::rotate(perifocal_to_reference_matrix, elements.longitude_of_ascending_node, K);
        perifocal_to_reference_matrix = glm::rotate(perifocal_to_reference_matrix, elements.inclination,                 I);
        perifocal_to_reference_matrix = glm::rotate(perifocal_to_reference_matrix, elements.argument_of_periapsis,       K);

        glm::vec<4,scalar,glm::defaultp> position = perifocal_to_reference_matrix * glm::vec<4,scalar,glm::defaultp>(perifocal_position, scalar(1.0));
		glm::vec<4,scalar,glm::defaultp> velocity = perifocal_to_reference_matrix * glm::vec<4,scalar,glm::defaultp>(perifocal_velocity, scalar(1.0));
		// NOTE: we convert to vec4 since it is the only thing glm allows to multiply with mat4
        return State<scalar>(position, velocity);
	}
	Universals<scalar> tare(const Universals<scalar>& orbit) const
	{
		const auto n = laguerre_method_n;
		const auto mu = properties.gravitational_parameter;
		const auto sqrt_mu = std::sqrt(mu);
		const auto sigma0 = glm::dot(orbit.initial_position, orbit.initial_velocity) / sqrt_mu;
		const auto dt = orbit.time_offset;
		const auto r0 = glm::length(orbit.position);
		const auto v0 = glm::length(orbit.velocity);
		const auto a = scalar(2)/r0 - v0*v0/mu;
		const auto one = scalar(1);
		const auto n_one = n-one;

		// Conway & Prussing suggest that any value within upper and lower bounds is appropriate
		// upper and lower bounds are each defined by sqrt_mu*dt/r, 
		// where r is an upper or lower bound of radius
		// better estimates exist but require complex calculation,
		// however we do know that r0 is guaranteed to be between upper and lower bounds for r,
		// so we suffice to choose an x based on r0.
		scalar x = sqrt_mu*dt/r0;
		// temporary variables to improve performance:
		scalar x2, x3, ax2, Cax2, Sax2, one_r0a, F, dFdx, d2Fdx2;
		for (int i = 0; i < laguerre_iteration_count; ++i)
		{
			x2 = x*x;
			x3 = x*x2;
			ax2 = a*x2;
			Cax2 = C(ax2);
			Sax2 = S(ax2);
			one_r0a = one-r0*a;
			F      = sigma0 * x2 * Cax2 + one_r0a*x3 * Sax2 + r0*x - sqrt_mu*dt;
			dFdx   = sigma0*x*(one-ax2*Sax2) + one_r0a*x2*Cax2 + r0;
			d2Fdx2 = sigma0 - sigma0*ax2*(Cax2 - 3*Sax2) + (one_r0a/x)*(one - ax2*Sax2 - 2*Cax2)
			x -= n*F / 
				(dFdx + sign(dFdx) * std::sqrt(std::abs(n_one*n_one*dFdx*dFdx - n*n_one*F*d2Fdx2)));
		}

		glm::vec<4,scalar,glm::defaultp> r  = (1.0 - x2/r0 * Cax2) * r0 + 
		                                      ((t-t0) - x3/sqrt_mu * Sax2) * v0;
		glm::vec<4,scalar,glm::defaultp> v  = (x*sqrt_mu/(r*r0) * (ax2 * Sax2 - 1.0)) * r0 +
		                                      (1.0 - x2/r * Cax2) * v0;
	}
};

}