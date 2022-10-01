#pragma once

namespace analytic {

    template<typename T>
    struct NaturalLogarithm {
        T weight;
        T scale;
        constexpr explicit NaturalLogarithm(const T weight, const T scale):
            weight(weight),
            scale(scale)
        {}
        // zero constructor
        constexpr explicit NaturalLogarithm():
            weight(0),
            scale(1)
        {}
        constexpr NaturalLogarithm(const NaturalLogarithm<T>& f):
            weight(f.weight),
            scale(f.scale)
        {}
        constexpr T operator()(const T x) const
        {
            return weight*std::log(x/scale);
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
        NaturalLogarithm<T> y(f);
        y *= k;
        return y;
    }
    template <typename T>
    constexpr NaturalLogarithm<T> operator*(const T k, const NaturalLogarithm<T> f)
    {
        NaturalLogarithm<T> y(f);
        y *= k;
        return y;
    }
    template <typename T>
    constexpr NaturalLogarithm<T> operator/(const NaturalLogarithm<T> f, const T k)
    {
        NaturalLogarithm<T> y(f);
        y *= k;
        return y;
    }


    template<typename T>
    NaturalLogarithm<T> compose(const NaturalLogarithm<T> f, const Scaling<T> g){
        return NaturalLogarithm<T>(f.weight, T(1)/g.factor);
    }

    template<typename T>
    NaturalLogarithm<T> log(const Identity<T> e){
        return NaturalLogarithm<T>(T(1),T(1));
    }

}