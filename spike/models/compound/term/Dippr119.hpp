#pragma once

namespace compound {
namespace term {

    struct Dippr119 {
        float c1;
        float c2;
        float c3;
        // zero constructor
        constexpr explicit Dippr119():
            c1(0.0f),
            c2(1.0f),
            c3(0.0f)
        {}
        // copy constructor
        constexpr Dippr119(const Dippr119& f):
            c1(f.c1),
            c2(f.c2),
            c3(f.c3)
        {}
        constexpr explicit Dippr119(
            const float c1,
            const float c2,
            const float c3
        ):
            c1(c1),
            c2(c2),
            c3(c3)
        {}
        constexpr float operator()(const float x) const
        {
            return c1*pow(1.0-(x/c2), c3);
        }
        constexpr Dippr119& operator*=(const float scalar)
        {
            c1 *= scalar;
            return *this;
        }
        constexpr Dippr119& operator/=(const float scalar)
        {
            c1 /= scalar;
            return *this;
        }
    };
    constexpr Dippr119 operator*(const Dippr119 relation, const float scalar)
    {
        Dippr119 result(relation);
        result *= scalar;
        return result;
    }
    constexpr Dippr119 operator*(const float scalar, const Dippr119 relation)
    {
        Dippr119 result(relation);
        result *= scalar;
        return result;
    }
    constexpr Dippr119 operator/(const Dippr119 relation, const float scalar)
    {
        Dippr119 result(relation);
        result /= scalar;
        return result;
    }
    constexpr Dippr119 operator-(const Dippr119 relation)
    {
        Dippr119 result(relation);
        result *= -1.0f;
        return result;
    }


    constexpr Dippr119 compose(Dippr119 f, const math::Scaling<float> g)
    {
        return Dippr119(
            f.c1,
            f.c2/g.factor,
            f.c3
        );
    }


}}
