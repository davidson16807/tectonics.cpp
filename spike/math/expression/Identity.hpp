#pragma once

namespace math {

    /* 
    `Identity` is a trivial class that represents the function f(x)=x,
    oddly enough, it was useful for something, so we implemented it.
    (it was useful as the base case for the recursive implementation of `compose()` for polynomials)
    */
    struct Identity {
        constexpr explicit Identity()
        {}
        constexpr float operator()(const float x) const
        {
            return x;
        }
    };
    
    // operators that cause cancelation
    constexpr float operator-(const Identity e1, const Identity e2)
    {
        return 0.0f;
    }
    constexpr float operator/(const Identity e1, const Identity e2)
    {
        return 0.0f;
    }

}