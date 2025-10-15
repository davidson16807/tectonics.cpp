#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

#include "State.hpp"

namespace body {

	/*
	Output of advance(t), tare(), and state() are conceptually pure methods, 
	i.e. they are free of side effects and their output is completely
	determiend by parameters and the state of `this`.

	Output of advance(t), tare(), and state() are always represented in 
	whatever units of measured that are used by parameters and `this`.
	It is the developer's responsibility to ensure consistency of units.
	*/
	template <typename scalar>
	struct Orbit
	{
		using vec3 = glm::vec<3,scalar,glm::defaultp>;

		const scalar gravitational_parameter;
		const vec3 initial_position;
		const vec3 initial_velocity;
		const scalar time_offset;

		Orbit(
			const scalar gravitational_parameter,
			const vec3 initial_position,
			const vec3 initial_velocity
		) : 
			initial_position(initial_position),
			initial_velocity(initial_velocity),
			time_offset(0)
		{}

		Orbit(
			const scalar gravitational_parameter,
			const State<scalar> state
		) : 
			initial_position(state.position),
			initial_velocity(state.velocity),
			time_offset(0)
		{}

		Orbit(
			const scalar gravitational_parameter,
			const vec3 initial_position,
			const vec3 initial_velocity,
			const scalar time_offset
		) : 
			initial_position(initial_position),
			initial_velocity(initial_velocity),
			time_offset(time_offset)
		{}

		Orbit(
			const scalar gravitational_parameter,
			const State<scalar> state,
			const scalar time_offset
		) : 
			initial_position(state.position),
			initial_velocity(state.velocity),
			time_offset(time_offset)
		{}

		Orbit<scalar> advance(const scalar time_step) const
		{
			return Orbit<scalar>(gravitational_parameter, initial_position, initial_velocity, time_offset+time_step);
		}

	};


	template <typename scalar>
	class OrbitPropagator
	{
		using vec3 = glm::vec<3,scalar,glm::defaultp>;

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
		const scalar laguerre_method_n;
		const int laguerre_iteration_count;
		OrbitPropagator():
			laguerre_method_n(6),
			laguerre_iteration_count(5)
		{

		}

		State<scalar> state(const Orbit<scalar> orbit, const scalar t) const
		{
			const auto n = laguerre_method_n;
			const auto mu = orbit.gravitational_parameter;
			const auto sqrt_mu = std::sqrt(mu);
			const auto sigma0 = glm::dot(orbit.initial_position, orbit.initial_velocity) / sqrt_mu;
			const auto dt = orbit.time_offset;
			const auto r0 = glm::length(orbit.position);
			const auto v0 = glm::length(orbit.velocity);
			const auto a = scalar(2)/r0 - v0*v0/mu;
			const auto one = scalar(1);
			const auto n_one = n-one;

			/*
			Conway & Prussing suggest that any value within upper and lower bounds is appropriate
			upper and lower bounds are each defined by sqrt_mu*dt/r, 
			where r is an upper or lower bound of radius
			better estimates exist but require complex calculation,
			however we do know that r0 is guaranteed to be between upper and lower bounds for r,
			so we suffice to choose an x based on r0.
			*/
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

			x2 = x*x;
			x3 = x*x2;
			vec3 r  = (1.0 - x2/r0 * Cax2) * r0 + 
			                                      ((t-t0) - x3/sqrt_mu * Sax2) * v0;
			vec3 v  = (x*sqrt_mu/(r*r0) * (ax2 * Sax2 - 1.0)) * r0 +
			                                      (1.0 - x2/r * Cax2) * v0;
			return State<scalar>(r,v);
		}

		Orbit<scalar> tare(const Orbit<scalar> orbit) const
		{
			return Orbit<scalar>(orbit.gravitational_parameter, state(orbit.time_offset));
		}

	};

}

