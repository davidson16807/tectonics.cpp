#pragma once

namespace math {

    /* 
    `Boxcar<T>` is a trivial class that represents the well-known "boxcar" function, 
    which we define here as boxcar(x)=1 for lo<x≤hi and boxcar(x)=0 otherwise.
    The choice of range ("lo<x≤hi") is mostly arbitrary and exists 
    only because a choice was needed that would allow the creation of continuous piecewise functions.
    */
    template<typename T>
    struct Boxcar {
        using value_type = T;

        T lo;
        T hi;
        constexpr explicit Boxcar<T>(const T lo, const T hi):
            lo(lo),
            hi(hi)
        {}
        constexpr T operator()(const T x) const
        {
            return T(lo<x && x<=hi);
        }
    };

}
