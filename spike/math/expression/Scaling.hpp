#pragma once

#include "Identity.hpp"
#include "Scaling.hpp"

namespace math {

    /* 
    `Scaling` is a trivial class that represents the function f(x)=ax,
    it was needed to provide an idiomatic way to request that input to other functions be scaled using compose(),
    however it has since found other uses.
    */
    struct Scaling {
        float factor;
        constexpr explicit Scaling(const float factor):
            factor(factor)
        {}
        constexpr explicit Scaling(const Identity e):
            factor(1.0f)
        {}
        constexpr float operator()(const float x) const
        {
            return factor*x;
        }

        constexpr Scaling& operator*=(const float k)
        {
            factor *= k;
            return *this;
        }

        constexpr Scaling& operator/=(const float k)
        {
            factor /= k;
            return *this;
        }

        constexpr Scaling& operator+=(const Scaling f)
        {
            factor += f.factor;
            return *this;
        }

        constexpr Scaling& operator-=(const Scaling f)
        {
            factor -= f.factor;
            return *this;
        }

    };

    // operators that are closed under Scaling relations
    constexpr Scaling operator+(const Scaling f, const Scaling g)
    {
        Scaling y = f;
        y += g;
        return y;
    }
    constexpr Scaling operator-(const Scaling f, const Scaling g)
    {
        Scaling y = f;
        y += g;
        return y;
    }
    constexpr Scaling operator-(const Scaling f)
    {
        return Scaling(-f.factor);
    }

    // operators with reals that are closed under Scaling relations
    constexpr Scaling operator*(const Scaling f, const float k)
    {
        return Scaling(f.factor * k);
    }
    constexpr Scaling operator*(const float k, const Scaling f)
    {
        return Scaling(k * f.factor);
    }
    constexpr Scaling operator/(const Scaling f, const float k)
    {
        return Scaling(f.factor / k);
    }

    // operators that produce Scaling relations when given other relations as input
    constexpr Scaling operator*(const Identity f, const float k)
    {
        return Scaling(k);
    }
    constexpr Scaling operator*(const float k, const Identity f)
    {
        return Scaling(k);
    }
    constexpr Scaling operator/(const Identity f, const float k)
    {
        return Scaling(1.0f / k);
    }
    constexpr Scaling operator-(const Identity e)
    {
        return Scaling(-1.0f);
    }

    constexpr auto operator+(const Identity e, const Scaling f)
    {
        return Scaling(e) + Scaling(f);
    }
    constexpr auto operator+(const Scaling f, const Identity e)
    {
        return Scaling(f) + Scaling(e);
    }
    constexpr auto operator-(const Identity e, const Scaling f)
    {
        return Scaling(e) - Scaling(f);
    }
    constexpr auto operator-(const Scaling f, const Identity e)
    {
        return Scaling(f) - Scaling(e);
    }

    // operators that cause cancelation
    constexpr float operator/(const Scaling f, const Scaling g)
    {
        return f.factor/g.factor;
    }
    constexpr float operator/(const Identity e, const Scaling f)
    {
        return 1.0f/f.factor;
    }
    constexpr float operator/(const Scaling f, const Identity e)
    {
        return f.factor;
    }





}

