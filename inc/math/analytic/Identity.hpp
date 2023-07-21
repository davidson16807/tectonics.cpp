#pragma once

namespace math {

    /* 
    `Identity` is a trivial class that represents the function f(x)=x,
    oddly enough, it was useful for something, so we implemented it.
    (it was useful as the base case for the recursive implementation of `compose()` for polynomials)
    */
    template<typename T>
    struct Identity {
        constexpr explicit Identity()
        {}
        constexpr T operator()(const T x) const
        {
            return x;
        }
    };
    
    // operators that cause cancelation
    template<typename T>
    constexpr T operator-(const Identity<T> e1, const Identity<T> e2)
    {
        return T(0.0);
    }
    template<typename T>
    constexpr T operator/(const Identity<T> e1, const Identity<T> e2)
    {
        return T(0.0);
    }

}