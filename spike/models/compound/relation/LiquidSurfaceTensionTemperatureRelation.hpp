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

    struct ScaledComplimentExponent {
        float scale;
        float weight;
        float exponent;
        constexpr explicit ScaledComplimentExponent(const float scale, const float weight, const float exponent):
            scale(scale),
            weight(weight),
            exponent(exponent)
        {}
        // zero constructor
        constexpr explicit ScaledComplimentExponent():
            scale(0.0f),
            weight(0.0f),
            exponent(0.0f)
        {}
        // constant constructor
        constexpr explicit ScaledComplimentExponent(float k):
            scale(1.0f),
            weight(k),
            exponent(0.0f)
        {}
        constexpr ScaledComplimentExponent(const ScaledComplimentExponent& f):
            scale(f.scale),
            weight(f.weight),
            exponent(f.exponent)
        {}
        constexpr float operator()(const float x) const
        {
            return weight*std::pow(1.0f-x/scale, exponent);
        }
        constexpr ScaledComplimentExponent& operator*=(const float k)
        {
            weight *= k;
            return *this;
        }
        constexpr ScaledComplimentExponent& operator/=(const float k)
        {
            weight /= k;
            return *this;
        }
    };

    constexpr ScaledComplimentExponent operator-(const ScaledComplimentExponent f)
    {
        return ScaledComplimentExponent(f.scale, -f.weight, f.exponent);
    }
    constexpr ScaledComplimentExponent operator*(const ScaledComplimentExponent f, const float k)
    {
        return ScaledComplimentExponent(f.scale, f.weight*k, f.exponent);
    }
    constexpr ScaledComplimentExponent operator*(const float k, const ScaledComplimentExponent f)
    {
        return ScaledComplimentExponent(f.scale, f.weight*k, f.exponent);
    }
    constexpr ScaledComplimentExponent operator/(const ScaledComplimentExponent f, const float k)
    {
        return ScaledComplimentExponent(f.scale, f.weight/k, f.exponent);
    }
    constexpr ScaledComplimentExponent operator/(const float k, const ScaledComplimentExponent f)
    {
        return ScaledComplimentExponent(f.scale, k/f.weight, -f.exponent);
    }

    constexpr ScaledComplimentExponent compose(const ScaledComplimentExponent f, const math::Scaling<float> g)
    {
        return ScaledComplimentExponent(
            f.scale/g.factor,
            f.weight,
            f.exponent
        );
    }


    using ClampedScaledComplimentExponent = math::Clamped<float,ScaledComplimentExponent>;


    /*
    `LiquidSurfaceTensionTemperatureRelation` consolidates many kinds of expressions
    that are commonly used to represent the relation between temperature and the density of liquids
    */
    class LiquidSurfaceTensionTemperatureRelation
    {
        std::vector<ClampedScaledComplimentExponent> terms;
        math::PolynomialRailyard<float, 0, 2> spline;

        si::temperature<double>    Tunits;
        si::surface_energy<double> yunits;

    public:
        float known_max_fractional_error;

        // zero constructor
        LiquidSurfaceTensionTemperatureRelation():
            terms(),
            spline(),

            Tunits(si::standard_temperature),
            yunits(si::surface_energy<double>(1.0)),

            known_max_fractional_error(0.0)
        {
        }

        // constant constructor
        LiquidSurfaceTensionTemperatureRelation(const si::surface_energy<double> constant):
            terms(1, ClampedScaledComplimentExponent(constant/si::surface_energy<double>(1.0))),
            spline(),

            Tunits(si::standard_temperature),
            yunits(si::surface_energy<double>(1.0)),

            known_max_fractional_error(0.0)
        {
        }

        LiquidSurfaceTensionTemperatureRelation(
            const std::vector<ClampedScaledComplimentExponent> terms,
            const math::PolynomialRailyard<float, 0, 2> spline,

            const si::temperature<double> Tunits,
            const si::surface_energy<double> yunits,

            const float known_max_fractional_error
        ):
            terms(terms),
            spline(spline),

            Tunits(Tunits),
            yunits(yunits),

            known_max_fractional_error(known_max_fractional_error)
        {
        }

        LiquidSurfaceTensionTemperatureRelation(
            const relation::PolynomialRailyardRelation<si::temperature<double>, si::surface_energy<double>, 0, 1> relation
        ):
            terms(),
            spline(relation.spline),

            Tunits(relation.xunits),
            yunits(relation.yunits),

            known_max_fractional_error(0.0)
        {
        }

        LiquidSurfaceTensionTemperatureRelation(
            const relation::PolynomialRailyardRelation<si::temperature<double>, si::surface_energy<double>, 0, 2> relation
        ):
            terms(),
            spline(relation.spline),

            Tunits(relation.xunits),
            yunits(relation.yunits),

            known_max_fractional_error(0.0)
        {
        }

        LiquidSurfaceTensionTemperatureRelation& operator=(const LiquidSurfaceTensionTemperatureRelation other)
        {
            terms = other.terms;
            spline = other.spline;

            Tunits = other.Tunits;
            yunits = other.yunits;

            known_max_fractional_error = other.known_max_fractional_error;

            return *this;
        }

        si::surface_energy<double> operator()(const si::temperature<double> temperature) const
        {
            const float T = float(temperature/Tunits);
            float y = spline(T);
            for (std::size_t i = 0; i < terms.size(); ++i)
            {
                y += terms[i](T);
            }
            return y * yunits;
        }

        LiquidSurfaceTensionTemperatureRelation& operator+=(const si::surface_energy<double> offset)
        {
            terms.push_back(ClampedScaledComplimentExponent(offset/yunits));
            return *this;
        }

        LiquidSurfaceTensionTemperatureRelation& operator-=(const si::surface_energy<double> offset)
        {
            terms.push_back(ClampedScaledComplimentExponent(-offset/yunits));
            return *this;
        }

        LiquidSurfaceTensionTemperatureRelation& operator*=(const float scalar)
        {
            for (std::size_t i = 0; i < terms.size(); ++i)
            {
                terms[i] *= scalar;
            }
            spline *= scalar;
            return *this;
        }

        LiquidSurfaceTensionTemperatureRelation operator/=(const float scalar)
        {
            for (std::size_t i = 0; i < terms.size(); ++i)
            {
                terms[i] /= scalar;
            }
            spline /= scalar;
            return *this;
        }

        LiquidSurfaceTensionTemperatureRelation& operator+=(const LiquidSurfaceTensionTemperatureRelation other)
        {
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            for (std::size_t i = 0; i < other.terms.size(); ++i)
            {
                terms.push_back(yscale * compose(other.terms[i], math::Scaling<float>(Tscale)));
            }
            spline += yscale * compose(other.spline, math::Scaling<float>(Tscale));
            return *this;
        }

        LiquidSurfaceTensionTemperatureRelation& operator-=(const LiquidSurfaceTensionTemperatureRelation other)
        {
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            for (std::size_t i = 0; i < other.terms.size(); ++i)
            {
                terms.push_back(-yscale * compose(other.terms[i], math::Scaling<float>(Tscale)));
            }
            spline -= yscale * compose(other.spline, math::Scaling<float>(Tscale));
            return *this;
        }

    };

    // 15 uses, for liquid surface tension
    LiquidSurfaceTensionTemperatureRelation get_refprop_liquid_surface_tension_temperature_function( 
        const si::temperature<double> Tunits, const si::surface_energy<double> yunits,
        const float Tc, const float sigma0, const float n0, const float sigma1, const float n1, const float sigma2, const float n2,
        const float Tmin, const float Tmax
    ){
        using F = ClampedScaledComplimentExponent;
        using G = ScaledComplimentExponent;
        return LiquidSurfaceTensionTemperatureRelation({
            F(Tmin, Tmax, G(Tc, sigma0, n0)),
            F(Tmin, Tmax, G(Tc, sigma1, n1)),
            F(Tmin, Tmax, G(Tc, sigma2, n2))
        }, math::PolynomialRailyard<float, 0, 2>(), Tunits, yunits, 0.0);
    }
    // from Mulero (2012)

    // 3 uses, for liquid surface tension
    LiquidSurfaceTensionTemperatureRelation get_linear_liquid_surface_tension_temperature_function( 
        const si::temperature<double> Tunits, const si::surface_energy<double> yunits,
        const float TL, const float gammaTL, const float dgamma_dT
    ){
        using P = math::Polynomial<float, 0, 2>;
        using R = math::PolynomialRailcar<float, 0, 2>;
        const float oo = std::numeric_limits<float>::max();
        return LiquidSurfaceTensionTemperatureRelation({}, 
            math::PolynomialRailyard<float, 0, 2>({
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
        using P = math::Polynomial<float, 0, 2>;
        using R = math::PolynomialRailcar<float, 0, 2>;
        return LiquidSurfaceTensionTemperatureRelation({}, 
            math::PolynomialRailyard<float, 0, 2>({
                    R(Tmin, Tmax, P({intercept, slope, square})),
                }), Tunits, yunits, 0.0);
    }

    LiquidSurfaceTensionTemperatureRelation get_quadratic_liquid_surface_tension_temperature_function( 
        const si::celcius_type<double> Tunits, const si::surface_energy<double> yunits,
        const float intercept, const float slope, const float square, 
        const float Tmin, float Tmax
    ){
        using P = math::Polynomial<float, 0, 2>;
        using R = math::PolynomialRailcar<float, 0, 2>;
        return LiquidSurfaceTensionTemperatureRelation({}, 
            math::compose(
                math::PolynomialRailyard<float, 0, 2>({ 
                    R(Tmin, Tmax, P({intercept, slope, square})) 
                }), 
                math::Shifting<float>(-si::standard_temperature/si::kelvin)
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

