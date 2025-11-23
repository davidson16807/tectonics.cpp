#pragma once

// 3rd party libraries
#include <glm/vec3.hpp>     // *vec3

#include <math/special.hpp>

#include "State.hpp"
#include "Universals.hpp"

namespace orbit {


	template <typename scalar>
	class UniversalPropagator {
		using vec3 = glm::vec<3,scalar,glm::defaultp>;

		static constexpr scalar s0 = scalar(0);
		static constexpr scalar s1 = scalar(1);
		static constexpr scalar s2 = scalar(2);
		static constexpr scalar s3 = scalar(3);

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
	    scalar standard_gravitational_parameter;
	    int    max_refinement_count;
	    scalar max_precision;
	    int    laguerre_method_n;

	    UniversalPropagator(
	        scalar standard_gravitational_parameter,
	        int   max_refinement_count = 10,
	        scalar max_precision       = 1e-10,
	        int   laguerre_method_n    = 5
	    ) : 
	    	standard_gravitational_parameter(standard_gravitational_parameter),
	    	max_refinement_count(max_refinement_count),
	    	max_precision(max_precision),
	    	laguerre_method_n(laguerre_method_n)
	    {
	    }

	    scalar refine(
    		const scalar x, 
			const scalar a, 
			const scalar r0, 
			const scalar sigma0, 
			const scalar dtsqrtmu
		) const {
	        const scalar x2   = x * x;
	        const scalar x3   = x * x2;
	        const scalar ax2  = a * x2;
	        const scalar Cax2 = C(ax2);
	        const scalar Sax2 = S(ax2);
	        const scalar one_r0a = s1 - r0 * a;

	        const scalar F    = sigma0 * x2 * Cax2 + one_r0a * x3 * Sax2 + r0 * x - dtsqrtmu;
	        const scalar dFdx = sigma0 * x * (s1 - ax2 * Sax2) + one_r0a * x2 * Cax2 + r0;

	        const int n   = this->laguerre_method_n;
	        const int n_1 = n - 1;

	        const scalar d2Fdx2 =
	            sigma0 - sigma0 * ax2 * (Cax2 - s3 * Sax2) +
	            (one_r0a / x) *
	                (s1 - ax2 * Sax2 - s2 * Cax2);

	        const scalar radicand = n_1 * n_1 * dFdx * dFdx - (n * n_1) * F * d2Fdx2;

	        return -scalar(n) * F / // Laguerre method
	        	(dFdx + math::sign(dFdx) * std::sqrt(std::abs(radicand)));
	    }

	    scalar solve(
    		const scalar x0, 
			const scalar a, 
			const scalar r0, 
			const scalar sigma0, 
			const scalar dtsqrtmu
		) const {
	        scalar x = x0;
	        for (int i = 0; i < this->max_refinement_count; ++i) {
	            const scalar dxdi = this->refine(x, a, r0, sigma0, dtsqrtmu);
	            if (dxdi != s0) {
	                x += dxdi;
	            }
	            if (std::abs(dxdi) < this->max_precision) {
	                return x;
	            }
	        }
	        return x;
	    }

	    State<scalar> state(const orbit::Universals<scalar>& orbit, scalar t) const {
	        if (t == s0) {
	            return State(orbit.initial_position, orbit.initial_velocity);
	        }
	        const scalar t0 = orbit.time_offset;
	        const vec3  R0  = orbit.initial_position;
	        const vec3  V0  = orbit.initial_velocity;
	        const scalar mu      = this->standard_gravitational_parameter * orbit.combined_mass;
	        const scalar sqrt_mu = std::sqrt(mu);
	        const scalar r0 = glm::length(R0);
	        const scalar v0 = glm::length(V0);
	        const scalar a = s2 / r0 - (v0 * v0) / mu;
	        const scalar dt = t - t0;
	        scalar x0;
	        if (a > s0) { // elliptic
	            x0 = dt * sqrt_mu * a;
	        } else {             // hyperbolic
	            x0 = math::sign(dt) * std::sqrt(-a) *
	                 std::log(
	                     -s2 * mu * dt /
	                     (a * (glm::dot(R0, V0) +
	                           math::sign(dt) * std::sqrt(-mu * a) * (s1 - r0 / a)))
	                 );
	        }
	        const scalar sigma0    = glm::dot(R0, V0) / sqrt_mu;
	        const scalar dtsqrtmu  = dt * sqrt_mu;
	        const scalar x         = this->solve(x0, a, r0, sigma0, dtsqrtmu);
	        const scalar x2   = x * x;
	        const scalar x3   = x * x2;
	        const scalar ax2  = a * x2;
	        const scalar Cax2 = C(ax2);
	        const scalar Sax2 = S(ax2);
	        const vec3 R =
	            (s1 - x2 / r0 * Cax2) * R0 +
	            (dt - x3 / sqrt_mu * Sax2) * V0;
	        const scalar r = glm::length(R);
	        const vec3 V =
	            (x * sqrt_mu / (r * r0) * (ax2 * Sax2 - s1)) * R0 +
	            (s1 - x2 / r * Cax2) * V0;
	        return State<scalar>(R, V);
	    }

	    orbit::Universals<scalar> advance(const orbit::Universals<scalar>& orbit, const scalar t) const {
	        State<scalar> state_ = this->state(orbit, t);
	        return orbit::Universals<scalar>(
	        	orbit.combined_mass,
            	s0,
            	state_.position,
            	state_.velocity);
	    }

	    orbit::Universals<scalar> tare(const orbit::Universals<scalar>& orbit) const {
	        return this->advance(orbit, orbit.time_offset);
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

