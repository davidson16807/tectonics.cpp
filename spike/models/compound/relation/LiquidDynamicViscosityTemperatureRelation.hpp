#pragma once

#include <iostream>
#include <algorithm>

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/Sum.hpp>
#include <math/analytic/Clamped.hpp>
#include <math/analytic/NaturalLogarithm.hpp>
#include <units/si.hpp>

#include "ExponentiatedPolynomialRailyardRelation.hpp"

namespace compound {
namespace relation {

    using ClampedNaturalLogarithm = analytic::Clamped<double,analytic::NaturalLogarithm<double>>;

    /*
    `LiquidDynamicViscosityTemperatureRelation` consolidates many kinds of expressions
    that are commonly used to represent the relation between temperature and the viscosity or vapor pressure of liquids

    In principle it could be used to model any temperature dependant property 
    that intrinsically cannot assume a negative value and follows a logarithmic mixing law.
    */
    struct LiquidDynamicViscosityTemperatureRelation
    {
        analytic::PolynomialRailyard<double, -1, 10> exponentiated_terms;
        analytic::Sum<double,ClampedNaturalLogarithm> exponentiated_logarithms;
        analytic::PolynomialRailyard<double, 0, 2> linear_terms;

        si::temperature<double> Tunits;
        si::dynamic_viscosity<double>                      yunits;

        double known_max_fractional_error;

