#pragma once

#include <iostream>
#include <algorithm>

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/Polynomial.hpp>
#include <math/analytic/Clamped.hpp>
#include <math/analytic/Exponent.hpp>
#include <math/analytic/ScaledComplement.hpp>
#include <units/si.hpp>


namespace compound {
namespace relation {

    using ScaledComplementExponent = analytic::ScaledComplement<float,analytic::Exponent<float>>;
    using ClampedScaledComplementExponent = analytic::Clamped<float,ScaledComplementExponent>;

    /*
    `LiquidSurfaceTensionTemperatureRelation` consolidates many kinds of expressions
    that are commonly used to represent the relation between temperature and the density of liquids
    */
    class LiquidSurfaceTensionTemperatureRelation
    {
        analytic::Sum<float,ClampedScaledComplementExponent> terms;
        analytic::PolynomialRailyard<float, 0, 2> yard;

        si::temperature<double>    Tunits;
        si::surface_energy<double> yunits;

    public:
        float known_max_fractional_error;

        // zero constructor
        LiquidSurfaceTensionTemperatureRelation():
            terms(),
            yard(),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        // constant constructor
        LiquidSurfaceTensionTemperatureRelation(const si::surface_energy<double> constant):
            terms(ClampedScaledComplementExponent(constant/si::surface_energy<double>(1.0))),
            yard(),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        LiquidSurfaceTensionTemperatureRelation(
            const analytic::Sum<float,ClampedScaledComplementExponent> terms,
            const analytic::PolynomialRailyard<float, 0, 2> yard,

            const si::temperature<double> Tunits,
            const si::surface_energy<double> yunits,

            const float known_max_fractional_error
        ):
            terms(terms),
            yard(yard),

            Tunits(Tunits),
            yunits(yunits),

            known_max_fractional_error(known_max_fractional_error)
        {
        }

        LiquidSurfaceTensionTemperatureRelation(
            const relation::PolynomialRailyardRelation<si::temperature<double>, si::surface_energy<double>, 0, 1> relation
        ):
            terms(),
            yard(relation.yard),

            Tunits(relation.xunits),
            yunits(relation.yunits),

            known_max_fractional_error(0.0)
        {
        }

        LiquidSurfaceTensionTemperatureRelation(
            const relation::PolynomialRailyardRelation<si::temperature<double>, si::surface_energy<double>, 0, 2> relation
        ):
            terms(),
            yard(relation.yard),

            Tunits(relation.xunits),
            yunits(relation.yunits),

            known_max_fractional_error(0.0)
        {
        }

        LiquidSurfaceTensionTemperatureRelation& operator=(const LiquidSurfaceTensionTemperatureRelation other)
        {
            terms = other.terms;
            yard = other.yard;

            Tunits = other.Tunits;
            yunits = other.yunits;

            known_max_fractional_error = other.known_max_fractional_error;

            return *this;
        }

        si::surface_energy<double> operator()(const si::temperature<double> temperature) const
        {
            const float T = float(temperature/Tunits);
            return (yard(T) + terms(T)) * yunits;
        }

        LiquidSurfaceTensionTemperatureRelation& operator+=(const si::surface_energy<double> offset)
        {
            terms += ClampedScaledComplementExponent(offset/yunits);
            return *this;
        }

        LiquidSurfaceTensionTemperatureRelation& operator-=(const si::surface_energy<double> offset)
        {
            terms += ClampedScaledComplementExponent(-offset/yunits);
            return *this;
        }

        LiquidSurfaceTensionTemperatureRelation& operator*=(const float scalar)
        {
            terms *= scalar;
            yard *= scalar;
            return *this;
        }

        LiquidSurfaceTensionTemperatureRelation operator/=(const float scalar)
        {
            terms /= scalar;
            yard /= scalar;
            return *this;
        }

        LiquidSurfaceTensionTemperatureRelation& operator+=(const LiquidSurfaceTensionTemperatureRelation other)
        {
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            terms += yscale * compose(other.terms, analytic::Scaling<float>(Tscale));
            yard += yscale * compose(other.yard, analytic::Scaling<float>(Tscale));
            return *this;
        }

        LiquidSurfaceTensionTemperatureRelation& operator-=(const LiquidSurfaceTensionTemperatureRelation other)
        {
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            terms -= yscale * compose(other.terms, analytic::Scaling<float>(Tscale));
            yard -= yscale * compose(other.yard, analytic::Scaling<float>(Tscale));
            return *this;
        }

    };

    // 15 uses, for liquid surface tension
    LiquidSurfaceTensionTemperatureRelation get_refprop_liquid_surface_tension_temperature_function( 
        const si::temperature<double> Tunits, const si::surface_energy<double> yunits,
        const float Tc, const float sigma0, const float n0, const float sigma1, const float n1, const float sigma2, const float n2,
        const float Tmin, const float Tmax
    ){
        using F = ClampedScaledComplementExponent;
        using G = ScaledComplementExponent;
        using H = analytic::Exponent<float>;
        return LiquidSurfaceTensionTemperatureRelation({
            F(Tmin, Tmax, G(Tc, H(sigma0, n0))),
            F(Tmin, Tmax, G(Tc, H(sigma1, n1))),
            F(Tmin, Tmax, G(Tc, H(sigma2, n2)))
        }, analytic::PolynomialRailyard<float, 0, 2>(), Tunits, yunits, 0.0);
    }
    // from Mulero (2012)

