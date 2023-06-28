#pragma once

// std libraries
#include <cmath>

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/rails/PolynomialRailyard_library.hpp>
#include <math/analytic/Exponentiated.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/rails/PolynomialRailyard_library.hpp>
#include <units/si.hpp>
#include <models/compound/property/published.hpp>

namespace compound {
namespace relation {

    /*
    `AnonymousTemperatureRelation` represents an arbitrary relation 
    that uses an arbitrary anonymous function to map quantities from one unit to another
    This class is incapable of introspection and is expected to have poor data locality, 
    however it is useful when representing relations with high fidelity when those relations resist other attempts of description.
    It is expected that instances of this relation will be mapped to lower fidelity approximations before use,
    such as PolynomialRailyardRelation or (if implemented) some kind of GriddedSplineRelation.
    */
    template<typename Ty>
    struct AnonymousTemperatureRelation
    {
        std::function<Ty(si::temperature<double>)> f;
        si::temperature<double> Tmin;
        si::temperature<double> Tmax;
        
        using value_type = Ty;

        // zero constructor
        constexpr AnonymousTemperatureRelation():
            f([](si::temperature<double> T){ return Ty(0); }),
            Tmin(0*si::kelvin),
            Tmax(std::numeric_limits<double>::max())
        {
        }

        // copy constructor
        template<int Qlo, int Qhi>
        constexpr AnonymousTemperatureRelation(
            const AnonymousTemperatureRelation<Ty>& other
        ):
            f(other.f),
            Tmin(other.Tmin),
            Tmax(other.Tmax)
        {
        }

        constexpr AnonymousTemperatureRelation(
            const std::function<Ty(si::temperature<double>)> f, 
            const si::temperature<double> Tmin, 
            const si::temperature<double> Tmax
        ):
            f(f),
            Tmin(Tmin),
            Tmax(Tmax)
        {
        }

        constexpr AnonymousTemperatureRelation(const Ty& other):
            f([other](si::temperature<double> T){return Ty(other);}),
            Tmin(0),
            Tmax(std::numeric_limits<double>::max())
        {
        }

        template<typename F>
        constexpr AnonymousTemperatureRelation(
            const F& other, 
            const si::temperature<double> Tmin, 
            const si::temperature<double> Tmax
        ):
            f([other](si::temperature<double> T){return other(T);}),
            Tmin(Tmin),
            Tmax(Tmax)
        {
        }

        constexpr AnonymousTemperatureRelation<Ty>& operator=(const AnonymousTemperatureRelation<Ty>& other)
        {
            f = other.f;
            Tmin = other.Tmin;
            Tmax = other.Tmax;
            return *this;
        }

        template<typename F>
        constexpr AnonymousTemperatureRelation<Ty>& operator=(const F& other)
        {
            f = [other](si::temperature<double> T){ return other(T); };
            Tmin = other.Tmin;
            Tmax = other.Tmax;
            return *this;
        }

        constexpr AnonymousTemperatureRelation<Ty>& operator=(const Ty& other)
        {
            f = [other](si::temperature<double> T){return Ty(other); };
            Tmin = 0*si::kelvin;
            Tmax = std::numeric_limits<double>::max();
            return *this;
        }

        Ty operator()(const si::temperature<double> T) const
        {
            return f(T);
        }

        // AnonymousTemperatureRelation<Ty> restriction(
        //     const si::temperature xlo, const si::temperature xhi,
        //     const double known_max_fractional_error
        // ) const
        // {
        //     return AnonymousTemperatureRelation<Ty>(restriction(yard, xlo/xunits, xhi/xunits), xunits, yunits);
        // }

        AnonymousTemperatureRelation<Ty> operator+=(const double scalar)
        {
            f = [=](si::temperature<double> T){return f(T)+scalar;};
            return *this;
        }

        AnonymousTemperatureRelation<Ty> operator-=(const double scalar)
        {
            f = [=](si::temperature<double> T){return f(T)-scalar;};
            return *this;
        }

        AnonymousTemperatureRelation<Ty> operator*=(const double scalar)
        {
            f = [=](si::temperature<double> T){return f(T)*scalar;};
            return *this;
        }

        AnonymousTemperatureRelation<Ty> operator/=(const double scalar)
        {
            f = [=](si::temperature<double> T){return f(T)/scalar;};
            return *this;
        }

        template<typename F>
        AnonymousTemperatureRelation<Ty>& operator+=(const F relation)
        {
            f = [=](si::temperature<double> T){return f(T)+relation(T);};
            Tmin = si::max(Tmin, relation.Tmin);
            Tmax = si::min(Tmax, relation.Tmax);
            return *this;
        }

