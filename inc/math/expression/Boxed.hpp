#pragma once

namespace math {

    /* 
    `Boxed<T,F>` is a trivial class that represents the function F(x) if lo<x≤hi and 0 otherwise.*
    Its name derives from multiplying F(x) by the "box" function, where box(x)=1 for lo<x≤hi and box(x)=0 otherwise. 
    The intent in creating `Boxed` functions is to represent piecewise functions as the sums of `Boxed` functions,
    since sums of boxed functions can be easily closed under arithmetic operations, derivatives, and integrals.
    This is in contrast to a piecewise representation, where implementing closed operations is harder to implement
    and requires constantly subdividing pieces and increasing piece count, potentially reducing performance.

    * The choice of range ("lo<x≤hi") is mostly arbitrary and exists 
    only because a choice was needed that would allow the creation of continuous piecewise functions.
    */
    template<typename T, typename F>
    struct Boxed {
        F f;
        T lo;
        T hi;
        constexpr explicit Boxed<T,F>(const T lo, const T hi, const F f):
            f(f),
            lo(lo),
            hi(hi)
        {}
        constexpr Boxed<T,F>(const Boxed<T,F>& piece):
            f(piece.f),
            lo(piece.lo),
            hi(piece.hi)
        {}
        constexpr Boxed<T,F>():
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
