#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

#include <math/special.hpp>

#include "State.hpp"
#include "Universals.hpp"

namespace orbit {

	template <typename scalar>
	class UniversalPropagator
	{
		using vec3 = glm::vec<3,scalar,glm::defaultp>;

		static constexpr scalar s1 = scalar(0);
		static constexpr scalar s1 = scalar(1);
		static constexpr scalar s2 = scalar(2);

		scalar C(const scalar y) const
		{
			scalar abs_y = std::abs(y);
			return y > s0? (s1 - std::cos(std::sqrt(abs_y)))  / abs_y:
			               (std::cosh(std::sqrt(abs_y)) - s1) / abs_y;
		}

		scalar S(const scalar y) const
		{
			scalar sqrt_abs_y = std::sqrt(std::abs(y));
			scalar sqrt_abs_y3 = sqrt_abs_y*sqrt_abs_y*sqrt_abs_y;
			return y > s0? (sqrt_abs_y - std::sin(sqrt_abs_y))  / sqrt_abs_y3 :
			                      (std::sinh(sqrt_abs_y) - sqrt_abs_y) / sqrt_abs_y3;
		}

	public:

		const scalar laguerre_method_n;
		const int refinement_count;

		UniversalPropagator():
			laguerre_method_n(6),
			refinement_count(5)
		{

		}

		State<scalar> state(const Orbit<scalar> orbit, const scalar t) const
		{
			const auto t0 = orbit.time_offset;
			const auto mu = standard_gravitation_parameter * orbit.combined_mass;
			const auto sqrt_mu = std::sqrt(mu);
			const auto R0 = orbit.initial_position;
			const auto V0 = orbit.initial_velocity;
			const auto r0 = glm::length(R0);
			const auto v0 = glm::length(V0);
			const auto sigma0 = glm::dot(R0, V0) / sqrt_mu;
			const auto dt = t - t0;
			const auto a = s2/r0 - v0*v0/mu;
			// const auto n = laguerre_method_n;
			// const auto n_1 = n-s1;

			/*
			Conway & Prussing suggest that any value within upper and lower bounds is appropriate
			upper (r⁺) and lower (r⁻) bounds are each defined by r± = sqrt_mu*dt/r∓.
			We do know that r0 is guaranteed to be between upper and lower bounds for r,
			so we suffice to choose an x based on r0.
			*/
			scalar x = sqrt_mu*dt/r0;
			// temporary variables to improve performance:
			scalar x2, x3, ax2, Cax2, Sax2, one_r0a, F, dFdx;//, d2Fdx2;
			for (int i = 0; i < refinement_count; ++i)
			{
				x2 = x*x;
				x3 = x*x2;
				ax2 = a*x2;
				Cax2 = C(ax2);
				Sax2 = S(ax2);
				one_r0a = s1-r0*a;
				F      = sigma0 * x2 * Cax2 + one_r0a*x3 * Sax2 + r0*x - sqrt_mu*dt;
				dFdx   = sigma0*x*(s1-ax2*Sax2) + one_r0a*x2*Cax2 + r0;
				// Newton's method:
				x -= F/dFdx;
				// Laguerre method:
				// d2Fdx2 = sigma0 - sigma0*ax2*(Cax2 - 3*Sax2) + (one_r0a/x)*(s1 - ax2*Sax2 - 2*Cax2);
				// x -= n*F / 
				// 	(dFdx + math::sign(dFdx) * std::sqrt(std::abs(n_1*n_1*dFdx*dFdx - n*n_1*F*d2Fdx2)));
			}

			x2 = x*x;
			x3 = x*x2;
			vec3 r  = (s1 - x2/r0 * Cax2) * R0 + 
			          ((t-t0) - x3/sqrt_mu * Sax2) * V0;
			vec3 v  = (x*sqrt_mu/(r*r0) * (ax2 * Sax2 - 1.0)) * R0 +
			                                      (1.0 - x2/r * Cax2) * V0;
			return State<scalar>(r,v);
		}

		Orbit<scalar> tare(const Orbit<scalar> orbit) const
		{
			return Orbit<scalar>(orbit.combined_mass, state(orbit, orbit.time_offset));
		}

	};

}

/*

We need Orbit to do the following:
1 allow queries over phases, such as to find average insolations over many years
2 apply force to create a new orbit without updating or changing the epoch used in orbits for other bodies

1) is accomplished using `state(t)`, however since `Orbit` can also represent hyperbolic or parabolic orbits,
time t must be selected according to phase outside of logic for `Orbit`. This behavior is still acceptable.

2) is accomplished such that, when applying a change in velocity Δv, 
the new state s+Δv is used to create an orbit Orbit(s+Δv, -t₀), where t₀ is time since the epoch started 
time t₀ is tracked outside the Orbits so that Orbits do not need update every timestep. 


advance is group-like (trivial):
	o.advance(0) = o
	o.advance(s+t) = o.advance(s).advance(t)
	o.advance(t).advance(-t) = t
behavioral equality (trivial):
	o.advance(0).state(0) ≈ o.state(0)

tare is idempotent:
	o.tare().tare() = o.tare()
invariance under reference time:
	o.state(t) ≈ o.tare().state(t)
continuity:
	d(o.state(ε), o.state(0)) < d(o.state(2ε), o.state(0)) 

preservation of combined mass:
	(o.advance(t)).combined_mass == o.combined_mass
	(o.tare()).combined_mass == o.combined_mass

energy is conserved:
	E = v²/2 - μ/|r|
angular momentum is conserved:
	h = r × v

radius is constant when eccentricity=0
	glm::length(o.advance(t).position) = glm::length(o.position)

*/