        template<typename F>
        AnonymousTemperatureRelation<Ty>& operator-=(const F relation)
        {
            f = [=](si::temperature<double> T){return f(T)-relation(T);};
            Tmin = si::max(Tmin, relation.Tmin);
            Tmax = si::min(Tmax, relation.Tmax);
            return *this;
        }

    };

    template<typename Ty>
    AnonymousTemperatureRelation<Ty> operator-(const AnonymousTemperatureRelation<Ty>& relation)
    {
        AnonymousTemperatureRelation<Ty> result = relation;
        result *= -1.0;
        return result;
    }

    template<typename Ty>
    AnonymousTemperatureRelation<Ty> operator+(const AnonymousTemperatureRelation<Ty>& relation, const Ty value)
    {
        AnonymousTemperatureRelation<Ty> result = relation;
        result += value;
        return result;
    }

    template<typename Ty>
    AnonymousTemperatureRelation<Ty> operator+(const Ty value, const AnonymousTemperatureRelation<Ty>& relation)
    {
        AnonymousTemperatureRelation<Ty> result = relation;
        result += value;
        return result;
    }

    template<typename Ty>
    AnonymousTemperatureRelation<Ty> operator-(const AnonymousTemperatureRelation<Ty>& relation, const Ty value)
    {
        AnonymousTemperatureRelation<Ty> result = relation;
        result -= value;
        return result;
    }

    template<typename Ty>
    AnonymousTemperatureRelation<Ty> operator-(const Ty value, const AnonymousTemperatureRelation<Ty>& relation)
    {
        AnonymousTemperatureRelation<Ty> result = relation;
        result *= -1.0;
        result += value;
        return result;
    }


    // template<typename Ty>
    // AnonymousTemperatureRelation<Ty> exp(const AnonymousTemperatureRelation<Ty>& relation)
    // {
    //     return AnonymousTemperatureRelation<Ty>([relation](si::temperature<double> T) {return si::exp(relation(T));});
    // }

    // template<typename Ty>
    // AnonymousTemperatureRelation<Ty> exp(const AnonymousTemperatureRelation<Ty>& relation)
    // {
    //     return AnonymousTemperatureRelation<Ty>([relation](si::temperature<double> T) {return std::exp(relation(T));});
    // }

    // template<typename Ty>
    // AnonymousTemperatureRelation<Ty> ln(const AnonymousTemperatureRelation<Ty>& relation)
    // {
    //     return AnonymousTemperatureRelation<Ty>([relation](si::temperature<double> T) {return si::ln(relation(T));});
    // }

    // template<typename Ty>
    // AnonymousTemperatureRelation<Ty> ln(const AnonymousTemperatureRelation<Ty>& relation)
    // {
    //     return AnonymousTemperatureRelation<Ty>([relation](si::temperature<double> T) {return std::ln(relation(T));});
    // }

    // template<typename Ty>
    // AnonymousTemperatureRelation<Ty> abs(const AnonymousTemperatureRelation<Ty>& relation)
    // {
    //     return AnonymousTemperatureRelation<Ty>([relation](si::temperature<double> T) {return si::abs(relation(T));});
    // }

    // template<typename Ty>
    // AnonymousTemperatureRelation<Ty> abs(const AnonymousTemperatureRelation<Ty>& relation)
    // {
    //     return AnonymousTemperatureRelation<Ty>([relation](si::temperature<double> T) {return std::abs(relation(T));});
    // }



    template<typename Ty, typename F>
    auto operator*(const AnonymousTemperatureRelation<Ty>& relation, const F other)
    {
        AnonymousTemperatureRelation<si::temperature<double>>([relation, other](si::temperature<double> T){return relation.f(T)*other(T);});
    }

    template<typename Ty, typename F>
    auto operator/(const AnonymousTemperatureRelation<Ty>& relation, const F other)
    {
        AnonymousTemperatureRelation<si::temperature<double>>([relation, other](si::temperature<double> T){return relation.f(T)/other(T);});
    }


    template<typename Ty, typename F>
    auto operator*(const F other, const AnonymousTemperatureRelation<Ty>& relation)
    {
        AnonymousTemperatureRelation<si::temperature<double>>([relation, other](si::temperature<double> T){return relation.f(T)*other(T);});
    }

    template<typename Ty, typename F>
    auto operator/(const F other, const AnonymousTemperatureRelation<Ty>& relation)
    {
        AnonymousTemperatureRelation<si::temperature<double>>([relation, other](si::temperature<double> T){return relation.f(T)/other(T);});
    }




