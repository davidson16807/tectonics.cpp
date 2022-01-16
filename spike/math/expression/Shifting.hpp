#pragma once

#include "Identity.hpp"

namespace math {

    /* 
    `Shifting` is a trivial class that represents the function f(x)=x+b,
    it was needed to provide an idiomatic way to generate taylor series using compose(),
    however it has since found other uses.
    */
    struct Shifting {
        float offset;
        constexpr explicit Shifting(const float offset):
            offset(offset)
        {}
        constexpr float operator()(const float x) const
        {
            return x+offset;
        }

        Shifting& operator+=(const float k)
        {
            offset += k;
            return *this;
        }

        Shifting& operator-=(const float k)
        {
            offset -= k;
            return *this;
        }
    };

    // operators with reals that are closed under Shifting relations
    constexpr Shifting operator+(const Shifting f, const float k)
    {
        return Shifting(f.offset + k);
    }
    constexpr Shifting operator+(const float k, const Shifting f)
    {
        return Shifting(f.offset + k);
    }
    constexpr Shifting operator-(const Shifting f, const float k)
    {
        return Shifting(f.offset - k);
    }

    // operators that produce Scaling relations when given other relations as input
    constexpr Shifting operator+(const Identity f, const float k)
    {
        return Shifting(k);
    }
    constexpr Shifting operator+(const float k, const Identity f)
    {
        return Shifting(k);
    }
    constexpr Shifting operator-(const Identity f, const float k)
    {
        return Shifting(-k);
    }

    // operators that cause cancelation
    constexpr float operator-(const Shifting f, const Shifting g)
    {
        return f.offset-g.offset;
    }
    
}