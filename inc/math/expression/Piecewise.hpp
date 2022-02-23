#pragma once

namespace math {

    /* 
    `Piecewise<T,F>` is a trivial class that represents the function F(x) if lo<x≤hi and 0 otherwise, 
    where F is a template function object that returns type T.
    The choice of range ("lo<x≤hi") is mostly arbitrary and exists 
    only because a choice was needed that would allow the creation of continuous piecewise functions.
    */
    template<typename T, typename F>
    struct Piecewise {
        F f;
        T lo;
        T hi;
        constexpr explicit Piecewise<T,F>(const T lo, const T hi, const F f):
            f(f),
            lo(lo),
            hi(hi)
        {}
        constexpr Piecewise<T,F>(const Piecewise<T,F>& piece):
            f(piece.f),
            lo(piece.lo),
            hi(piece.hi)
        {}
        constexpr Piecewise<T,F>():
            f(),
            lo(),
            hi()
        {}
        constexpr T operator()(const T x) const
        {
            return lo<x && x<=hi? f(x) : T(0.0);
        }
    };

}