    template<typename Ty>
    AnonymousTemperatureRelation<Ty> logarithmic_mix(const std::vector<AnonymousTemperatureRelation<Ty>>& relations, const std::vector<double>& ratios)
    {
        AnonymousTemperatureRelation<Ty> result;
        for (std::size_t i=0; i<relations.size(); i++){
            result += exp(ratios[i] * ln(relations[i]));
        }
        return result;
    }

    // 21 uses, for viscosity of liquids
    template<typename Tx>
    AnonymousTemperatureRelation<si::dynamic_viscosity<double>> get_dippr_liquid_dynamic_viscosity_temperature_relation_101(
        const Tx Tunits, const si::dynamic_viscosity<double> yunits,
        const double log_intercept, const double log_slope, const double log_log, const double log_exponentiated, const int exponent,
        const double Tmin, const double Tmax
    ){
        return AnonymousTemperatureRelation<si::dynamic_viscosity<double>>(
            [=](const si::temperature<double> T)
            {
                double t = std::clamp(T/Tunits, Tmin, Tmax);
                return std::exp(log_intercept + log_slope/t + log_log*std::log(t) + log_exponentiated*std::pow(t,exponent))*yunits;
            }, Tmin*Tunits, Tmax*Tunits
        );
    }

    // 1 use, for viscosity of liquids
    template<typename Ty>
    AnonymousTemperatureRelation<si::dynamic_viscosity<double>> get_exponential_linear_dynamic_viscosity_temperature_relation(
        const si::temperature<double> Tunits, const Ty yunits,
        const double x0, const double x1,
        const double y0, const double y1
    ){
        std::vector<double> xs    {x0,           x1          };
        std::vector<double> logys {std::log(y0), std::log(y1)};
        return AnonymousTemperatureRelation<si::dynamic_viscosity<double>>(
            relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,0,1>(
                analytic::spline::linear_spline<float>(xs, logys), Tunits, yunits),
            x0*Tunits, x1*Tunits);
    }

    // Letsou-Stiel method: https://chemicals.readthedocs.io/chemicals.viscosity.html?highlight=letsou%20stiel#chemicals.viscosity.Letsou_Stiel
    AnonymousTemperatureRelation<si::dynamic_viscosity<double>> estimate_viscosity_as_liquid_from_letsou_stiel(
        const double acentric_factor,
        const si::molar_mass<double> molar_mass, 
        const si::temperature<double> critical_temperature, 
        const si::pressure<double> critical_pressure
    ){
        return AnonymousTemperatureRelation<si::dynamic_viscosity<double>>(
            [=](si::temperature<double> temperature){
                return property::estimate_viscosity_as_liquid_from_letsou_stiel(
                    acentric_factor,
                    molar_mass,  
                    temperature, 
                    critical_temperature, 
                    critical_pressure
                );
            }, 
            0.76*critical_temperature, 
            0.98*critical_temperature
        );
    }


    // 5 uses, for heat capacities of liquids
    template<typename Tx>
    AnonymousTemperatureRelation<si::specific_heat_capacity<double>> get_dippr_liquid_heat_capacity_temperature_function_114( 
        const Tx Tc, const si::specific_heat_capacity<double> yunits,
        const double c1, const double c2, const double c3, const double c4, const double c5,
        const Tx Tmin, Tx Tmax
    ){
        return AnonymousTemperatureRelation<si::specific_heat_capacity<double>>(
            [Tc, yunits, Tmin, Tmax, c1, c2, c3, c4, c5]
            (const si::pressure<double> p, const si::temperature<double> T)
            {
                double Tr = std::clamp(T, Tmin, Tmax)/Tc;
                double tau = 1.0-Tr;
                double tau2 = tau*tau;
                double tau3 = tau2*tau;
                double tau4 = tau3*tau;
                double tau5 = tau4*tau;
                return (c1*c1/tau + c2 - 2.0*c1*c3*tau - c1*c4*tau2 - c3*c3*tau3/3.0 - c3*c4*tau4/2.0 - c4*c4*tau5/5.0)*yunits;
            }, Tmin, Tmax
        );
    }


    // 21 uses, for vapor pressures of liquids
    template<typename Tx>
    AnonymousTemperatureRelation<si::pressure<double>> get_dippr_liquid_vapor_pressure_temperature_relation_101(
        const Tx Tunits, const si::pressure<double> yunits,
        const double log_intercept, const double log_slope, const double log_log, const double log_exponentiated, const int exponent,
        const double Tmin, const double Tmax
    ){
        return AnonymousTemperatureRelation<si::pressure<double>>(
            [=](const si::temperature<double> T)
            {
                double t = std::clamp(T/Tunits, Tmin, Tmax);
                return std::exp(log_intercept + log_slope/t + log_log*std::log(t) + log_exponentiated*std::pow(t,exponent))*yunits;
            }, Tmin*Tunits, Tmax*Tunits
        );
    }

