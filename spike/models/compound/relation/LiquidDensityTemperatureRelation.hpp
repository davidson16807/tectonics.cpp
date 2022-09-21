#pragma once

#include <iostream>
#include <algorithm>

// in-house libraries
#include <math/expression/Scaling.hpp>
#include <math/expression/Polynomial.hpp>
#include <math/expression/Clamped.hpp>
#include <math/inspection.hpp>
#include <units/si.hpp>

namespace compound {
namespace relation {


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

    using ClampedDippr105 = math::Clamped<float,Dippr105>;
    using ClampedDippr119 = math::Clamped<float,Dippr119>;

    /*
    `LiquidDensityTemperatureRelation` consolidates many kinds of expressions
    that are commonly used to represent the relation between temperature and the density of liquids
    */
    class LiquidDensityTemperatureRelation
    {
        std::vector<ClampedDippr105> dippr105s;
        std::vector<ClampedDippr119> dippr119s;

        si::temperature<double> Tunits;
        si::density<double>     yunits;

    public:
        float intercept;
        float known_max_fractional_error;

        // zero constructor
        LiquidDensityTemperatureRelation():
            dippr105s(),
            dippr119s(),

            Tunits(1.0),
            yunits(1.0),

            intercept(0.0),
            known_max_fractional_error(0.0)
        {
        }

        // constant constructor
        LiquidDensityTemperatureRelation(const si::density<double> k):
            dippr105s(),
            dippr119s(),

            Tunits(1.0),
            yunits(1.0),

            intercept(k/si::density<double>(1.0)),
            known_max_fractional_error(0.0)
        {
        }

        LiquidDensityTemperatureRelation(
            const std::vector<ClampedDippr105> dippr105s,
            const std::vector<ClampedDippr119> dippr119s,

            const si::temperature<double> Tunits,
            const si::density<double> yunits,

            const float intercept,
            const float known_max_fractional_error
        ):
            dippr105s(dippr105s),
            dippr119s(dippr119s),

            Tunits(Tunits),
            yunits(yunits),

            intercept(intercept),
            known_max_fractional_error(known_max_fractional_error)
        {
        }

        LiquidDensityTemperatureRelation& operator=(const LiquidDensityTemperatureRelation other)
        {
            dippr105s = other.dippr105s;
            dippr119s = other.dippr119s;

            Tunits = other.Tunits;
            yunits = other.yunits;

            intercept = other.intercept;
            known_max_fractional_error = other.known_max_fractional_error;

            return *this;
        }

        si::density<double> operator()(const si::temperature<double> temperature) const
        {
            const float T = float(temperature/Tunits);
            ClampedDippr105 dippr105;
            ClampedDippr119 dippr119;
            float y = intercept;
            for (std::size_t i = 0; i < dippr105s.size(); ++i)
            {
                y += dippr105s[i](T);
            }
            for (std::size_t i = 0; i < dippr119s.size(); ++i)
            {
                y += dippr119s[i](T);
            }
            return y * yunits;
        }

        LiquidDensityTemperatureRelation& operator*=(const float scalar)
        {
            for (std::size_t i = 0; i < dippr105s.size(); ++i)
            {
                dippr105s[i] *= scalar;
            }
            for (std::size_t i = 0; i < dippr119s.size(); ++i)
            {
                dippr119s[i] *= scalar;
            }
            intercept *= scalar;
            return *this;
        }

        LiquidDensityTemperatureRelation operator/=(const float scalar)
        {
            for (std::size_t i = 0; i < dippr105s.size(); ++i)
            {
                dippr105s[i] /= scalar;
            }
            for (std::size_t i = 0; i < dippr119s.size(); ++i)
            {
                dippr119s[i] /= scalar;
            }
            intercept /= scalar;
            return *this;
        }

