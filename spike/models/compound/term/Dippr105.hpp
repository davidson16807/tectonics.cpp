
namespace compound {
namespace term {

    struct Dippr105 {
        float c1;
        float c2;
        float c3;
        float c4;
        // zero constructor
        constexpr explicit Dippr105():
            c1(0.0f),
            c2(1.0f),
            c3(0.0f),
            c4(0.0f)
        {}
        // copy constructor
        constexpr Dippr105(const Dippr105& f):
            c1(f.c1),
            c2(f.c2),
            c3(f.c3),
            c4(f.c4)
        {}
        constexpr explicit Dippr105(
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
        constexpr float operator()(const float x) const
        {
            return c1 / std::pow(c2, 1+std::pow(1.0-(x/c3), c4));
        }
        constexpr Dippr105& operator*=(const float scalar)
        {
            c1 *= scalar;
            return *this;
        }
        constexpr Dippr105& operator/=(const float scalar)
        {
            c1 /= scalar;
            return *this;
        }
    };
    constexpr Dippr105 operator*(const Dippr105 relation, const float scalar)
    {
        Dippr105 result(relation);
        result *= scalar;
        return result;
    }
    constexpr Dippr105 operator*(const float scalar, const Dippr105 relation)
    {
        Dippr105 result(relation);
        result *= scalar;
        return result;
    }
    constexpr Dippr105 operator/(const Dippr105 relation, const float scalar)
    {
        Dippr105 result(relation);
        result /= scalar;
        return result;
    }
    constexpr Dippr105 operator-(const Dippr105 relation)
    {
        Dippr105 result(relation);
        result *= -1.0f;
        return result;
    }


    constexpr Dippr105 compose(Dippr105 f, const math::Scaling<float> g)
    {
        return Dippr105(
            f.c1,
            f.c2,
            f.c3/g.factor,
            f.c4
        );
    }


}}
