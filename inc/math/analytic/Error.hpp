#pragma once

// C libraries
#include <cmath>

#include "Gaussian.hpp"

namespace analytic {

    template<typename T>
    struct Error {
        using value_type = T;

        static constexpr T pi = T(3.141592653589793238462643383279);

        T mean;
        T standard_deviation;
        T amplitude;
        constexpr explicit Error(const T mean, const T standard_deviation, const T amplitude):
            mean(mean),
            standard_deviation(standard_deviation),
            amplitude(amplitude)
        {}
        constexpr explicit Error(const T mean, const T standard_deviation):
            mean(mean),
            standard_deviation(standard_deviation),
            amplitude(T(1)/(standard_deviation*std::sqrt(T(2)*pi)))
        {}
        // zero constructor
        constexpr explicit Error():
            mean(0),
            standard_deviation(1),
            amplitude(0)
        {}
        constexpr Error(const Error<T>& f):
            mean(f.mean),
            standard_deviation(f.standard_deviation),
            amplitude(f.amplitude)
        {}
        constexpr T operator()(const T x) const
        {
            const T a(0.1);
            const T b(1.1295);
            const T u((x-mean)/(standard_deviation*std::sqrt(T(2))));
            // Approximation for erf() is accurate to within 2⋅10⁻⁴ over the range [-10,10].
            return (amplitude*(standard_deviation*std::sqrt(T(2)*pi))) * T(0.5) * (T(1) + std::tanh(a*u*u*u + b*u));
        }
        constexpr Error<T>& operator*=(const T k)
        {
            amplitude *= k;
            return *this;
        }
        constexpr Error<T>& operator/=(const T k)
        {
            amplitude /= k;
            return *this;
        }
    };

    template<typename T>
    constexpr Error<T> operator*(const Error<T>& f, const T k)
    {
        Error<T> y(f);
        y *= k;
        return y;
    }
    template<typename T>
    constexpr Error<T> operator*(const T k, const Error<T>& f)
    {
        Error<T> y(f);
        y *= k;
        return y;
    }
    template<typename T>
    constexpr Error<T> operator/(const Error<T>& f, const T k)
    {
        Error<T> y(f);
        y /= k;
        return y;
    }

    template<typename T>
    constexpr Error<T> compose(const Error<T>& f, const analytic::Scaling<T> g)
    {
        return Error<T>(
            f.mean * g.factor,
            f.standard_deviation/g.factor,
            f.amplitude
        );
    }

    template<typename T>
    constexpr Error<T> compose(const Error<T>& f, const analytic::Shifting<T> g)
    {
        return Error<T>(
            f.mean + g.offset,
            f.standard_deviation,
            f.amplitude
        );
    }


    template<typename T>
    constexpr T maximum(const Error<T>& f, const T lo, const T hi)
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) > f(lo)? hi : lo;
    }

    template<typename T>
    constexpr T minimum(const Error<T>& f, const T lo, const T hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) < f(lo)? hi : lo;
    }

    template<typename T>
    auto integral(const Gaussian<T>& f)
    {
        return Error<T>(f.mean, f.standard_deviation, f.amplitude);
    }

    template<typename T>
    auto integral(const Sum<T,Gaussian<T>>& sum)
    {
        Sum<T,Error<T>> I;
        for (std::size_t i=0; i<sum.terms.size(); i++)
        {
            I.terms.push_back(integral(sum.terms[i])); 
        }
        return I;
    }
    
}

