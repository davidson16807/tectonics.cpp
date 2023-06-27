#pragma once

#include <iostream>
#include <algorithm>

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/Rational.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/ScaledComplement.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/rails/RationalRailyard.hpp>
#include <units/si.hpp>


namespace compound {
namespace relation {

    /*
    `LiquidHeatCapacityTemperatureRelation` consolidates many kinds of expressions
    that are commonly used to represent the relation between temperature and the density of liquids
    */
    class LiquidHeatCapacityTemperatureRelation
    {
        analytic::Railyard<float, analytic::Rational<float, 0,6,0,1>> temperature_terms;

        si::temperature<double>  Tunits;
        si::specific_heat_capacity<double> yunits;

    public:
        float known_max_fractional_error;

        // zero constructor
        LiquidHeatCapacityTemperatureRelation():
            temperature_terms(),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        // constant constructor
        LiquidHeatCapacityTemperatureRelation(const si::specific_heat_capacity<double> constant):
            temperature_terms(constant/si::specific_heat_capacity<double>(1.0)),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        // copy constructor
        template<int Plo, int Phi>
        LiquidHeatCapacityTemperatureRelation(
            const relation::PolynomialRailyardRelation<si::temperature<double>, si::specific_heat_capacity<double>, Plo, Phi> relation
        ):
            temperature_terms(relation.yard),

            Tunits(relation.xunits),
            yunits(relation.yunits),

            known_max_fractional_error(0.0)
        {
        }

        LiquidHeatCapacityTemperatureRelation(
            const analytic::Railyard<float, analytic::Rational<float, 0, 6, 0, 1>> temperature_terms,

            const si::temperature<double> Tunits,
            const si::specific_heat_capacity<double> yunits,

            const float known_max_fractional_error
        ):
            temperature_terms(temperature_terms),

            Tunits(Tunits),
            yunits(yunits),

            known_max_fractional_error(known_max_fractional_error)
        {
        }

        LiquidHeatCapacityTemperatureRelation& operator=(const LiquidHeatCapacityTemperatureRelation other)
        {
            temperature_terms = other.temperature_terms;

            Tunits = other.Tunits;
            yunits = other.yunits;

            known_max_fractional_error = other.known_max_fractional_error;

            return *this;
        }

        si::specific_heat_capacity<double> operator()(const si::temperature<double> temperature) const
        {
            const float T = float(temperature/Tunits);
            return (temperature_terms(T)) * yunits;
        }

        LiquidHeatCapacityTemperatureRelation& operator+=(const si::specific_heat_capacity<double> offset)
        {
            using R = analytic::Rational<float, 0, 6, 0, 1>;
            using RC = analytic::Railcar<float, R>;
            temperature_terms.cars.push_back(RC(offset/yunits));
            return *this;
        }

        LiquidHeatCapacityTemperatureRelation& operator-=(const si::specific_heat_capacity<double> offset)
        {
            using R = analytic::Rational<float, 0, 6, 0, 1>;
            using RC = analytic::Railcar<float, R>;
            temperature_terms.cars.push_back(RC(-offset/yunits));
            return *this;
        }

        LiquidHeatCapacityTemperatureRelation& operator*=(const float scalar)
        {
            temperature_terms *= scalar;
            return *this;
        }

        LiquidHeatCapacityTemperatureRelation operator/=(const float scalar)
        {
            temperature_terms /= scalar;
            return *this;
        }

        LiquidHeatCapacityTemperatureRelation& operator+=(const LiquidHeatCapacityTemperatureRelation other)
        {
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            temperature_terms += yscale * compose(other.temperature_terms, analytic::Scaling<float>(Tscale));
            return *this;
        }

        LiquidHeatCapacityTemperatureRelation& operator-=(const LiquidHeatCapacityTemperatureRelation other)
        {
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            temperature_terms -= yscale * compose(other.temperature_terms, analytic::Scaling<float>(Tscale));
            return *this;
        }

    };


