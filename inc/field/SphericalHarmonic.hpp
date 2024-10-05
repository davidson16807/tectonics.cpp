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
    `SphericalHarmonic<T,M,L>` is a field (i.e. it maps: ℝ³→ℝ).
    It is a class template that represents a single spherical harmonic of degree L and order -L≤M≤L.
    This is equivalent to Yₗₘ as defined here: https://shtools.github.io/SHTOOLS/fortran-real-spherical-harmonics.html
    This webpage is archived under research/math/shtools.pdf
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

