#pragma once

namespace math {

    template<typename T>
    struct NaturalLogarithm {
        T weight;
        constexpr explicit NaturalLogarithm(const T weight):
            weight(weight)
        {}
        // zero constructor
        constexpr explicit NaturalLogarithm():
            weight(0.0f)
        {}
        constexpr NaturalLogarithm(const NaturalLogarithm<T>& f):
            weight(f.weight)
        {}
        constexpr T operator()(const T x) const
        {
            return weight*std::log(x);
        }
        constexpr NaturalLogarithm<T>& operator*=(const T k)
        {
            weight *= k;
            return *this;
        }
        constexpr NaturalLogarithm<T>& operator/=(const T k)
        {
            weight /= k;
            return *this;
        }
    };

    template <typename T>
    constexpr NaturalLogarithm<T> operator*(const NaturalLogarithm<T> f, const T k)
    {
        return NaturalLogarithm<T>(f.weight*k);
    }
    template <typename T>
    constexpr NaturalLogarithm<T> operator*(const T k, const NaturalLogarithm<T> f)
    {
        return NaturalLogarithm<T>(f.weight*k);
    }
    template <typename T>
    constexpr NaturalLogarithm<T> operator/(const NaturalLogarithm<T> f, const T k)
    {
        return NaturalLogarithm<T>(f.weight/k);
    }


    template<typename T>
    NaturalLogarithm<T> ln(const Identity<T> e){
        return NaturalLogarithm<T>(T(1));
    }

}