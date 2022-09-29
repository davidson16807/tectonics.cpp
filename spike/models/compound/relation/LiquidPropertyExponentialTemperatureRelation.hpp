#pragma once

#include <iostream>
#include <algorithm>

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/PolynomialRailyard.hpp>
#include <math/analytic/Sum.hpp>
#include <math/analytic/Clamped.hpp>
#include <math/analytic/NaturalLogarithm.hpp>
#include <units/si.hpp>

#include "ExponentiatedPolynomialRailyardRelation.hpp"

namespace compound {
namespace relation {

    using ClampedNaturalLogarithm = analytic::Clamped<float,analytic::NaturalLogarithm<float>>;

    /*
    `LiquidPropertyExponentialTemperatureRelation` consolidates many kinds of expressions
    that are commonly used to represent the relation between temperature and the viscosity or vapor pressure of liquids

    In principle it could be used to model any temperature dependant property 
    that intrinsically cannot assume a negative value and follows a logarithmic mixing law.
    */
    template<typename Ty, int Plo, int Phi>
    class LiquidPropertyExponentialTemperatureRelation
    {
        analytic::PolynomialRailyard<double, Plo, Phi> exponentiated_terms;
        analytic::Sum<float,ClampedNaturalLogarithm> exponentiated_logarithms;
        analytic::PolynomialRailyard<double, 0, 2> linear_terms;

        si::temperature<double> Tunits;
        Ty                      yunits;

    public:
        double known_max_fractional_error;

        // zero constructor
        constexpr LiquidPropertyExponentialTemperatureRelation():
            exponentiated_terms(),
            exponentiated_logarithms(),
            linear_terms(),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        // constant constructor
        constexpr LiquidPropertyExponentialTemperatureRelation(const Ty k):
            exponentiated_terms(std::log(k/Ty(1.0))),
            exponentiated_logarithms(),
            linear_terms(),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        template<int Qlo, int Qhi>
        constexpr LiquidPropertyExponentialTemperatureRelation(
            const analytic::PolynomialRailyard<double, Qlo, Qhi> exponentiated_terms,
            const analytic::Sum<float,ClampedNaturalLogarithm> exponentiated_logarithms,

            const si::temperature<double> Tunits,
            const Ty yunits,

            const double known_max_fractional_error
        ):
            exponentiated_terms(exponentiated_terms),
            exponentiated_logarithms(exponentiated_logarithms),
            linear_terms(),

            Tunits(Tunits),
            yunits(yunits),

            known_max_fractional_error(known_max_fractional_error)
        {
        }

        constexpr LiquidPropertyExponentialTemperatureRelation(
            const analytic::PolynomialRailyard<double, 0, 2> linear_terms,

            const si::temperature<double> Tunits,
            const Ty yunits,

            const double known_max_fractional_error
        ):
            exponentiated_terms(),
            exponentiated_logarithms(),
            linear_terms(linear_terms),

            Tunits(Tunits),
            yunits(yunits),

            known_max_fractional_error(known_max_fractional_error)
        {
        }

        template<int Qlo, int Qhi>
        constexpr LiquidPropertyExponentialTemperatureRelation(
            const ExponentiatedPolynomialRailyardRelation<si::temperature<double>, Ty, Qlo,Qhi>& relation
        ):
            exponentiated_terms(relation.yard),
            exponentiated_logarithms(),

            Tunits(relation.xunits),
            yunits(relation.yunits),

            known_max_fractional_error(0.0)
        {
        }

        constexpr LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>& operator=(const LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> other)
        {
            exponentiated_terms = other.exponentiated_terms;
            exponentiated_logarithms = other.exponentiated_logarithms;
            linear_terms = other.linear_terms;

            Tunits = other.Tunits;
            yunits = other.yunits;

            known_max_fractional_error = other.known_max_fractional_error;

            return *this;
        }

        Ty operator()(const si::temperature<double> temperature) const
        {
            const double T = double(temperature/Tunits);
            return (std::exp(exponentiated_terms(T) + exponentiated_logarithms(T)) + linear_terms(T)) * yunits;
        }

        LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>& operator*=(const LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>& other)
        {
            exponentiated_terms += other.exponentiated_terms;
            exponentiated_logarithms += other.exponentiated_logarithms;
            linear_terms += other.linear_terms;
            return *this;
        }

        LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>& operator/=(const LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>& other)
        {
            exponentiated_terms -= other.exponentiated_terms;
            exponentiated_logarithms -= other.exponentiated_logarithms;
            linear_terms -= other.linear_terms;
            return *this;
        }

        LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>& operator*=(const double scalar)
        {
            linear_terms += scalar;
            return *this;
        }

        LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> operator/=(const double scalar)
        {
            linear_terms -= scalar;
            return *this;
        }

    };

    template<typename Ty, int Plo, int Phi>
    LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> operator*(const LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> relation, const LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> other)
    {
        LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> result = relation;
        result *= other;
        return result;
    }

    template<typename Ty, int Plo, int Phi>
    LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> operator/(const LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> relation, const LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> other)
    {
        LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> result = relation;
        result /= other;
        return result;
    }

    template<typename Ty, int Plo, int Phi>
    LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> operator*(const LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> relation, const double scalar)
    {
        LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> result = relation;
        result *= scalar;
        return result;
    }

    template<typename Ty, int Plo, int Phi>
    LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> operator/(const LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> relation, const double scalar)
    {
        LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> result = relation;
        result /= scalar;
        return result;
    }

    template<typename Ty, int Plo, int Phi>
    LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> operator*(const double scalar, const LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> relation)
    {
        LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> result = relation;
        result *= scalar;
        return result;
    }

    template<typename Ty, int Plo, int Phi>
    LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> operator/(const double scalar, const LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> relation)
    {
        return LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>(scalar) / relation;
    }


    template<typename Ty, int Plo, int Phi>
    LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> logarithmic_mix(const std::vector<LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>>& relations, const std::vector<float>& ratios)
    {
        LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> result;
        for (std::size_t i=0; i<relations.size(); i++){
            const float Tscale = float(relations[i].xunits / result.xunits);
            const float yscale = float(relations[i].yunits / result.yunits);
            result.exponentiated_terms      += yscale * ratios[i] * compose(relations[i].exponentiated_terms, analytic::Scaling(Tscale));
            result.exponentiated_logarithms += yscale * ratios[i] * compose(relations[i].exponentiated_logarithms, analytic::Scaling(Tscale));
        }
        return result;
    }


    // 42 uses, for viscosity and vapor pressures of liquids
    template<int Plo, int Phi, typename Ty>
    LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> get_dippr_temperature_relation_101(
        const si::temperature<double> Tunits, const Ty yunits,
        const double log_intercept, const double log_slope, const double log_log, const double log_exponentiated, const int exponent,
        const double Tmin, const double Tmax
    ){
        const double oo = std::numeric_limits<double>::max();
        using P = analytic::Polynomial<double, Plo, Phi>;
        using R = analytic::PolynomialRailcar<double, Plo, Phi>;
        P p(log_intercept);
        p[-1] = log_slope;
        p[exponent] += log_exponentiated;
        return relation::LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>(
            analytic::PolynomialRailyard<double, Plo, Phi>({
                R(-oo,  Tmin, P(p(Tmin))),
                R(Tmin, Tmax, p),
                R(Tmax,   oo, P(p(Tmax))),
            }), 
            analytic::Sum<float,ClampedNaturalLogarithm>({
                ClampedNaturalLogarithm(Tmin, Tmax, analytic::NaturalLogarithm<float>(log_log))
            }), 
            Tunits, yunits, 0.0);
    }

    // Letsou-Stiel method: https://chemicals.readthedocs.io/chemicals.viscosity.html?highlight=letsou%20stiel#chemicals.viscosity.Letsou_Stiel
    template<typename Ty, int Plo, int Phi>
    LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> estimate_viscosity_as_liquid_from_letsou_stiel(
        const double acentric_factor,
        const si::molar_mass<double> molar_mass, 
        const si::temperature<double> critical_temperature, 
        const si::pressure<double> critical_pressure
    ){
        analytic::Identity<double> T;
        double Tc = critical_temperature / si::kelvin;
        double w = acentric_factor;
        double M = (molar_mass / (si::gram / si::mole));
        double pc = (critical_pressure / si::pascal);
        double zeta = 2173.424 * pow(Tc, 1.0/6.0)  /  (sqrt(M) * pow(pc, 2.0/3.0));
        auto Tr = 1.0 - T / Tc;
        auto eta_zeta_0 = (1.5174 - 2.135 * Tr + 0.75 * Tr * Tr) * 1e-5;
        auto eta_zeta_1 = (4.2552 - 7.674 * Tr + 3.40 * Tr * Tr) * 1e-5;
        auto eta_zeta = eta_zeta_0 + w * eta_zeta_1;
        auto eta = eta_zeta / zeta;
        return LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>(analytic::PolynomialRailyard<double,0,2>(eta), si::kelvin, si::pascal * si::second, 0.0); 
    }

}}

