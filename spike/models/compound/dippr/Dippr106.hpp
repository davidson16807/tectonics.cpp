#pragma once

namespace compound {
namespace dippr {

    struct Dippr106 {
        float c1;
        float c2;
        float c3;
        float c4;
        float Tc;
        constexpr explicit Dippr106(
            const float c1,
            const float c2,
            const float c3,
            const float c4,
            const float Tc
        ):
            c1(c1),
            c2(c2),
            c3(c3),
            c4(c4),
            Tc(Tc)
        {}
        // zero constructor
        constexpr explicit Dippr106():
            c1(0.0f),
            c2(0.0f),
            c3(0.0f),
            c4(0.0f),
            Tc(1.0f)
        {}
        // constant constructor
        constexpr explicit Dippr106(const float k):
            c1(k),
            c2(0.0f),
            c3(0.0f),
            c4(0.0f),
            Tc(1.0f)
        {}
        // copy constructor
        constexpr Dippr106(const Dippr106& f):
            c1(f.c1),
            c2(f.c2),
            c3(f.c3),
            c4(f.c4),
            Tc(f.Tc)
        {}
        constexpr float operator()(const float x) const
        {
            return c1 * std::pow(1.0f-x/Tc, c2 + c3*x + c4*x*x);
        }
        constexpr Dippr106& operator*=(const float scalar)
        {
            c1 *= scalar;
            return *this;
        }
        constexpr Dippr106& operator/=(const float scalar)
        {
            c1 /= scalar;
            return *this;
        }
    };
    constexpr Dippr106 operator*(const Dippr106 relation, const float scalar)
    {
        return Dippr106(relation.c1 * scalar, relation.c2, relation.c3, relation.c4, relation.Tc);
    }
    constexpr Dippr106 operator*(const float scalar, const Dippr106 relation)
    {
        return Dippr106(relation.c1 * scalar, relation.c2, relation.c3, relation.c4, relation.Tc);
    }
    constexpr Dippr106 operator/(const Dippr106 relation, const float scalar)
    {
        return Dippr106(relation.c1 / scalar, relation.c2, relation.c3, relation.c4, relation.Tc);
    }
    constexpr Dippr106 operator-(const Dippr106 relation)
    {
        return Dippr106(relation.c1 * -1.0f, relation.c2, relation.c3, relation.c4, relation.Tc);
    }

    constexpr Dippr106 compose(Dippr106 f, const analytic::Scaling<float> g)
    {
        return Dippr106(
            f.c1,
            f.c2,
            f.c3,
            f.c4,
            f.Tc / g.factor
        );
    }


}}