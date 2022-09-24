#pragma once

namespace math {

    template<typename T>
    struct NaturalExponent {
        T weight;
        T exponent;
        constexpr explicit NaturalExponent(const T weight):
            weight(weight)
        {}
        // zero constructor
        constexpr explicit NaturalExponent():
            weight(0.0f)
        {}
        // constant constructor
        constexpr explicit NaturalExponent(const T k):
            weight(k)
        {}
        constexpr NaturalExponent(const NaturalExponent<T>& f):
            weight(f.weight)
        {}
        constexpr T operator()(const T x) const
        {
            return weight*std::exp(x);
        }
        constexpr NaturalExponent<T>& operator*=(const T k)
        {
            weight *= k;
            return *this;
        }
        constexpr NaturalExponent<T>& operator/=(const T k)
        {
            weight /= k;
            return *this;
        }
    };

    template<typename T>
    constexpr NaturalExponent<T> operator*(const NaturalExponent<T>& f, const T k)
    {
        return NaturalExponent<T>(f.weight*k);
    }
    template<typename T>
    constexpr NaturalExponent<T> operator*(const T k, const NaturalExponent<T>& f)
    {
        return NaturalExponent<T>(f.weight*k);
    }
    template<typename T>
    constexpr NaturalExponent<T> operator/(const NaturalExponent<T>& f, const T k)
    {
        return NaturalExponent<T>(f.weight/k);
    }
    template<typename T>
    constexpr NaturalExponent<T> operator/(const T k, const NaturalExponent<T>& f)
    {
        return NaturalExponent<T>(k/f.weight);
    }

    template<typename T>
    constexpr NaturalExponent<T> compose(const NaturalExponent<T>& f, const math::Scaling<T> g)
    {
        return NaturalExponent<T>(f.weight*std::exp(g.factor));
    }

    template<typename T>
    constexpr T maximum(const NaturalExponent<T>& f, const T lo, const T hi)
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) > f(lo)? hi : lo;
    }

    template<typename T>
    constexpr T minimum(const NaturalExponent<T>& f, const T lo, const T hi) 
    {
        // function is monotonic, so solution must be either lo or hi
        return f(hi) < f(lo)? hi : lo;
    }
    
    /*
    `max_distance` measures the largest absolute difference between the output of two functions over the given range.
    This is useful when determining whether two functions should be consolidated when used in a larger equation.
    */
    template<typename T>
    constexpr T max_distance(const NaturalExponent<T>& relation1, const NaturalExponent<T>& relation2, const T lo, const T hi)
    {
        return std::max(
            std::abs(relation1(hi)-relation2(hi)),
            std::abs(relation1(lo)-relation2(lo))
        );
    }

    // template<typename T>
    // constexpr NaturalExponent<T> exp(const Identity<T>& e)
    // {
    //     return NaturalExponent<T>(T(1), exponent);
    // }

}