        // zero constructor
        LiquidDynamicViscosityTemperatureRelation():
            exponentiated_terms(),
            exponentiated_logarithms(),
            linear_terms(),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        // constant constructor
        LiquidDynamicViscosityTemperatureRelation(const si::dynamic_viscosity<double> k):
            exponentiated_terms(std::log(k/si::dynamic_viscosity<double>(1.0))),
            exponentiated_logarithms(),
            linear_terms(),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        template<int Qlo, int Qhi>
        LiquidDynamicViscosityTemperatureRelation(
            const analytic::PolynomialRailyard<double, Qlo, Qhi> exponentiated_terms,
            const analytic::Sum<double,ClampedNaturalLogarithm> exponentiated_logarithms,

            const si::temperature<double> Tunits,
            const si::dynamic_viscosity<double> yunits,

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

        LiquidDynamicViscosityTemperatureRelation(
            const analytic::PolynomialRailyard<double, 0, 2> linear_terms,

            const si::temperature<double> Tunits,
            const si::dynamic_viscosity<double> yunits,

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
        LiquidDynamicViscosityTemperatureRelation(
            const ExponentiatedPolynomialRailyardRelation<si::temperature<double>, si::dynamic_viscosity<double>, Qlo,Qhi>& relation
        ):
            exponentiated_terms(relation.yard),
            exponentiated_logarithms(),
            linear_terms(),

            Tunits(relation.xunits),
            yunits(relation.yunits),

            known_max_fractional_error(0.0)
        {
        }

        LiquidDynamicViscosityTemperatureRelation& operator=(const LiquidDynamicViscosityTemperatureRelation other)
        {
            exponentiated_terms = other.exponentiated_terms;
            exponentiated_logarithms = other.exponentiated_logarithms;
            linear_terms = other.linear_terms;

            Tunits = other.Tunits;
            yunits = other.yunits;

            known_max_fractional_error = other.known_max_fractional_error;

            return *this;
        }

        si::dynamic_viscosity<double> operator()(const si::temperature<double> temperature) const
        {
            const double T = double(temperature/Tunits);
            return (std::exp(exponentiated_terms(T) + exponentiated_logarithms(T)) + linear_terms(T)) * yunits;
        }

        LiquidDynamicViscosityTemperatureRelation& operator*=(const LiquidDynamicViscosityTemperatureRelation& other)
        {
            exponentiated_terms += other.exponentiated_terms;
            exponentiated_logarithms += other.exponentiated_logarithms;
            linear_terms += other.linear_terms;
            return *this;
        }

        LiquidDynamicViscosityTemperatureRelation& operator/=(const LiquidDynamicViscosityTemperatureRelation& other)
        {
            exponentiated_terms -= other.exponentiated_terms;
            exponentiated_logarithms -= other.exponentiated_logarithms;
            linear_terms -= other.linear_terms;
            return *this;
        }

        LiquidDynamicViscosityTemperatureRelation& operator*=(const double scalar)
        {
            linear_terms += scalar;
            return *this;
        }

        LiquidDynamicViscosityTemperatureRelation operator/=(const double scalar)
        {
            linear_terms -= scalar;
            return *this;
        }

    };

    LiquidDynamicViscosityTemperatureRelation operator*(const LiquidDynamicViscosityTemperatureRelation relation, const LiquidDynamicViscosityTemperatureRelation other)
    {
        LiquidDynamicViscosityTemperatureRelation result = relation;
        result *= other;
        return result;
    }

    LiquidDynamicViscosityTemperatureRelation operator/(const LiquidDynamicViscosityTemperatureRelation relation, const LiquidDynamicViscosityTemperatureRelation other)
    {
        LiquidDynamicViscosityTemperatureRelation result = relation;
        result /= other;
        return result;
    }

    LiquidDynamicViscosityTemperatureRelation operator*(const LiquidDynamicViscosityTemperatureRelation relation, const double scalar)
    {
        LiquidDynamicViscosityTemperatureRelation result = relation;
        result *= scalar;
        return result;
    }

    LiquidDynamicViscosityTemperatureRelation operator/(const LiquidDynamicViscosityTemperatureRelation relation, const double scalar)
    {
        LiquidDynamicViscosityTemperatureRelation result = relation;
        result /= scalar;
        return result;
    }

    LiquidDynamicViscosityTemperatureRelation operator*(const double scalar, const LiquidDynamicViscosityTemperatureRelation relation)
    {
        LiquidDynamicViscosityTemperatureRelation result = relation;
        result *= scalar;
        return result;
    }

    LiquidDynamicViscosityTemperatureRelation operator/(const double scalar, const LiquidDynamicViscosityTemperatureRelation relation)
    {
        return LiquidDynamicViscosityTemperatureRelation(scalar) / relation;
    }

    LiquidDynamicViscosityTemperatureRelation logarithmic_mix(const std::vector<LiquidDynamicViscosityTemperatureRelation>& relations, const std::vector<double>& ratios)
    {
        LiquidDynamicViscosityTemperatureRelation result;
        for (std::size_t i=0; i<relations.size(); i++){
            const double Tscale = double(relations[i].Tunits / result.Tunits);
            const double yscale = double(relations[i].yunits / result.yunits);
            result.exponentiated_terms      += yscale * ratios[i] * compose(relations[i].exponentiated_terms, analytic::Scaling(Tscale));
            result.exponentiated_logarithms += yscale * ratios[i] * compose(relations[i].exponentiated_logarithms, analytic::Scaling(Tscale));
        }
        return result;
    }

    // 42 uses, for viscosity and vapor pressures of liquids
    LiquidDynamicViscosityTemperatureRelation get_dippr_dynamic_viscosity_temperature_relation_101(
        const si::temperature<double> Tunits, const si::dynamic_viscosity<double> yunits,
        const double log_intercept, const double log_slope, const double log_log, const double log_exponentiated, const int exponent,
        const double Tmin, const double Tmax
    ){
        using P = analytic::Polynomial<double,-1,10>;
        using CP = analytic::Clamped<double,P>;
        using L = analytic::NaturalLogarithm<double>;
        using CL = analytic::Clamped<double,L>;
        P p(log_intercept);
        p[-1] = log_slope;
        p[exponent] += log_exponentiated;
        return relation::LiquidDynamicViscosityTemperatureRelation(
            analytic::Railyard<double,P>(CP(Tmin, Tmax, p)), 
            analytic::Sum<double,CL>(CL(Tmin, Tmax, L(log_log, 1.0))), 
            Tunits, yunits, 0.0);
    }

    // Letsou-Stiel method: https://chemicals.readthedocs.io/chemicals.viscosity.html?highlight=letsou%20stiel#chemicals.viscosity.Letsou_Stiel
    LiquidDynamicViscosityTemperatureRelation estimate_viscosity_as_liquid_from_letsou_stiel(
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
        return LiquidDynamicViscosityTemperatureRelation(
            analytic::PolynomialRailyard<double,0,2>(eta), 
            si::kelvin, si::pascal * si::second, 0.0); 
    }

}}