    // 175 uses
    template<typename Ty>
    AnonymousTemperatureRelation<si::pressure<double>> get_vapor_pressure_exponential_interpolated_temperature_function(
        const si::temperature<double> Tunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys,
        const si::temperature<double> Tmin,
        const si::temperature<double> Tmax
    ){
        assert(xs.size() == ys.size());
        std::vector<double> logys;
        for (std::size_t i=0; i<xs.size(); i++){
            logys.push_back(std::log(ys[i]));
        }
        return AnonymousTemperatureRelation<si::pressure<double>>(
                relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,0,1>(
                    analytic::spline::linear_spline<float>(xs, logys), Tunits, yunits), 
            Tmin, Tmax);
    }

    // 175 uses
    template<typename Ty>
    AnonymousTemperatureRelation<si::pressure<double>> get_vapor_pressure_exponential_interpolated_temperature_function(
        const si::celcius_type<double> Tunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys,
        const si::temperature<double> Tmin,
        const si::temperature<double> Tmax
    ){
        assert(xs.size() == ys.size());
        std::vector<double> kelvins;
        std::vector<double> logys;
        for (std::size_t i=0; i<xs.size(); i++){
            kelvins.push_back(xs[i]*Tunits/si::kelvin);
            logys.push_back(std::log(ys[i]));
        }
        return AnonymousTemperatureRelation<si::pressure<double>>(relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,0,1>(
                    analytic::spline::linear_spline<float>(kelvins, logys), si::kelvin, yunits), 
            Tmin, Tmax);
    }





    // // 15 uses, for liquid surface tension
    // template<typename Tx>
    // AnonymousTemperatureRelation<si::surface_energy<double>> get_refprop_liquid_surface_tension_temperature_function( 
    //     const Tx Tunits, const si::surface_energy<double> yunits,
    //     const double Tc, const double sigma0, const double n0, const double sigma1, const double n1, const double sigma2, const double n2,
    //     const double Tmin, const double Tmax
    // ){
    //     return AnonymousTemperatureRelation<si::surface_energy<double>>(
    //         [Tunits, yunits, Tc, sigma0, n0, sigma1, n1, sigma2, n2, Tmin, Tmax]
    //         (const si::pressure<double> p, const si::temperature<double> T)
    //         {
    //             double Tr = std::clamp(T/Tunits, Tmin, Tmax)/Tc;
    //             return ( sigma0*std::pow(1.0 - Tr, n0) + sigma1*std::pow(1.0 - Tr, n1) + sigma2*std::pow(1.0 - Tr, n2) )*yunits;
    //         }, Tmin*Tunits, Tmax*Tunits
    //     );
    // }
    // // from Mulero (2012)

    // // 3 uses, for liquid surface tension
    // template<typename Tx>
    // AnonymousTemperatureRelation<si::surface_energy<double>> get_linear_liquid_surface_tension_temperature_function( 
    //     const Tx Tunits, const si::surface_energy<double> yunits,
    //     const double TL, const double gammaTL, const double dgamma_dT,
    //     const double Tmin, const double Tmax
    // ){
    //     return AnonymousTemperatureRelation<si::surface_energy<double>>(
    //         [Tunits, yunits, TL, gammaTL, dgamma_dT]
    //         (const si::pressure<double> p, const si::temperature<double> T)
    //         {
    //             double t = T/Tunits;
    //             return ( gammaTL + dgamma_dT * (t-TL) )*yunits;
    //         }, Tmin*Tunits, Tmax*Tunits
    //     );
    // }

    // /*
    // `get_dippr_quartic_temperature_function_100()` is equivalent to dippr function 100,
    // for liquid thermal conductivity, heat capacity, and solid density
    // */
    // template<typename Tx>
    // AnonymousTemperatureRelation<si::surface_energy<double>> get_quadratic_liquid_surface_tension_temperature_function( 
    //     const Tx Tunits, const si::surface_energy<double> yunits,
    //     const double intercept, const double slope, const double square, const double cube, const double fourth,
    //     const double Tmin, double Tmax
    // ){
    //     return AnonymousTemperatureRelation<si::surface_energy<double>>(
    //         [Tunits, yunits, Tmin, Tmax, intercept, slope, square, cube, fourth]
    //         (const si::pressure<double> p, const si::temperature<double> T)
    //         {
    //             double t = std::clamp(T/Tunits, Tmin, Tmax);
    //             return (intercept + slope*t + square*t*t + cube*t*t*t + fourth*t*t*t*t)*yunits;
    //         }, Tmin*Tunits, Tmax*Tunits
    //     );
    // }












}}

