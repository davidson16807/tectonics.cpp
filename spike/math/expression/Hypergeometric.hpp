#pragma once

namespace math {

    /*
    NOTE: the following is my untested and uncompiled attempt to represent hypergeometric functions in C++
    */

    template<typename T, typename I>
    struct Hypergeometric {
        T initial_coefficient;
        ArbitraryDegreeRational<T,I> coefficient_ratio;
        constexpr explicit Hypergeometric(const T initial_coefficient, const ArbitraryDegreeRational<T,I>& coefficient_ratio):
            initial_coefficient(initial_coefficient),
            coefficient_ratio(coefficient_ratio)
        {}
        // zero constructor
        constexpr explicit Hypergeometric():
            initial_coefficient(0.0f),
            coefficient_ratio()
        {}
        // copy constructor
        constexpr Hypergeometric(const Hypergeometric<T,I>& f):
            initial_coefficient(f.initial_coefficient),
            coefficient_ratio(f.coefficient_ratio)
        {}
        // constant constructor
        constexpr explicit Hypergeometric(const T k):
            initial_coefficient(k),
            coefficient_ratio()
        {}
        template<int Nhi>
        constexpr polynomial() const
        {
            Polynomial<T,0,Nhi> p;
            T coefficient = initial_coefficient;
            for (int n = 0; n <= Nhi; ++n)
            {
                p[n] = coefficient;
                coefficient *= coefficient_ratio(n);
            }
            return p;
        }
        constexpr polynomial(const int Nhi) const
        {
            ArbitraryDegreePolynomial<T> p;
            T coefficient = initial_coefficient;
            for (int n = 0; n <= Nhi; ++n)
            {
                p[n] = coefficient;
                coefficient *= coefficient_ratio(n);
            }
            return p;
        }
        constexpr T operator()(const T x, const int Nhi) const
        {
            T y = 0.0f;
            T coefficient = initial_coefficient;
            for (int n = 0; n < count; ++n)
            {
                y += std::pow(x,n) * coefficient;
                coefficient *= coefficient_ratio(n);
            }
            return y;
        }
        constexpr Hypergeometric<T,I>& operator*=(const T k)
        {
            initial_coefficient *= k;
            return *this;
        }
        constexpr Hypergeometric<T,I>& operator/=(const T k)
        {
            initial_coefficient /= k;
            return *this;
        }
    };

    // operations that are closed under Rationals
    template<typename T, typename I>
    constexpr auto operator+(const Hypergeometric<T,I>& f, const Hypergeometric<T,I>& g)
    {
        return Hypergeometric<T,I>(
            f.initial_coefficient + g.initial_coefficient
            (f.coefficient_ratio.p * g.coefficient_ratio.p) / 
            (f.coefficient_ratio.q * g.coefficient_ratio.q)
        );
    }

    template<typename T>
    constexpr Hypergeometric<T,I> integral(const Hypergeometric<T,I>& f)
    {
        return Hypergeometric<T,I>(
            // T(0), f.initial_coefficient, // ?
            (f.coefficient_ratio.p * Shifting<T>(1)) / 
            (f.coefficient_ratio.q * Shifting<T>(2))
        );
    }

    template<typename T>
    constexpr Hypergeometric<T,I> derivative(const Hypergeometric<T,I>& f)
    {
        return Hypergeometric<T,I>(
            // T(0), T(1), // ?
            (f.coefficient_ratio.p * Shifting<T>(1)) / 
            (f.coefficient_ratio.q * Shifting<T>(0))
        );
    }


}