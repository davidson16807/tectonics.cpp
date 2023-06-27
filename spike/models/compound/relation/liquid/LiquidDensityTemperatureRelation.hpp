#pragma once

#include <iostream>
#include <algorithm>

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/Polynomial.hpp>
#include <math/analytic/Clamped.hpp>
#include <math/analytic/Sum.hpp>
#include <units/si.hpp>

#include <models/compound/dippr/Dippr105.hpp>
#include <models/compound/dippr/Dippr119Term.hpp>

namespace compound {
namespace relation {

    using ClampedDippr105 = analytic::Clamped<float,dippr::Dippr105>;
    using ClampedDippr119Term = analytic::Clamped<float,dippr::Dippr119Term>;
    using ClampedDippr105Sum = analytic::Sum<float,ClampedDippr105>;
    using ClampedDippr105TermSum = analytic::Sum<float,ClampedDippr119Term>;
    
    /*
    `LiquidDensityTemperatureRelation` consolidates many kinds of expressions
    that are commonly used to represent the relation between temperature and the density of liquids
    */
    class LiquidDensityTemperatureRelation
    {

        ClampedDippr105Sum dippr105s;
        ClampedDippr105TermSum dippr119s;

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
            const ClampedDippr105Sum dippr105s,
            const ClampedDippr105TermSum dippr119s,

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
            ClampedDippr119Term dippr119;
            return (intercept + dippr105s(T) + dippr119s(T)) * yunits;
        }

        LiquidDensityTemperatureRelation& operator*=(const float scalar)
        {
            dippr105s *= scalar;
            dippr119s *= scalar;
            intercept *= scalar;
            return *this;
        }

        LiquidDensityTemperatureRelation operator/=(const float scalar)
        {
            dippr105s /= scalar;
            dippr119s /= scalar;
            intercept /= scalar;
            intercept /= scalar;
            return *this;
        }

        LiquidDensityTemperatureRelation& operator+=(const LiquidDensityTemperatureRelation other)
        {
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            dippr105s += yscale * compose(other.dippr105s, analytic::Scaling<float>(Tscale));
            dippr119s += yscale * compose(other.dippr119s, analytic::Scaling<float>(Tscale));
            intercept += yscale * other.intercept;
            return *this;
        }

        LiquidDensityTemperatureRelation& operator-=(const LiquidDensityTemperatureRelation other)
        {
            const float Tscale =  float(other.Tunits / Tunits);
            const float yscale = -float(other.yunits / yunits);
            dippr105s -= yscale * compose(other.dippr105s, analytic::Scaling<float>(Tscale));
            dippr119s -= yscale * compose(other.dippr119s, analytic::Scaling<float>(Tscale));
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
            {ClampedDippr105(Tmin, Tmax, dippr::Dippr105(c1, c2, c3, c4))}, {}, Tunits, yunits, 0.0f, 0.0f);
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