    // 3 uses, for liquid surface tension
    LiquidSurfaceTensionTemperatureRelation get_linear_liquid_surface_tension_temperature_function( 
        const si::temperature<double> Tunits, const si::surface_energy<double> yunits,
        const float TL, const float gammaTL, const float dgamma_dT
    ){
        using P = analytic::Polynomial<float, 0, 2>;
        using R = analytic::PolynomialRailcar<float, 0, 2>;
        const float oo = std::numeric_limits<float>::max();
        return LiquidSurfaceTensionTemperatureRelation({}, 
            analytic::PolynomialRailyard<float, 0, 2>({
                    R(-oo, oo, P({gammaTL - dgamma_dT*TL, dgamma_dT, 0.0f})),
                }), Tunits, yunits, 0.0);
        // NOTE: the above is equivalent to:
        // ( gammaTL + dgamma_dT * (T-TL) );
    }

    /*
    `get_dippr_quartic_temperature_function_100()` is equivalent to dippr function 100,
    for liquid thermal conductivity, heat capacity, and solid density
    */
    LiquidSurfaceTensionTemperatureRelation get_quadratic_liquid_surface_tension_temperature_function( 
        const si::temperature<double> Tunits, const si::surface_energy<double> yunits,
        const float intercept, const float slope, const float square, 
        const float Tmin, float Tmax
    ){
        using P = analytic::Polynomial<float, 0, 2>;
        using R = analytic::PolynomialRailcar<float, 0, 2>;
        return LiquidSurfaceTensionTemperatureRelation({}, 
            analytic::PolynomialRailyard<float, 0, 2>({
                    R(Tmin, Tmax, P({intercept, slope, square})),
                }), Tunits, yunits, 0.0);
    }

    LiquidSurfaceTensionTemperatureRelation get_quadratic_liquid_surface_tension_temperature_function( 
        const si::celcius_type<double> Tunits, const si::surface_energy<double> yunits,
        const float intercept, const float slope, const float square, 
        const float Tmin, float Tmax
    ){
        using P = analytic::Polynomial<float, 0, 2>;
        using R = analytic::PolynomialRailcar<float, 0, 2>;
        return LiquidSurfaceTensionTemperatureRelation({}, 
            analytic::compose(
                analytic::PolynomialRailyard<float, 0, 2>({ 
                    R(Tmin, Tmax, P({intercept, slope, square})) 
                }), 
                analytic::Shifting<float>(-si::standard_temperature/si::kelvin)
            ), si::kelvin, yunits, 0.0);
    }

    LiquidSurfaceTensionTemperatureRelation operator+(const LiquidSurfaceTensionTemperatureRelation relation, const LiquidSurfaceTensionTemperatureRelation other)
    {
        LiquidSurfaceTensionTemperatureRelation result = relation;
        result += other;
        return result;
    }

    LiquidSurfaceTensionTemperatureRelation operator-(const LiquidSurfaceTensionTemperatureRelation relation, const LiquidSurfaceTensionTemperatureRelation other)
    {
        LiquidSurfaceTensionTemperatureRelation result = relation;
        result -= other;
        return result;
    }

    LiquidSurfaceTensionTemperatureRelation operator+(const LiquidSurfaceTensionTemperatureRelation relation, const si::surface_energy<double> offset)
    {
        LiquidSurfaceTensionTemperatureRelation result = relation;
        result += offset;
        return result;
    }

    LiquidSurfaceTensionTemperatureRelation operator-(const LiquidSurfaceTensionTemperatureRelation relation, const si::surface_energy<double> offset)
    {
        LiquidSurfaceTensionTemperatureRelation result = relation;
        result -= offset;
        return result;
    }

    LiquidSurfaceTensionTemperatureRelation operator*(const LiquidSurfaceTensionTemperatureRelation relation, const float scalar)
    {
        LiquidSurfaceTensionTemperatureRelation result = relation;
        result *= scalar;
        return result;
    }

    LiquidSurfaceTensionTemperatureRelation operator/(const LiquidSurfaceTensionTemperatureRelation relation, const float scalar)
    {
        LiquidSurfaceTensionTemperatureRelation result = relation;
        result /= scalar;
        return result;
    }

    LiquidSurfaceTensionTemperatureRelation operator+(const si::surface_energy<double> offset, const LiquidSurfaceTensionTemperatureRelation relation)
    {
        LiquidSurfaceTensionTemperatureRelation result = relation;
        result += offset;
        return result;
    }

    LiquidSurfaceTensionTemperatureRelation operator-(const si::surface_energy<double> offset, const LiquidSurfaceTensionTemperatureRelation relation)
    {
        LiquidSurfaceTensionTemperatureRelation result = relation;
        result *= 1.0f;
        result += offset;
        return result;
    }

    LiquidSurfaceTensionTemperatureRelation operator-(const LiquidSurfaceTensionTemperatureRelation relation)
    {
        LiquidSurfaceTensionTemperatureRelation result = relation;
        result *= -1.0f;
        return result;
    }

    LiquidSurfaceTensionTemperatureRelation operator*(const float scalar, const LiquidSurfaceTensionTemperatureRelation relation)
    {
        LiquidSurfaceTensionTemperatureRelation result = relation;
        result *= scalar;
        return result;
    }

}}

