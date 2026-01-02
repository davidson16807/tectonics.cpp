#pragma once

// C libraries
#include <cmath>

// in-house libraries
#include <math/analytic/Scaling.hpp>

namespace compound {
namespace dippr {

    struct Dippr102 {
        float c1;
        float c2;
        float c3;
        float c4;
        constexpr explicit Dippr102(
            const float c1,
            const float c2,
            const float c3,
            const float c4
        ):
            c1(c1),
            c2(c2),
            c3(c3),
            c4(c4)
        {}
        constexpr explicit Dippr102():
            c1(0.0f),
            c2(1.0f),
            c3(0.0f),
            c4(0.0f)
        {}
        constexpr Dippr102(const Dippr102& f):
            c1(f.c1),
            c2(f.c2),
            c3(f.c3),
            c4(f.c4)
        {}
        float operator()(const float x) const
        {
            return c1 * std::pow(x, c2) / (1.0f + c3/x + c4/(x*x));
        }
        constexpr Dippr102& operator*=(const float scalar)
        {
            c1 *= scalar;
            return *this;
        }
        constexpr Dippr102& operator/=(const float scalar)
        {
            c1 /= scalar;
            return *this;
        }
    };
    constexpr Dippr102 operator*(const Dippr102 relation, const float scalar)
    {
        return Dippr102(relation.c1 * scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr102 operator*(const float scalar, const Dippr102 relation)
    {
        return Dippr102(relation.c1 * scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr102 operator/(const Dippr102 relation, const float scalar)
    {
        return Dippr102(relation.c1 / scalar, relation.c2, relation.c3, relation.c4);
    }
    constexpr Dippr102 operator-(const Dippr102 relation)
    {
        return Dippr102(relation.c1 * -1.0f, relation.c2, relation.c3, relation.c4);
    }


    Dippr102 compose(Dippr102 f, const analytic::Scaling<float> g)
    {
        return Dippr102(
            f.c1 * std::pow(g.factor, f.c2),
            f.c2,
            f.c3 / g.factor,
            f.c4 / (g.factor*g.factor)
        );
    }


}}