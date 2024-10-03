#pragma once

// C libraries
#include <cmath>

// in-house libraries
#include <math/analytic/Polynomial.hpp>
#include <math/combinatorics.hpp>

namespace field {

    /* 
    `SphericalHarmonic<T,M,L>` is a class template that represents a single spherical harmonic of degree L and order -L≤M≤L.
    This is equivalent to Yᵐₗ as defined here: https://shtools.github.io/SHTOOLS/fortran-real-spherical-harmonics.html
    or as archived under research/math/shtools.pdf
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
                std::sqrt(std::pow(T(1) - z*z, T(lMl))) * 
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


/*
    template<typename T, int Plo, int Phi>
    constexpr std::string to_string(const SphericalHarmonic<T,Plo,Phi>& p)
    {
        // const std::string exponents("⁰¹²³⁴⁵⁶⁷⁸⁹");
        std::string output;
        for (int i = Plo; i <= Phi; ++i)
        {
            output += std::to_string(p[i]);
            output += i==0?       "" : "x";
            output += 0<=i&&i<=1? "" : "^" + std::to_string(i);
            output += i>=Phi?     "" : " + ";
        }
        return output;
    }

    template<typename T, int Plo, int Phi>
    std::ostream& operator<<(std::ostream& os, const SphericalHarmonic<T,Plo,Phi>& p) { 
        os << to_string(p);
        return os;
    }

    template<typename T, int Plo, int Phi>
    constexpr SphericalHarmonic<T,Plo,Phi> operator*(const SphericalHarmonic<T,Plo,Phi>& p, const T k)
    {
        SphericalHarmonic<T,Plo,Phi> y(p);
        y *= k;
        return y;
    }

    template<typename T, int Plo, int Phi>
    constexpr SphericalHarmonic<T,Plo,Phi> operator*(const T k, const SphericalHarmonic<T,Plo,Phi>& p)
    {
        SphericalHarmonic<T,Plo,Phi> y(p);
        y *= k;
        return y;
    }

    template<typename T, int Plo, int Phi>
    constexpr SphericalHarmonic<T,Plo,Phi> operator/(const SphericalHarmonic<T,Plo,Phi>& p, const T k)
    {
        SphericalHarmonic<T,Plo,Phi> y(p);
        y /= k;
        return y;
    }

    template<typename T, int Plo, int Phi>
    constexpr SphericalHarmonic<T,Plo,Phi> operator-(const SphericalHarmonic<T,Plo,Phi>& p)
    {
        SphericalHarmonic<T,Plo,Phi> y(p);
        y *=-1.0f;
        return y;
    }

    // operators that are not closed under SphericalHarmonic<T,Plo,Phi> relations, but produce SphericalHarmonic<T,> relations of some degree
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr SphericalHarmonic<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> operator+(const SphericalHarmonic<T,Plo,Phi>& p, const SphericalHarmonic<T,P,Q>& q)
    {
        SphericalHarmonic<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> y(p);
        y += q;
        return y;
    }

    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr SphericalHarmonic<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> operator-(const SphericalHarmonic<T,Plo,Phi>& p, const SphericalHarmonic<T,P,Q>& q)
    {
        SphericalHarmonic<T,std::min(Plo,Qlo),std::max(Phi,Qhi)> y(p);
        y -= q;
        return y;
    }



*/


}

