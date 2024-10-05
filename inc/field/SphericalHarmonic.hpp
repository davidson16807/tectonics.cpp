#pragma once

// C libraries
#include <cmath>

// 3rd-party libraries
#include <glm/vec3.hpp>

// in-house libraries
#include <math/analytic/Polynomial.hpp>
#include <math/combinatorics.hpp>

namespace field {

    /* 
    `SphericalHarmonic<T,M,L>` is a field (i.e. it maps: ℝ³→ℝ for some representation of ℝ, `T`).
    It is a class template that represents a single 4π-normalized spherical harmonic of degree L and order -L≤M≤L.
    It implements the following for a unit vector with longitude of ϕ and latitude of asin(z):

        Yₗₘ(z,ϕ) = 
              ⎧ Pₗ|ₘ|(z) cos(|m|ϕ) if m≥0
              ⎩ Pₗ|ₘ|(z) sin(|m|ϕ) if m<0

    where:

        Pₗₘ(z) = pₗₘ(z) √((2-δₘ₀) (2l+1) (l-m)!/(l+m)!)
            is the 4π-normalized associated Legendre polynomial of degree l and order m

        pₗₘ(z) = (1-z²)ᵐᐟ² dᵐ/dzᵐ pₗ(z) 
            is the unnormalized associated Legendre polynomial of degree l and order m

        pₗ(z) = dˡ/dzˡ (z²-1)ˡ/(2ˡl!) 
            is the lth standard Legendre polynomial 
            (`analytic::legendre_polynomial<L>` in math/analytic/Polynomial.hpp)

        δᵢⱼ = 
              ⎧ 1 if i=j
              ⎩ 0 if i≠j
            is the Kronecker delta function
    */
    template<typename T, int M, int L>
    struct SphericalHarmonic
    {
        using value_type = T;

        constexpr SphericalHarmonic()
        {
        }

        T call(const T z, const T phi) const
        {
            const int lMl = M>=0? M:-M;
            return 
                std::sqrt(
                    T(2-(lMl==0)) *
                    T(2*L+1) *
                    combinatoric::factorial(L-lMl) / 
                    combinatoric::factorial(L+lMl)
                ) * 
                std::sqrt(std::pow(T(1 - z*z), T(lMl)/T(2))) * 
                analytic::polynomial(
                    analytic::higher_order_derivative<lMl>(analytic::legendre_polynomial<T,L>())
                )(z) * 
                (M>=0? std::cos(lMl*phi) : std::sin(lMl*phi))
            ;
        }

        inline T operator()(const T z, const T phi) const
        {
            return call(z, phi);
        }

        inline T operator()(const T x, const T y, const T z) const
        {
            return call(z, std::atan2(y,x));
        }

        template<int N, glm::qualifier Q=glm::defaultp>
        inline T operator()(const glm::vec<N,T,Q> v) const
        {
            auto u = glm::normalize(v);
            return call(u.z, std::atan2(u.y, u.x));
        }

    };

}

