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
		using Universals = orbit::Universals<scalar>;
		using State = orbit::State<scalar>;

		static constexpr scalar s0 = scalar(0);
		static constexpr scalar s1 = scalar(1);
		static constexpr scalar s2 = scalar(2);
		static constexpr scalar s3 = scalar(3);

		scalar C(const scalar y) const
		{
			scalar Ay = std::abs(y);
			scalar SAy = std::sqrt(Ay);
			return (y >= s0? (s1 - cos(SAy)) 
			              : (cosh(SAy) - s1)) / Ay;
		}

		scalar S(const scalar y) const
		{
			scalar SAy = std::sqrt(std::abs(y));
			scalar SAy3 = SAy*SAy*SAy;
			return (y >= s0? (SAy - sin(SAy))
	                      : (sinh(SAy) - SAy)) / SAy3;
		}

		scalar dCax2dx(const scalar a, const scalar x) const
		{
			const scalar Ax = std::abs(x);
			const scalar Aa = std::abs(a);
			const scalar SAa = std::sqrt(Aa);
			const scalar sx = math::sign(x);
	        const scalar x2  = x*x;
	        const scalar x3  = x*x2;
	        const scalar ax2  = a * x2;
			return (ax2 >= s0? 
			     std::sin(SAa*Ax)*sx/(x2*SAa) - s2*(s1  - std::cos(SAa*Ax))/(x3*Aa)
			  : std::sinh(SAa*Ax)*sx/(x2*SAa) - s2*(-s1 + std::cosh(SAa*Ax))/(x3*Aa)
			);
		}

		scalar dSax2dx(const scalar a, const scalar x) const
		{
			const scalar Ax = std::abs(x);
			const scalar Aa = std::abs(a);
			const scalar SAa = std::sqrt(Aa);
			const scalar SAa3 = SAa*SAa*SAa;
			const scalar sx = math::sign(x);
	        const scalar x2  = x*x;
	        const scalar x3  = x*x2;
	        const scalar x4  = x*x3;
	        const scalar x5  = x*x4;
	        const scalar ax2  = a * x2;
			return (ax2 >= s0? 
			    ((-std::cos(SAa*Ax)*SAa*sx + SAa*sx)*Ax/(x4*SAa3) + (-std::sin(SAa*Ax) + SAa*Ax)*sx/(x4*SAa3) - 4*(-std::sin(SAa*Ax) + SAa*Ax)*Ax/(x5*SAa3))
			  : ((std::cosh(SAa*Ax)*SAa*sx - SAa*sx)*Ax/(x4*SAa3) + (std::sinh(SAa*Ax) - SAa*Ax)*sx/(x4*SAa3) - 4*(std::sinh(SAa*Ax) - SAa*Ax)*Ax/(x5*SAa3))
			);
		}

		scalar d2Cax2dx2(const scalar a, const scalar x) const
		{
			const scalar Ax = std::abs(x);
			const scalar Aa = std::abs(a);
			const scalar SAa = std::sqrt(Aa);
			const scalar sx = math::sign(x);
	        const scalar x2  = x*x;
	        const scalar ax2  = a * x2;
			return (ax2 >= s0? 
		        (std::cos(SAa*Ax)*sx*sx  - 4*std::sin(SAa*Ax)*sx/(x*SAa)  + 6*(s1 - std::cos (SAa*Ax))/(x2*Aa))/x2
   			  : (std::cosh(SAa*Ax)*sx*sx - 4*std::sinh(SAa*Ax)*sx/(x*SAa) - 6*(s1 - std::cosh(SAa*Ax))/(x2*Aa))/x2 
			);
		}

		scalar d2Sax2dx2(const scalar a, const scalar x) const
		{
			const scalar Ax = std::abs(x);
			const scalar Aa = std::abs(a);
			const scalar SAa = std::sqrt(Aa);
			const scalar SAa3 = SAa*SAa*SAa;
			const scalar sx = math::sign(x);
	        const scalar x2  = x*x;
	        const scalar x3  = x*x2;
	        const scalar x4  = x*x3;
	        const scalar ax2  = a * x2;
			return (ax2 >= s0? 
			      (- 2*(std::cos(SAa*Ax)  - 1)*sx*sx/Aa + (std::sin(SAa*Ax)*Aa*sx*sx )*Ax/SAa3 + 8*(std::sin(SAa*Ax)  - SAa*Ax)*sx/(x*SAa3) + 8*(std::cos(SAa*Ax)  - 1)*Ax*sx/(x*Aa) - 20*(std::sin(SAa*Ax)  - SAa*Ax)*Ax/(x2*SAa3))/x4
			    : (+ 2*(std::cosh(SAa*Ax) - 1)*sx*sx/Aa + (std::sinh(SAa*Ax)*Aa*sx*sx)*Ax/SAa3 - 8*(std::sinh(SAa*Ax) - SAa*Ax)*sx/(x*SAa3) - 8*(std::cosh(SAa*Ax) - 1)*Ax*sx/(x*Aa) + 20*(std::sinh(SAa*Ax) - SAa*Ax)*Ax/(x2*SAa3))/x4
			);
		}

	public:
	    scalar gravitational_constant;
	    int    max_refinement_count;
	    scalar max_precision;
	    int    laguerre_method_n;

	    UniversalPropagator(
	        scalar gravitational_constant,
	        int   max_refinement_count = 100,
	        scalar max_precision       = 1e-15,
	        int   laguerre_method_n    = 5
	    ) : 
	    	gravitational_constant(gravitational_constant),
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
	        // const scalar x4   = x * x3;
	        // const scalar x5   = x * x4;
	        // const scalar x4   = x * x3;
	        // const scalar a2   = a * a;
	        const scalar ax2  = a * x2;
	        const scalar one_r0a = s1 - r0 * a;
	        const scalar Cax2 = C(ax2);
	        const scalar Sax2 = S(ax2);
	        const scalar dCax2dx_ = dCax2dx(a,x);
	        const scalar dSax2dx_ = dSax2dx(a,x);
	        const scalar d2Cax2dx2_ = d2Cax2dx2(a,x);
	        const scalar d2Sax2dx2_ = d2Sax2dx2(a,x);

	        const scalar F    = sigma0 * x2 * Cax2 + one_r0a * x3 * Sax2 + r0 * x - dtsqrtmu;
	        const scalar dFdx = 
        	(
        		   r0
        		 + sigma0*x2*dCax2dx_
        		 + s2*sigma0*x*Cax2
        		 + x3*(-a*r0 + s1)*dSax2dx_
        		 + s3*x2*(-a*r0 + s1)*Sax2
        	);

	        const scalar n   = laguerre_method_n;
	        const scalar n_1 = n - s1;

	        const scalar d2Fdx2 = 
			(
				  sigma0*x2*d2Cax2dx2_
				+ 4*sigma0*x*dCax2dx_
				+ 2*sigma0*Cax2
				+ x3*one_r0a*d2Sax2dx2_
				+ 6*x2*one_r0a*dSax2dx_ 
				+ 6*x*one_r0a*Sax2
			);

	        return -scalar(n) * F / // Laguerre method
	        	(dFdx + math::sign(dFdx) * std::sqrt(std::abs(n_1 * n_1 * dFdx * dFdx - (n * n_1 * F * d2Fdx2))));
	    }

	    scalar solve(
    		const scalar x0, 
			const scalar a, 
			const scalar r0, 
			const scalar sigma0, 
			const scalar dtsqrtmu
		) const {
	        scalar x = x0;
	        for (int i = 0; i < max_refinement_count; ++i) {
	            const scalar dxdi = refine(x, a, r0, sigma0, dtsqrtmu);
	            if (dxdi != s0) {
	                x += dxdi;
	            }
	            if (std::abs(dxdi) < max_precision) {
	                return x;
	            }
	        }
	        return x;
	    }

	    State state(const Universals& orbit, const scalar t) const {
	        if (t == s0) {
	            return State(orbit.initial_position, orbit.initial_velocity);
	        }
	        const scalar t0 = orbit.time_offset;
	        const vec3  R0  = orbit.initial_position;
	        const vec3  V0  = orbit.initial_velocity;
	        const scalar mu = gravitational_constant * orbit.combined_mass;
	        const scalar sqrt_mu = std::sqrt(mu);
	        const scalar r0 = glm::length(R0);
	        const scalar v0 = glm::length(V0);
	        const scalar a  = s2 / r0 - (v0 * v0) / mu;
	        const scalar sma  = s1/a; // semi-major axis
	        const scalar dt = t - t0;
	        const scalar x = solve(
	        	/* x0 */ a >= s0? 
	        		  dt*sqrt_mu*a // elliptic
	        		: math::sign(dt) * std::sqrt(-sma) *
		                 std::log(
		                     -s2 * dt * mu /
		                     (sma*(glm::dot(R0, V0) +
		                       math::sign(dt) * std::sqrt(-mu*sma) * (s1 - r0 * a)))
		                 ), // hyperbolic
	        	a, r0, 
	        	/* sigma0 */    glm::dot(R0, V0) / sqrt_mu, 
	        	/* dtsqrtmu */  dt * sqrt_mu 
	        );
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
	        return State(R, V);
	    }

	    Universals advance(const Universals& orbit, const scalar t) const {
	        State state_ = state(orbit, t);
	        return Universals(orbit.combined_mass, s0, state_.position, state_.velocity);
	    }

	    Universals tare(const Universals& orbit) const {
	        return advance(orbit, orbit.time_offset);
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

