#pragma once

namespace compound {
namespace dippr {

    struct Dippr119Term {
        float c1;
        float c2;
        float c3;
        // zero constructor
        constexpr explicit Dippr119Term():
            c1(0.0f),
            c2(1.0f),
            c3(0.0f)
        {}
        // copy constructor
        constexpr Dippr119Term(const Dippr119Term& f):
            c1(f.c1),
            c2(f.c2),
            c3(f.c3)
        {}
        constexpr explicit Dippr119Term(
            const float c1,
            const float c2,
            const float c3
        ):
            c1(c1),
            c2(c2),
            c3(c3)
        {}
        float operator()(const float x) const
        {
            return c1*pow(1.0-(x/c2), c3);
        }
        constexpr Dippr119Term& operator*=(const float scalar)
        {
            c1 *= scalar;
            return *this;
        }
        constexpr Dippr119Term& operator/=(const float scalar)
        {
            c1 /= scalar;
            return *this;
        }
    };
    constexpr Dippr119Term operator*(const Dippr119Term relation, const float scalar)
    {
        Dippr119Term result(relation);
        result *= scalar;
        return result;
    }
    constexpr Dippr119Term operator*(const float scalar, const Dippr119Term relation)
    {
        Dippr119Term result(relation);
        result *= scalar;
        return result;
    }
    constexpr Dippr119Term operator/(const Dippr119Term relation, const float scalar)
    {
        Dippr119Term result(relation);
        result /= scalar;
        return result;
    }
    constexpr Dippr119Term operator-(const Dippr119Term relation)
    {
        Dippr119Term result(relation);
        result *= -1.0f;
        return result;
    }


    constexpr Dippr119Term compose(Dippr119Term f, const analytic::Scaling<float> g)
    {
        return Dippr119Term(
            f.c1,
            f.c2/g.factor,
            f.c3
        );
    }


}}
