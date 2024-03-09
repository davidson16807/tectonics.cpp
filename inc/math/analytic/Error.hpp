#pragma once

namespace analytic {

    template<typename T>
    struct Error {
        using value_type = T;

        constexpr T pi = 3.141592653589793238462643383279;

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
            amplitude(T(1)/(standard_deviation*std::sqrt(pi)))
        {}
        // zero constructor
        constexpr explicit Error():
            mean(0.0f),
            standard_deviation(1.0f),
            amplitude(0.0f)
        {}
        constexpr Error(const Error<T>& f):
            mean(f.mean),
            standard_deviation(f.standard_deviation)
        {}
        constexpr T operator()(const T x) const
        {
            const T a(0.1);
            const T b(1.1295);
            return std::tanh(a*x*x*x + b*x);
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
        return
            lo < f.mean && f.mean < hi? f.mean
          : f(hi) > f(lo)? hi : lo;
    }

    template<typename T>
    constexpr T minimum(const Error<T>& f, const T lo, const T hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) < f(lo)? hi : lo;
    }
    
}