        LiquidDensityTemperatureRelation& operator+=(const LiquidDensityTemperatureRelation other)
        {
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            for (std::size_t i = 0; i < other.dippr105s.size(); ++i)
            {
                dippr105s.push_back(yscale * compose(other.dippr105s[i], math::Scaling<float>(Tscale)));
            }
            for (std::size_t i = 0; i < other.dippr119s.size(); ++i)
            {
                dippr119s.push_back(yscale * compose(other.dippr119s[i], math::Scaling<float>(Tscale)));
            }
            intercept += yscale * other.intercept;
            return *this;
        }

        LiquidDensityTemperatureRelation& operator-=(const LiquidDensityTemperatureRelation other)
        {
            const float Tscale =  float(other.Tunits / Tunits);
            const float yscale = -float(other.yunits / yunits);
            for (std::size_t i = 0; i < other.dippr105s.size(); ++i)
            {
                dippr105s.push_back(yscale * compose(other.dippr105s[i], math::Scaling<float>(Tscale)));
            }
            for (std::size_t i = 0; i < other.dippr119s.size(); ++i)
            {
                dippr119s.push_back(yscale * compose(other.dippr119s[i], math::Scaling<float>(Tscale)));
            }
            intercept += yscale * other.intercept;
            return *this;
        }

    };

    // 20 uses, for density of liquids
    template<typename Tx, typename Ty>
    LiquidDensityTemperatureRelation get_dippr_temperature_relation_105( 
        const Tx Tunits, const Ty yunits,
        const float c1, const float c2, const float c3, const float c4,
        const float Tmin, const float Tmax
    ){
        return LiquidDensityTemperatureRelation(
            {ClampedDippr105(Tmin, Tmax, Dippr105(c1, c2, c3, c4))}, {}, Tunits, yunits, 0.0f, 0.0f);
    }

    LiquidDensityTemperatureRelation operator+(const LiquidDensityTemperatureRelation relation, const LiquidDensityTemperatureRelation other)
    {
        LiquidDensityTemperatureRelation result = relation;
        result += other;
        return result;
    }

    LiquidDensityTemperatureRelation operator-(const LiquidDensityTemperatureRelation relation, const LiquidDensityTemperatureRelation other)
    {
        LiquidDensityTemperatureRelation result = relation;
        result -= other;
        return result;
    }

    LiquidDensityTemperatureRelation operator+(const LiquidDensityTemperatureRelation relation, const si::density<double> offset)
    {
        LiquidDensityTemperatureRelation result = relation;
        result += offset;
        return result;
    }

    LiquidDensityTemperatureRelation operator-(const LiquidDensityTemperatureRelation relation, const si::density<double> offset)
    {
        LiquidDensityTemperatureRelation result = relation;
        result -= offset;
        return result;
    }

    LiquidDensityTemperatureRelation operator*(const LiquidDensityTemperatureRelation relation, const float scalar)
    {
        LiquidDensityTemperatureRelation result = relation;
        result *= scalar;
        return result;
    }

    LiquidDensityTemperatureRelation operator/(const LiquidDensityTemperatureRelation relation, const float scalar)
    {
        LiquidDensityTemperatureRelation result = relation;
        result /= scalar;
        return result;
    }

    LiquidDensityTemperatureRelation operator+(const si::density<double> offset, const LiquidDensityTemperatureRelation relation)
    {
        LiquidDensityTemperatureRelation result = relation;
        result += offset;
        return result;
    }

    LiquidDensityTemperatureRelation operator-(const si::density<double> offset, const LiquidDensityTemperatureRelation relation)
    {
        LiquidDensityTemperatureRelation result = relation;
        result *= 1.0f;
        result += offset;
        return result;
    }

    LiquidDensityTemperatureRelation operator-(const LiquidDensityTemperatureRelation relation)
    {
        LiquidDensityTemperatureRelation result = relation;
        result *= 1.0f;
        return result;
    }

    LiquidDensityTemperatureRelation operator*(const float scalar, const LiquidDensityTemperatureRelation relation)
    {
        LiquidDensityTemperatureRelation result = relation;
        result *= scalar;
        return result;
    }
}}

