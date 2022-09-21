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

    struct Logarithm {
        double weight;
        constexpr explicit Logarithm(const double weight):
            weight(weight)
        {}
        // zero constructor
        constexpr explicit Logarithm():
            weight(0.0f)
        {}
        constexpr Logarithm(const Logarithm& f):
            weight(f.weight)
        {}
        constexpr double operator()(const double x) const
        {
            return weight*std::log(x);
        }
        constexpr Logarithm& operator*=(const double k)
        {
            weight *= k;
            return *this;
        }
        constexpr Logarithm& operator/=(const double k)
        {
            weight /= k;
            return *this;
        }
    };

    constexpr Logarithm operator*(const Logarithm f, const double k)
    {
        return Logarithm(f.weight*k);
    }
    constexpr Logarithm operator*(const double k, const Logarithm f)
    {
        return Logarithm(f.weight*k);
    }
    constexpr Logarithm operator/(const Logarithm f, const double k)
    {
        return Logarithm(f.weight/k);
    }

    using ClampedLogarithm = math::Clamped<double,Logarithm>;

    /*
    `LiquidPropertyExponentialTemperatureRelation` consolidates many kinds of expressions
    that are commonly used to represent the relation between temperature and the viscosity or vapor pressure of liquids

    In principle it could be used to model any temperature dependant property 
    that intrinsically cannot assume a negative value and follows a logarithmic mixing law.
    */
    template<typename Ty, int Plo, int Phi>
    class LiquidPropertyExponentialTemperatureRelation
    {
        math::PolynomialRailyard<double, Plo, Phi> terms;
        std::vector<ClampedLogarithm> logarithms;

        si::temperature<double> Tunits;
        Ty                      yunits;

    public:
        double known_max_fractional_error;

        // zero constructor
        constexpr LiquidPropertyExponentialTemperatureRelation():
            terms(),
            logarithms(),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        // constant constructor
        constexpr LiquidPropertyExponentialTemperatureRelation(const Ty k):
            terms(std::log(k/Ty(1.0))),
            logarithms(),

            Tunits(1.0),
            yunits(1.0),

            known_max_fractional_error(0.0)
        {
        }

        constexpr LiquidPropertyExponentialTemperatureRelation(
            const math::PolynomialRailyard<double, Plo, Phi> terms,
            const std::vector<ClampedLogarithm> logarithms,

            const si::temperature<double> Tunits,
            const Ty yunits,

            const double known_max_fractional_error
        ):
            terms(terms),
            logarithms(logarithms),

            Tunits(Tunits),
            yunits(yunits),

            known_max_fractional_error(known_max_fractional_error)
        {
        }

        constexpr LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>& operator=(const LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> other)
        {
            terms = other.terms;
            logarithms = other.logarithms;

            Tunits = other.Tunits;
            yunits = other.yunits;

            known_max_fractional_error = other.known_max_fractional_error;

            return *this;
        }

        Ty operator()(const si::temperature<double> temperature) const
        {
            const double T = double(temperature/Tunits);
            double logy = terms(T);
            for (std::size_t i = 0; i < logarithms.size(); ++i)
            {
                logy += logarithms[i](T);
            }
            return std::exp(logy) * yunits;
        }

        LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>& operator*=(const LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>& other)
        {
            terms += other.terms;
            for (std::size_t i = 0; i < other.logarithms.size(); ++i)
            {
                logarithms.push_back(other.logarithms[i]);
            }
            return *this;
        }

        LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>& operator/=(const LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>& other)
        {
            terms -= other.terms;
            for (std::size_t i = 0; i < other.logarithms.size(); ++i)
            {
                logarithms.push_back(-other.logarithms[i]);
            }
            return *this;
        }

        LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>& operator*=(const double scalar)
        {
            terms[0] += std::log(scalar);
            return *this;
        }

        LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> operator/=(const double scalar)
        {
            terms[0] -= std::log(scalar);
            return *this;
        }

    };

    // 42 uses, for viscosity and vapor pressures of liquids
    template<typename Ty, int Plo, int Phi>
    LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> get_dippr_temperature_relation_101(
        const si::temperature<double> Tunits, const Ty yunits,
        const double log_intercept, const double log_slope, 
        const double Tmin, const double Tmax
    ){
        const double oo = std::numeric_limits<double>::max();
        using P = math::Polynomial<double, Plo, Phi>;
        using R = math::PolynomialRailcar<double, Plo, Phi>;
        P p(log_intercept);
        p[-1] = log_slope;
        return relation::LiquidPropertyExponentialTemperatureRelation(
            math::PolynomialRailyard<double, Plo, Phi>({
                R(-oo,  Tmin, P(p(Tmin))),
                R(Tmin, Tmax, p),
                R(Tmax,   oo, P(p(Tmax))),
            }), {}, 
            Tunits, yunits, 0.0f);
    }

    // 42 uses, for viscosity and vapor pressures of liquids
    template<typename Ty, int Plo, int Phi>
    LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> get_dippr_temperature_relation_101(
        const si::temperature<double> Tunits, const Ty yunits,
        const double log_intercept, const double log_slope, const double log_log,
        const double Tmin, const double Tmax
    ){
        const double oo = std::numeric_limits<double>::max();
        using P = math::Polynomial<double, Plo, Phi>;
        using R = math::PolynomialRailcar<double, Plo, Phi>;
        P p(log_intercept);
        p[-1] = log_slope;
        return relation::LiquidPropertyExponentialTemperatureRelation(
            math::PolynomialRailyard<double, Plo, Phi>({
                R(-oo,  Tmin, P(p(Tmin))),
                R(Tmin, Tmax, p),
                R(Tmax,   oo, P(p(Tmax))),
            }), 
            {
                ClampedLogarithm(Tmin, Tmax, Logarithm(log_log))
            }, 
            Tunits, yunits, 0.0f);
    }

    // 42 uses, for viscosity and vapor pressures of liquids
    template<typename Ty, int Plo, int Phi>
    LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> get_dippr_temperature_relation_101(
        const si::temperature<double> Tunits, const Ty yunits,
        const double log_intercept, const double log_slope, const double log_log, const double log_exponentiated, const int exponent,
        const double Tmin, const double Tmax
    ){
        const double oo = std::numeric_limits<double>::max();
        using P = math::Polynomial<double, Plo, Phi>;
        using R = math::PolynomialRailcar<double, Plo, Phi>;
        P p(log_intercept);
        p[-1] = log_slope;
        p[exponent] = log_exponentiated;
        return relation::LiquidPropertyExponentialTemperatureRelation(
            math::PolynomialRailyard<double, Plo, Phi>({
                R(-oo,  Tmin, P(p(Tmin))),
                R(Tmin, Tmax, p),
                R(Tmax,   oo, P(p(Tmax))),
            }), 
            {
                ClampedLogarithm(Tmin, Tmax, Logarithm(log_log))
            }, 
            Tunits, yunits, 0.0f);
    }

    // // 42 uses, for viscosity and vapor pressures of liquids
    // template<typename Tx, typename Ty>
    // relation::StateFunction<Ty> get_dippr_temperature_function_101( 
    //     const Tx Tunits, const Ty yunits,
    //     const double log_intercept, const double log_slope, const double log_log, const double log_exponentiated, const double exponent,
    //     const double Tmin, const double Tmax
    // ){
    //     return relation::StateFunction<Ty>(
    //         [Tunits, yunits, log_intercept, log_slope, log_log, log_exponentiated, exponent, Tmin, Tmax]
    //         (const si::pressure<double> p, const si::temperature<double> T)
    //         {
    //             double t = std::clamp(T/Tunits, Tmin, Tmax);
    //             return std::exp(log_intercept + log_slope/t + log_log*std::log(t) + log_exponentiated*std::pow(t,exponent))*yunits;
    //         }
    //     );
    // }

    // 175 uses
    template<typename Ty, int Plo, int Phi>
    LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> get_exponential_interpolated_temperature_function(
        const si::temperature<double> Tunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        std::vector<double> logys;
        for (std::size_t i=0; i<logys.size(); i++){
            logys.push_back(std::log(ys[i]));
        }
        return relation::LiquidPropertyExponentialTemperatureRelation(
            math::spline::linear_spline<double>(xs, logys), {},
            Tunits, yunits, 0.0f);
    }

    // // 175 uses
    // template<typename Ty, int Plo, int Phi>
    // LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi> get_exponential_interpolated_inverse_temperature_function(
    //     const si::temperature<double> Tunits, const Ty yunits,
    //     const std::vector<double>xs,
    //     const std::vector<double>ys
    // ){
    //     assert(xs.size() == ys.size());
    //     std::vector<double> invxs;
    //     std::vector<double> logys;
    //     for (std::size_t i=0; i<logys.size(); i++){
    //         invxs.push_back(1.0f/xs[i]);
    //         logys.push_back(std::log(ys[i]));
    //     }
    //     return relation::LiquidPropertyExponentialTemperatureRelation<Ty,Plo,Phi>(
    //         math::compose(math::spline::linear_spline<double>(invxs, logys), math::Polynomial<double,-1,-1>), {},
    //         Tunits, yunits, 0.0f);
    // }

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

}}

