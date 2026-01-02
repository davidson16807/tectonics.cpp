#pragma once

#include <iostream>
#include <algorithm>

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/Clamped.hpp>
#include <math/analytic/Sum.hpp>
#include <unit/si.hpp>

#include <model/compound/dippr/Dippr106.hpp>

namespace compound {
namespace relation {

    using ClampedDippr106 = analytic::Clamped<float,dippr::Dippr106>;
    using ClampedDippr106Sum = analytic::Sum<float,ClampedDippr106>;

    /*
    `LatentHeatTemperatureRelation` consolidates many kinds of expressions
    that are commonly used to represent the relation between temperature and the density of liquids
    */
    class LatentHeatTemperatureRelation
    {

        analytic::Sum<float,ClampedDippr106> dippr106s;

        si::temperature<double>    Tunits;
        si::specific_energy<double> yunits;

    public:
        float intercept;
        float known_max_fractional_error;

        // zero constructor
        explicit LatentHeatTemperatureRelation():
            dippr106s(),

            Tunits(1.0),
            yunits(1.0),

            intercept(0.0),
            known_max_fractional_error(0.0)
        {
        }

        // constant constructor
        LatentHeatTemperatureRelation(const si::specific_energy<double> constant):
            dippr106s(),

            Tunits(1.0),
            yunits(1.0),

            intercept(constant/si::specific_energy<double>(1.0)),
            known_max_fractional_error(0.0)
        {
        }

        explicit LatentHeatTemperatureRelation(
            const ClampedDippr106Sum& dippr106s,

            const si::temperature<double> Tunits,
            const si::specific_energy<double> yunits,

            const float intercept,
            const float known_max_fractional_error
        ):
            dippr106s(dippr106s),

            Tunits(Tunits),
            yunits(yunits),

            intercept(intercept),
            known_max_fractional_error(known_max_fractional_error)
        {
        }

        LatentHeatTemperatureRelation& operator=(const LatentHeatTemperatureRelation& other)
        {
            dippr106s = other.dippr106s;

            Tunits = other.Tunits;
            yunits = other.yunits;

            intercept = other.intercept;
            known_max_fractional_error = other.known_max_fractional_error;

            return *this;
        }

        si::specific_energy<double> operator()(const si::temperature<double> temperature) const
        {
            const float T = float(temperature/Tunits);
            return (dippr106s(T) + intercept) * yunits;
        }

        LatentHeatTemperatureRelation& operator+=(const LatentHeatTemperatureRelation other)
        {
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            dippr106s += yscale * compose(other.dippr106s, analytic::Scaling<float>(Tscale));
            intercept += yscale * other.intercept;
            return *this;
        }

        LatentHeatTemperatureRelation& operator-=(const LatentHeatTemperatureRelation other)
        {
            const float Tscale = float(other.Tunits / Tunits);
            const float yscale = float(other.yunits / yunits);
            dippr106s -= yscale * compose(other.dippr106s, analytic::Scaling<float>(Tscale));
            intercept -= yscale * other.intercept;
            return *this;
        }

        LatentHeatTemperatureRelation& operator+=(const si::specific_energy<double> offset)
        {
            intercept += offset/yunits;
            return *this;
        }

        LatentHeatTemperatureRelation& operator-=(const si::specific_energy<double> offset)
        {
            intercept -= -offset/yunits;
            return *this;
        }

        LatentHeatTemperatureRelation& operator*=(const float scalar)
        {
            dippr106s *= scalar;
            intercept *= scalar;
            return *this;
        }

        LatentHeatTemperatureRelation& operator/=(const float scalar)
        {
            dippr106s /= scalar;
            intercept /= scalar;
            return *this;
        }
    };

    LatentHeatTemperatureRelation get_pitzer_latent_heat_of_vaporization_temperature_relation(
        const si::molar_mass<double> molar_mass, const si::temperature<double> critical_temperature, const double acentric_factor
    ){
        const auto yunits = si::specific_energy<double>(1.0);
        const auto Tunits = si::temperature<double>(1.0);
        const auto k = (si::universal_gas_constant * critical_temperature / molar_mass) / yunits;
        const double w = std::max(acentric_factor, 0.0);
        const float Tc = float(critical_temperature / Tunits);
        const float a = float(7.08*k);
        const float b = std::max(float(10.95*w*k), a);
        return LatentHeatTemperatureRelation(
            ClampedDippr106Sum{
                ClampedDippr106(0.0, Tc, dippr::Dippr106(a, 0.354f, 0.0f, 0.0f, Tc)),
                ClampedDippr106(0.0, Tc, dippr::Dippr106(b, 0.456f, 0.0f, 0.0f, Tc)),
            }, Tunits, yunits, 0.0f, 0.0f);
    }

    LatentHeatTemperatureRelation get_dippr_temperature_relation_106(
        const si::temperature<double> Tunits, const si::specific_energy<double> yunits,
        const float Tc, const float c1, const float c2, const float c3, const float c4,
        const float Tmin, const float Tmax
    ){
        return LatentHeatTemperatureRelation(
            ClampedDippr106Sum{ ClampedDippr106(Tmin, Tmax, dippr::Dippr106(c1, c2, c3, c4, Tc)) }, 
            Tunits, yunits, 0.0f, 0.0f);
    }

    LatentHeatTemperatureRelation operator+(const LatentHeatTemperatureRelation relation, const LatentHeatTemperatureRelation other)
    {
        LatentHeatTemperatureRelation result = relation;
        result += other;
        return result;
    }

    LatentHeatTemperatureRelation operator-(const LatentHeatTemperatureRelation relation, const LatentHeatTemperatureRelation other)
    {
        LatentHeatTemperatureRelation result = relation;
        result -= other;
        return result;
    }

    LatentHeatTemperatureRelation operator+(const LatentHeatTemperatureRelation relation, const si::specific_energy<double> offset)
    {
        LatentHeatTemperatureRelation result = relation;
        result += offset;
        return result;
    }

    LatentHeatTemperatureRelation operator-(const LatentHeatTemperatureRelation relation, const si::specific_energy<double> offset)
    {
        LatentHeatTemperatureRelation result = relation;
        result -= offset;
        return result;
    }

    LatentHeatTemperatureRelation operator*(const LatentHeatTemperatureRelation relation, const float scalar)
    {
        LatentHeatTemperatureRelation result = relation;
        result *= scalar;
        return result;
    }

    LatentHeatTemperatureRelation operator/(const LatentHeatTemperatureRelation relation, const float scalar)
    {
        LatentHeatTemperatureRelation result = relation;
        result /= scalar;
        return result;
    }

    LatentHeatTemperatureRelation operator+(const si::specific_energy<double> offset, const LatentHeatTemperatureRelation relation)
    {
        LatentHeatTemperatureRelation result = relation;
        result += offset;
        return result;
    }

    LatentHeatTemperatureRelation operator-(const si::specific_energy<double> offset, const LatentHeatTemperatureRelation relation)
    {
        LatentHeatTemperatureRelation result = relation;
        result *= 1.0f;
        result += offset;
        return result;
    }

    LatentHeatTemperatureRelation operator-(const LatentHeatTemperatureRelation relation)
    {
        LatentHeatTemperatureRelation result = relation;
        result *= -1.0f;
        return result;
    }

    LatentHeatTemperatureRelation operator*(const float scalar, const LatentHeatTemperatureRelation relation)
    {
        LatentHeatTemperatureRelation result = relation;
        result *= scalar;
        return result;
    }

}}

