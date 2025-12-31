#pragma once

// C libraries
#include <cmath> // pow

// C libraries
#include <algorithm> // max

#include "Identity.hpp"
#include "Scaling.hpp"

namespace analytic {

    template<typename T>
    struct Exponent {
        using value_type = T;

        T weight;
        T exponent;
        constexpr explicit Exponent(const T weight, const T exponent):
            weight(weight),
            exponent(exponent)
        {}
        // zero constructor
        constexpr explicit Exponent():
            weight(0.0f),
            exponent(1.0f)
        {}
        // constant constructor
        constexpr explicit Exponent(const T k):
            weight(k),
            exponent(0.0f)
        {}
        constexpr Exponent(const Exponent<T>& f):
            weight(f.weight),
            exponent(f.exponent)
        {}
        constexpr T operator()(const T x) const
        {
            return weight*std::pow(x, exponent);
        }
        constexpr Exponent<T>& operator*=(const T k)
        {
            weight *= k;
            return *this;
        }
        constexpr Exponent<T>& operator/=(const T k)
        {
            weight /= k;
            return *this;
        }
    };

    template<typename T>
    constexpr Exponent<T> operator*(const Exponent<T>& f, const T k)
    {
        return Exponent<T>(f.weight*k, f.exponent);
    }
    template<typename T>
    constexpr Exponent<T> operator*(const T k, const Exponent<T>& f)
    {
        return Exponent<T>(f.weight*k, f.exponent);
    }
    template<typename T>
    constexpr Exponent<T> operator/(const Exponent<T>& f, const T k)
    {
        return Exponent<T>(f.weight/k, f.exponent);
    }
    template<typename T>
    constexpr Exponent<T> operator/(const T k, const Exponent<T>& f)
    {
        return Exponent<T>(k/f.weight, -f.exponent);
    }

    template<typename T>
    constexpr Exponent<T> compose(const Exponent<T>& f, const analytic::Scaling<T> g)
    {
        return Exponent<T>(
            f.weight*std::pow(g.factor, f.exponent),
            f.exponent
        );
    }

    template<typename T>
    constexpr T maximum(const Exponent<T>& f, const T lo, const T hi)
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) > f(lo)? hi : lo;
    }

    template<typename T>
    constexpr T minimum(const Exponent<T>& f, const T lo, const T hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) < f(lo)? hi : lo;
    }
    
    /*
    `max_distance` measures the largest absolute difference between the output of two functions over the given range.
    This is useful when determining whether two functions should be consolidated when used in a larger equation.
    */
    template<typename T>
    constexpr T max_distance(const Exponent<T>& relation1, const Exponent<T>& relation2, const T lo, const T hi)
    {
        return std::max(
            std::abs(relation1(hi)-relation2(hi)),
            std::abs(relation1(lo)-relation2(lo))
        );
    }

    template<typename T>
    constexpr Exponent<T> pow(const Identity<T>& e, const T exponent)
    {
        return Exponent<T>(T(1), exponent);
    }

}