    LiquidHeatCapacityTemperatureRelation operator+(const LiquidHeatCapacityTemperatureRelation relation, const LiquidHeatCapacityTemperatureRelation other)
    {
        LiquidHeatCapacityTemperatureRelation result = relation;
        result += other;
        return result;
    }

    LiquidHeatCapacityTemperatureRelation operator-(const LiquidHeatCapacityTemperatureRelation relation, const LiquidHeatCapacityTemperatureRelation other)
    {
        LiquidHeatCapacityTemperatureRelation result = relation;
        result -= other;
        return result;
    }

    LiquidHeatCapacityTemperatureRelation operator+(const LiquidHeatCapacityTemperatureRelation relation, const si::specific_heat_capacity<double> offset)
    {
        LiquidHeatCapacityTemperatureRelation result = relation;
        result += offset;
        return result;
    }

    LiquidHeatCapacityTemperatureRelation operator-(const LiquidHeatCapacityTemperatureRelation relation, const si::specific_heat_capacity<double> offset)
    {
        LiquidHeatCapacityTemperatureRelation result = relation;
        result -= offset;
        return result;
    }

    LiquidHeatCapacityTemperatureRelation operator*(const LiquidHeatCapacityTemperatureRelation relation, const float scalar)
    {
        LiquidHeatCapacityTemperatureRelation result = relation;
        result *= scalar;
        return result;
    }

    LiquidHeatCapacityTemperatureRelation operator/(const LiquidHeatCapacityTemperatureRelation relation, const float scalar)
    {
        LiquidHeatCapacityTemperatureRelation result = relation;
        result /= scalar;
        return result;
    }

    LiquidHeatCapacityTemperatureRelation operator+(const si::specific_heat_capacity<double> offset, const LiquidHeatCapacityTemperatureRelation relation)
    {
        LiquidHeatCapacityTemperatureRelation result = relation;
        result += offset;
        return result;
    }

    LiquidHeatCapacityTemperatureRelation operator-(const si::specific_heat_capacity<double> offset, const LiquidHeatCapacityTemperatureRelation relation)
    {
        LiquidHeatCapacityTemperatureRelation result = relation;
        result *= 1.0f;
        result += offset;
        return result;
    }

    LiquidHeatCapacityTemperatureRelation operator-(const LiquidHeatCapacityTemperatureRelation relation)
    {
        LiquidHeatCapacityTemperatureRelation result = relation;
        result *= -1.0f;
        return result;
    }

    LiquidHeatCapacityTemperatureRelation operator*(const float scalar, const LiquidHeatCapacityTemperatureRelation relation)
    {
        LiquidHeatCapacityTemperatureRelation result = relation;
        result *= scalar;
        return result;
    }


    // 5 uses, for heat capacities of liquids
    LiquidHeatCapacityTemperatureRelation get_dippr_liquid_heat_capacity_temperature_relation_114( 
        const si::temperature<double> Tc, const si::specific_heat_capacity<double> yunits,
        const float c1, const float c2, const float c3, const float c4, const float c5,
        const float Tmin, float Tmax
    ){
        const float oo = std::numeric_limits<float>::max();
        using P = analytic::Polynomial<float,-1,5>;
        using R = analytic::Rational<float, 0,6,0,1>;
        using RC = analytic::Railcar<float, R>;
        analytic::Identity<float> T;
        R r = analytic::compose(
            P({c1*c1, c2, -2.0f*c1*c3, -c1*c4, -c3*c3/3.0f, -c3*c4/2.0f, -c4*c4/5.0f}), 
            1.0f-T/float(Tc/si::kelvin));
        return LiquidHeatCapacityTemperatureRelation(
            analytic::Railyard<float, R>({
                RC(-oo,  Tmin, R(r(Tmin))),
                RC(Tmin, Tmax, r),
                RC(Tmax, oo,   R(r(Tmin))),
            }), si::kelvin, yunits, 0.0f);
    }


}}

