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

    /*
    `LiquidVaporPressureTemperatureRelation` consolidates many kinds of expressions
    that are commonly used to represent the relation between temperature and the viscosity or vapor pressure of liquids

    In principle it could be used to model any temperature dependant property 
    that intrinsically cannot assume a negative value and follows a logarithmic mixing law.
    */
    struct LiquidVaporPressureTemperatureRelation
    {
        using NaturalLogarithm = analytic::NaturalLogarithm<double>;
        using ClampedNaturalLogarithm = analytic::Clamped<double,NaturalLogarithm>;
        using ScaledComplementNaturalLogarithm = analytic::ScaledComplement<double,analytic::NaturalLogarithm<double>>;
        using ClampedScaledComplementNaturalLogarithm = analytic::Clamped<double,ScaledComplementNaturalLogarithm>;

        analytic::Railyard<double,analytic::Polynomial<double,-1,6>> exponentiated_terms;
        analytic::Sum<double,ClampedNaturalLogarithm> exponentiated_logarithms;
        analytic::Railyard<double, analytic::ScaledComplement<double, analytic::Polynomial<double,-1,6>>> reduced_exponentiated_terms;
        analytic::Sum<double,analytic::Clamped<double,ScaledComplementNaturalLogarithm>> reduced_exponentiated_logarithms;

        si::temperature<double> Tunits;
        si::pressure<double>    yunits;

        double known_max_fractional_error;

        // zero constructor
        LiquidVaporPressureTemperatureRelation():
            exponentiated_terms(),
            exponentiated_logarithms(),
            reduced_exponentiated_terms(),
            reduced_exponentiated_logarithms(),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        // constant constructor
        LiquidVaporPressureTemperatureRelation(const si::pressure<double> k):
            exponentiated_terms(std::log(k/si::pressure<double>(1.0))),
            exponentiated_logarithms(),
            reduced_exponentiated_terms(),
            reduced_exponentiated_logarithms(),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        LiquidVaporPressureTemperatureRelation(
            const analytic::Railyard<double,analytic::Polynomial<double,-1,6>> exponentiated_terms,
            const analytic::Sum<double,ClampedNaturalLogarithm> exponentiated_logarithms,

            const si::temperature<double> Tunits,
            const si::pressure<double> yunits,

            const double known_max_fractional_error
        ):
            exponentiated_terms(exponentiated_terms),
            exponentiated_logarithms(exponentiated_logarithms),
            reduced_exponentiated_terms(),
            reduced_exponentiated_logarithms(),

            Tunits(Tunits),
            yunits(yunits),

            known_max_fractional_error(known_max_fractional_error)
        {
        }

        LiquidVaporPressureTemperatureRelation(
            const analytic::Railyard<double, analytic::ScaledComplement<double, analytic::Polynomial<double,-1,6>>> reduced_exponentiated_terms,
            const analytic::Sum<double,ClampedScaledComplementNaturalLogarithm> reduced_exponentiated_logarithms,

            const si::temperature<double> Tunits,
            const si::pressure<double> yunits,

            const double known_max_fractional_error
        ):
            exponentiated_terms(),
            exponentiated_logarithms(),
            reduced_exponentiated_terms(reduced_exponentiated_terms),
            reduced_exponentiated_logarithms(reduced_exponentiated_logarithms),

            Tunits(Tunits),
            yunits(yunits),

            known_max_fractional_error(known_max_fractional_error)
        {
        }

        template<int Qlo, int Qhi>
        LiquidVaporPressureTemperatureRelation(
            const ExponentiatedPolynomialRailyardRelation<si::temperature<double>, si::pressure<double>, Qlo,Qhi>& relation
        ):
            exponentiated_terms(relation.yard),
            exponentiated_logarithms(),
            reduced_exponentiated_terms(),
            reduced_exponentiated_logarithms(),

            Tunits(relation.xunits),
            yunits(relation.yunits),

            known_max_fractional_error(0.0)
        {
        }

        LiquidVaporPressureTemperatureRelation& operator=(const LiquidVaporPressureTemperatureRelation other)
        {
            exponentiated_terms = other.exponentiated_terms;
            exponentiated_logarithms = other.exponentiated_logarithms;
            reduced_exponentiated_terms = other.reduced_exponentiated_terms;
            reduced_exponentiated_logarithms = other.reduced_exponentiated_logarithms;

            Tunits = other.Tunits;
            yunits = other.yunits;

            known_max_fractional_error = other.known_max_fractional_error;

            return *this;
        }

        si::pressure<double> operator()(const si::temperature<double> temperature) const
        {
            const double T = double(temperature/Tunits);
            return std::exp(
                exponentiated_terms(T) + exponentiated_logarithms(T) + 
                reduced_exponentiated_terms(T) + reduced_exponentiated_logarithms(T)) * yunits;
        }

        LiquidVaporPressureTemperatureRelation& operator*=(const LiquidVaporPressureTemperatureRelation& other)
        {
            exponentiated_terms += other.exponentiated_terms;
            exponentiated_logarithms += other.exponentiated_logarithms;
            reduced_exponentiated_terms += other.reduced_exponentiated_terms;
            reduced_exponentiated_logarithms += other.reduced_exponentiated_logarithms;
            return *this;
        }

        LiquidVaporPressureTemperatureRelation& operator/=(const LiquidVaporPressureTemperatureRelation& other)
        {
            exponentiated_terms -= other.exponentiated_terms;
            exponentiated_logarithms -= other.exponentiated_logarithms;
            reduced_exponentiated_terms -= other.reduced_exponentiated_terms;
            reduced_exponentiated_logarithms -= other.reduced_exponentiated_logarithms;
            return *this;
        }

        LiquidVaporPressureTemperatureRelation& operator*=(const double scalar)
        {
            reduced_exponentiated_terms += std::log(scalar);
            return *this;
        }

        LiquidVaporPressureTemperatureRelation operator/=(const double scalar)
        {
            reduced_exponentiated_terms -= std::log(scalar);
            return *this;
        }

    };

    LiquidVaporPressureTemperatureRelation operator*(const LiquidVaporPressureTemperatureRelation relation, const LiquidVaporPressureTemperatureRelation other)
    {
        LiquidVaporPressureTemperatureRelation result = relation;
        result *= other;
        return result;
    }

    LiquidVaporPressureTemperatureRelation operator/(const LiquidVaporPressureTemperatureRelation relation, const LiquidVaporPressureTemperatureRelation other)
    {
        LiquidVaporPressureTemperatureRelation result = relation;
        result /= other;
        return result;
    }

    LiquidVaporPressureTemperatureRelation operator*(const LiquidVaporPressureTemperatureRelation relation, const double scalar)
    {
        LiquidVaporPressureTemperatureRelation result = relation;
        result *= scalar;
        return result;
    }

    LiquidVaporPressureTemperatureRelation operator/(const LiquidVaporPressureTemperatureRelation relation, const double scalar)
    {
        LiquidVaporPressureTemperatureRelation result = relation;
        result /= scalar;
        return result;
    }

    LiquidVaporPressureTemperatureRelation operator*(const double scalar, const LiquidVaporPressureTemperatureRelation relation)
    {
        LiquidVaporPressureTemperatureRelation result = relation;
        result *= scalar;
        return result;
    }

    LiquidVaporPressureTemperatureRelation operator/(const double scalar, const LiquidVaporPressureTemperatureRelation relation)
    {
        return LiquidVaporPressureTemperatureRelation(scalar) / relation;
    }


    LiquidVaporPressureTemperatureRelation logarithmic_mix(const std::vector<LiquidVaporPressureTemperatureRelation>& relations, const std::vector<double>& ratios)
    {
        LiquidVaporPressureTemperatureRelation result;
        for (std::size_t i=0; i<relations.size(); i++){
            const double Tscale = double(relations[i].Tunits / result.Tunits);
            const double yscale = double(relations[i].yunits / result.yunits);
            result.exponentiated_terms              += yscale * ratios[i] * compose(relations[i].exponentiated_terms, analytic::Scaling(Tscale));
            result.exponentiated_logarithms         += yscale * ratios[i] * compose(relations[i].exponentiated_logarithms, analytic::Scaling(Tscale));
            result.reduced_exponentiated_terms      += yscale * ratios[i] * compose(relations[i].reduced_exponentiated_terms, analytic::Scaling(Tscale));
            result.reduced_exponentiated_logarithms += yscale * ratios[i] * compose(relations[i].reduced_exponentiated_logarithms, analytic::Scaling(Tscale));
        }
        return result;
    }


    // 42 uses, for viscosity and vapor pressures of liquids
    LiquidVaporPressureTemperatureRelation get_dippr_vapor_pressure_temperature_relation_101(
        const si::temperature<double> Tunits, const si::pressure<double> yunits,
        const double log_intercept, const double log_slope, const double log_log, const double log_exponentiated, const int exponent,
        const double Tmin, const double Tmax
    ){
        using P = analytic::Polynomial<double,-1,6>;
        using CP = analytic::Clamped<double,P>;
        using L = analytic::NaturalLogarithm<double>;
        using CL = analytic::Clamped<double,L>;
        P p(log_intercept);
        p[-1] = log_slope;
        p[exponent] += log_exponentiated;
        return relation::LiquidVaporPressureTemperatureRelation(
            analytic::Railyard<double,P>(CP(Tmin, Tmax, p)), 
            analytic::Sum<double,CL>(CL(Tmin, Tmax, L(log_log, 1.0))), 
            Tunits, yunits, 0.0);
    }

    // Lee Kesler method: https://en.wikipedia.org/wiki/Lee%E2%80%93Kesler_method
    LiquidVaporPressureTemperatureRelation estimate_vapor_pressure_as_liquid_from_lee_kesler(
        const double acentric_factor, 
        const si::temperature<double> critical_temperature, 
        const si::pressure<double> critical_pressure,
        const si::temperature<double> boiling_point_sample_temperature
    ){
        using P = analytic::Polynomial<double,-1,6>;
        using SP = analytic::ScaledComplement<double,P>;
        using CSP = analytic::Clamped<double,SP>;
        using L = analytic::NaturalLogarithm<double>;
        using SL = analytic::ScaledComplement<double,L>;
        using CSL = analytic::Clamped<double,SL>;
        double pc = critical_pressure/si::pascal;
        double Tc = critical_temperature / si::kelvin;
        double Tb = boiling_point_sample_temperature / si::kelvin;
        double Tmax = std::min(Tb, 0.999*Tc); // Tmax must be at most a fraction of Tc so that nans are not generated from calculating log(0)
        double w = acentric_factor;
        analytic::Identity<double> Tr;
        auto f0linear = 5.92714 - 6.09648/Tr + 0.169347*Tr*Tr*Tr*Tr*Tr*Tr;
        auto f1linear = 15.2518 - 15.6875/Tr + 0.43577 *Tr*Tr*Tr*Tr*Tr*Tr;
        auto f0log = -1.28862*analytic::log(Tr);
        auto f1log = -13.4721*analytic::log(Tr);
        P flinear = std::log(pc) + w * f1linear + f0linear;
        return LiquidVaporPressureTemperatureRelation(
            analytic::Railyard<double,SP>(
                CSP(0.0, Tmax, SP(Tc, flinear))
            ), 
            analytic::Sum<double,CSL>({
                CSL(0.0, Tmax, SL(Tc, w*f1log)),
                CSL(0.0, Tmax, SL(Tc,   f0log))
            }),
            si::kelvin, si::pascal, 0.0); 
    }

}}

