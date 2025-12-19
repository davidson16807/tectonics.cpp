#pragma once

// C headers
#include <cmath>     // std::sqrt, std::abs

// standard headers
#include <algorithm> // std::max

namespace analytic {

    template <typename T>
    struct AlgebraicSigmoid{
        using value_type = T;

        T xscale;
        T xoffset;
        T ymax;
        constexpr explicit AlgebraicSigmoid(const T xscale, const T xoffset, const T ymax):
            xscale(xscale),
            xoffset(xoffset),
            ymax(ymax)
        {}
        // zero constructor
        constexpr explicit AlgebraicSigmoid():
            xscale(0),
            xoffset(0),
            ymax(0)
        {}
        T operator()(const T x) const
        {
            T u = xscale * x + xoffset;
            return ymax * u / std::sqrt(T(1) + u*u);
        }
        constexpr AlgebraicSigmoid<T>& operator*=(const T k)
        {
            ymax *= k;
            return *this;
        }
        constexpr AlgebraicSigmoid<T>& operator/=(const T k)
        {
            ymax /= k;
            return *this;
        }
    };

    template<typename T>
    constexpr AlgebraicSigmoid<T> operator*(const AlgebraicSigmoid<T>& relation, const T k)
    {
        return AlgebraicSigmoid(relation.xscale, relation.xoffset, relation.ymax*k);
    }

    template<typename T>
    constexpr AlgebraicSigmoid<T> operator*(const T k, const AlgebraicSigmoid<T>& relation)
    {
        return AlgebraicSigmoid(relation.xscale, relation.xoffset, relation.ymax*k);
    }

    template<typename T>
    constexpr AlgebraicSigmoid<T> operator/(const AlgebraicSigmoid<T>& relation, const T k)
    {
        return AlgebraicSigmoid(relation.xscale, relation.xoffset, relation.ymax/k);
    }


    template<typename T>
    constexpr AlgebraicSigmoid<T> compose(const AlgebraicSigmoid<T>& f, const analytic::Scaling<T> g)
    {
        return AlgebraicSigmoid(
            f.xscale * g.factor,
            f.xoffset,
            f.ymax
        );
    }

    template<typename T>
    constexpr T maximum(const AlgebraicSigmoid<T>& f, const T lo, const T hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) > f(lo)? hi : lo;
    }

    template<typename T>
    constexpr T minimum(const AlgebraicSigmoid<T>& f, const T lo, const T hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) < f(lo)? hi : lo;
    }


    template<typename T>
    constexpr T max_distance(const AlgebraicSigmoid<T>& relation1, const AlgebraicSigmoid<T>& relation2, const T lo, const T hi)
    {
        return std::max(
            std::abs(relation1(hi)-relation2(hi)),
            std::abs(relation1(lo)-relation2(lo))
        );
    }

}
