#pragma once

#include "Scaling.hpp"
#include "Shifting.hpp"

namespace analytic {


    template<typename T>
    struct Poisson {
        using value_type = T;

        static constexpr T pi = 3.141592653589793238462643383279;

        T mean;
        T standard_deviation;
        T amplitude;
        constexpr explicit Poisson(const T mean, const T standard_deviation, const T amplitude):
            mean(mean),
            standard_deviation(standard_deviation),
            amplitude(amplitude)
        {}
        constexpr explicit Poisson(const T mean, const T standard_deviation):
            mean(mean),
            standard_deviation(standard_deviation),
            amplitude(T(1)/(standard_deviation*std::sqrt(pi)))
        {}
        // zero constructor
        constexpr explicit Poisson():
            mean(0.0f),
            standard_deviation(1.0f),
            amplitude(0.0f)
        {}
        constexpr Poisson(const Poisson<T>& f):
            mean(f.mean),
            standard_deviation(f.standard_deviation),
            amplitude(f.amplitude)
        {}
        constexpr T operator()(const T x) const
        {
            T u = (x-mean)/standard_deviation;
            return amplitude * std::exp(-u*u/T(2));
        }
        constexpr Poisson<T>& operator*=(const T k)
        {
            amplitude *= k;
            return *this;
        }
        constexpr Poisson<T>& operator/=(const T k)
        {
            amplitude /= k;
            return *this;
        }
    };

    template<typename T>
    constexpr Poisson<T> operator*(const Poisson<T>& f, const T k)
    {
        Poisson<T> y(f);
        y *= k;
        return y;
    }
    template<typename T>
    constexpr Poisson<T> operator*(const T k, const Poisson<T>& f)
    {
        Poisson<T> y(f);
        y *= k;
        return y;
    }
    template<typename T>
    constexpr Poisson<T> operator/(const Poisson<T>& f, const T k)
    {
        Poisson<T> y(f);
        y /= k;
        return y;
    }

    template<typename T>
    constexpr Poisson<T> compose(const Poisson<T>& f, const analytic::Scaling<T> g)
    {
        return Poisson<T>(
            f.mean * g.factor,
            f.standard_deviation/g.factor,
            f.amplitude
        );
    }

    template<typename T>
    constexpr Poisson<T> compose(const Poisson<T>& f, const analytic::Shifting<T> g)
    {
        return Poisson<T>(
            f.mean + g.offset,
            f.standard_deviation,
            f.amplitude
        );
    }

    template<typename T>
    constexpr T maximum(const Poisson<T>& f, const T lo, const T hi)
    {
        // function is monotonic, so solution must be either lo or hi
        return
            lo < f.mean && f.mean < hi? f.mean
          : f(hi) > f(lo)? hi : lo;
    }

    template<typename T>
    constexpr T minimum(const Poisson<T>& f, const T lo, const T hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) < f(lo)? hi : lo;
    }

}

