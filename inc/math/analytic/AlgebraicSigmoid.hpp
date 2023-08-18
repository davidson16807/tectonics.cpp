#pragma once

namespace math {

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
            xscale(0.0f),
            xoffset(0.0f),
            ymax(0.0f)
        {}
        constexpr T operator()(const T x) const
        {
            T u = xscale * x + xoffset;
            return ymax * u / std::sqrt(1.0f + u*u);
        }
        constexpr AlgebraicSigmoid<T>& operator*=(const T scalar)
        {
            ymax *= scalar;
            return *this;
        }
        constexpr AlgebraicSigmoid<T>& operator/=(const T scalar)
        {
            ymax /= scalar;
            return *this;
        }
    };

    template<typename T>
    constexpr AlgebraicSigmoid<T> operator*(const AlgebraicSigmoid<T>& relation, const T scalar)
    {
        return AlgebraicSigmoid(relation.xscale, relation.xoffset, relation.ymax*scalar);
    }

    template<typename T>
    constexpr AlgebraicSigmoid<T> operator*(const T scalar, const AlgebraicSigmoid<T>& relation)
    {
        return AlgebraicSigmoid(relation.xscale, relation.xoffset, relation.ymax*scalar);
    }

    template<typename T>
    constexpr AlgebraicSigmoid<T> operator/(const AlgebraicSigmoid<T>& relation, const T scalar)
    {
        return AlgebraicSigmoid(relation.xscale, relation.xoffset, relation.ymax/scalar);
    }


    template<typename T>
    constexpr AlgebraicSigmoid<T> compose(const AlgebraicSigmoid<T>& f, const math::Scaling<T> g)
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
