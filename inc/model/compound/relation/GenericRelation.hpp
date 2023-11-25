#pragma once

// std libraries
#include <cmath>

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/rails/PolynomialRailyard_library.hpp>
#include <unit/si.hpp>
#include <model/compound/relation/PolynomialRailyardRelation.hpp>
#include <model/compound/relation/ExponentiatedPolynomialRailyardRelation.hpp>
#include <model/compound/correlation/published.hpp>

namespace compound {
namespace relation {

    /*
    `GenericRelation` represents an arbitrary relation 
    that uses an arbitrary anonymous function to map quantities from one unit to another
    This class is incapable of introspection and is expected to have poor data locality, 
    however it is useful when representing relations with high fidelity when those relations resist other attempts of description.
    It is expected that instances of this relation will be mapped to lower fidelity approximations before use,
    such as PolynomialRailyardRelation or (if implemented) some kind of GriddedSplineRelation.
    */
    template<typename Tx, typename Ty>
    struct GenericRelation
    {
        std::function<Ty(Tx)> f;
        
        using value_type = Ty;

        // zero constructor
        constexpr GenericRelation():
            f([](Tx x){ return Ty(0); })
        {
        }

        // constant constructor
        constexpr GenericRelation(const Ty& constant):
            f([constant](Tx x){ return constant; })
        {
        }

        // copy constructor
        constexpr GenericRelation(
            const GenericRelation<Tx,Ty>& other
        ):
            f(other.f)
        {
        }

        constexpr GenericRelation(
            const std::function<Ty(Tx)> f
        ):
            f(f)
        {
        }

        template<typename F>
        constexpr GenericRelation(
            const F& other
        ):
            f([other](Tx x){return other(x);})
        {
        }

        constexpr GenericRelation<Tx,Ty>& operator=(const GenericRelation<Tx,Ty>& other)
        {
            f = other.f;
            return *this;
        }

        template<typename F>
        constexpr GenericRelation<Tx,Ty>& operator=(const F& other)
        {
            f = [other](Tx x){ return other(x); };
            return *this;
        }

        constexpr GenericRelation<Tx,Ty>& operator=(const Ty& other)
        {
            f = [other](Tx x){return Ty(other); };
            return *this;
        }

        Ty operator()(const Tx x) const
        {
            return f(x);
        }

        // GenericRelation<Tx,Ty> restriction(
        //     const si::temperature xlo, const si::temperature xhi,
        //     const double known_max_fractional_error
        // ) const
        // {
        //     return GenericRelation<Tx,Ty>(restriction(yard, xlo/xunits, xhi/xunits), xunits, yunits);
        // }

        GenericRelation<Tx,Ty> operator+=(const double scalar)
        {
            f = [=](Tx x){return f(x)+scalar;};
            return *this;
        }

        GenericRelation<Tx,Ty> operator-=(const double scalar)
        {
            f = [=](Tx x){return f(x)-scalar;};
            return *this;
        }

        GenericRelation<Tx,Ty> operator*=(const double scalar)
        {
            f = [=](Tx x){return f(x)*scalar;};
            return *this;
        }

        GenericRelation<Tx,Ty> operator/=(const double scalar)
        {
            f = [=](Tx x){return f(x)/scalar;};
            return *this;
        }

        template<typename F>
        GenericRelation<Tx,Ty>& operator+=(const F relation)
        {
            f = [=](Tx x){return f(x)+relation(x);};
            return *this;
        }

        template<typename F>
        GenericRelation<Tx,Ty>& operator-=(const F relation)
        {
            f = [=](Tx x){return f(x)-relation(x);};
            return *this;
        }

    };

    template<typename Tx, typename Ty>
    GenericRelation<Tx,Ty> operator-(const GenericRelation<Tx,Ty>& relation)
    {
        GenericRelation<Tx,Ty> result = relation;
        result *= -1.0;
        return result;
    }

    template<typename Tx, typename Ty>
    GenericRelation<Tx,Ty> operator+(const GenericRelation<Tx,Ty>& relation, const Ty value)
    {
        GenericRelation<Tx,Ty> result = relation;
        result += value;
        return result;
    }

    template<typename Tx, typename Ty>
    GenericRelation<Tx,Ty> operator+(const Ty value, const GenericRelation<Tx,Ty>& relation)
    {
        GenericRelation<Tx,Ty> result = relation;
        result += value;
        return result;
    }

    template<typename Tx, typename Ty>
    GenericRelation<Tx,Ty> operator-(const GenericRelation<Tx,Ty>& relation, const Ty value)
    {
        GenericRelation<Tx,Ty> result = relation;
        result -= value;
        return result;
    }

    template<typename Tx, typename Ty>
    GenericRelation<Tx,Ty> operator-(const Ty value, const GenericRelation<Tx,Ty>& relation)
    {
        GenericRelation<Tx,Ty> result = relation;
        result *= -1.0;
        result += value;
        return result;
    }




    // 21 uses, for viscosity of liquids
    template<typename Tx>
    GenericRelation<si::temperature<double>, si::dynamic_viscosity<double>> get_dippr_liquid_dynamic_viscosity_temperature_relation_101(
        const Tx Tunits, const si::dynamic_viscosity<double> yunits,
        const double log_intercept, const double log_slope, const double log_log, const double log_exponentiated, const int exponent,
        const double xmin, const double xmax
    ){
        return GenericRelation<si::temperature<double>, si::dynamic_viscosity<double>>(
            [=](const si::temperature<double> x)
            {
                double t = std::clamp(x/Tunits, xmin, xmax);
                return std::exp(log_intercept + log_slope/t + log_log*std::log(t) + log_exponentiated*std::pow(t,exponent))*yunits;
            });
    }

    // 1 use, for viscosity of liquids
    template<typename Ty>
    GenericRelation<si::temperature<double>, si::dynamic_viscosity<double>> get_exponential_linear_dynamic_viscosity_temperature_relation(
        const si::temperature<double> Tunits, const Ty yunits,
        const double x0, const double x1,
        const double y0, const double y1
    ){
        std::vector<double> xs    {x0,           x1          };
        std::vector<double> logys {std::log(y0), std::log(y1)};
        return GenericRelation<si::temperature<double>, si::dynamic_viscosity<double>>(
            relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,0,1>(
                math::spline::linear_spline<float>(xs, logys), Tunits, yunits));
    }

    // Letsou-Stiel method: https://chemicals.readthedocs.io/chemicals.viscosity.html?highlight=letsou%20stiel#chemicals.viscosity.Letsou_Stiel
    GenericRelation<si::temperature<double>, si::dynamic_viscosity<double>> estimate_viscosity_as_liquid_from_letsou_stiel(
        const double acentric_factor,
        const si::molar_mass<double> molar_mass, 
        const si::temperature<double> critical_temperature, 
        const si::pressure<double> critical_pressure
    ){
        return GenericRelation<si::temperature<double>, si::dynamic_viscosity<double>>(
            [=](si::temperature<double> temperature){
                return correlation::estimate_viscosity_as_liquid_from_letsou_stiel(
                    acentric_factor,
                    molar_mass,  
                    temperature, 
                    critical_temperature, 
                    critical_pressure
                );
            }
        );
    }


    // 5 uses, for heat capacities of liquids
    template<typename Tx>
    GenericRelation<si::temperature<double>, si::specific_heat_capacity<double>> get_dippr_liquid_heat_capacity_temperature_function_114( 
        const Tx Tc, const si::specific_heat_capacity<double> yunits,
        const double c1, const double c2, const double c3, const double c4, const double c5,
        const Tx xmin, Tx xmax
    ){
        return GenericRelation<si::temperature<double>, si::specific_heat_capacity<double>>(
            [=](const si::temperature<double> x)
            {
                double Tr = std::clamp(x, xmin, xmax)/Tc;
                double tau = 1.0-Tr;
                double tau2 = tau*tau;
                double tau3 = tau2*tau;
                double tau4 = tau3*tau;
                double tau5 = tau4*tau;
                return (c1*c1/tau + c2 - 2.0*c1*c3*tau - c1*c4*tau2 - c3*c3*tau3/3.0 - c3*c4*tau4/2.0 - c4*c4*tau5/5.0)*yunits;
            }
        );
    }


    // 21 uses, for vapor pressures of liquids
    template<typename Tx>
    GenericRelation<si::temperature<double>, si::pressure<double>> get_dippr_liquid_vapor_pressure_temperature_relation_101(
        const Tx Tunits, const si::pressure<double> yunits,
        const double log_intercept, const double log_slope, const double log_log, const double log_exponentiated, const int exponent,
        const double xmin, const double xmax
    ){
        return GenericRelation<si::temperature<double>, si::pressure<double>>(
            [=](const si::temperature<double> x)
            {
                double t = std::clamp(x/Tunits, xmin, xmax);
                return std::exp(log_intercept + log_slope/t + log_log*std::log(t) + log_exponentiated*std::pow(t,exponent))*yunits;
            }
        );
    }

    // 175 uses
    GenericRelation<si::temperature<double>, si::pressure<double>> get_vapor_pressure_exponential_interpolated_temperature_function(
        const si::temperature<double> Tunits, const si::pressure<double> yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys,
        const si::temperature<double> xmin,
        const si::temperature<double> xmax
    ){
        assert(xs.size() == ys.size());
        std::vector<double> logys;
        for (std::size_t i=0; i<xs.size(); i++){
            logys.push_back(std::log(ys[i]));
        }
        return GenericRelation<si::temperature<double>, si::pressure<double>>(
                relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,si::pressure<double>,0,1>(
                    math::spline::linear_spline<float>(xs, logys), Tunits, yunits));
    }

    // 175 uses
    template<typename Ty>
    GenericRelation<si::temperature<double>, si::pressure<double>> get_vapor_pressure_exponential_interpolated_temperature_function(
        const si::celcius_type<double> Tunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys,
        const si::temperature<double> xmin,
        const si::temperature<double> xmax
    ){
        assert(xs.size() == ys.size());
        std::vector<double> kelvins;
        std::vector<double> logys;
        for (std::size_t i=0; i<xs.size(); i++){
            kelvins.push_back(xs[i]*Tunits/si::kelvin);
            logys.push_back(std::log(ys[i]));
        }
        return GenericRelation<si::temperature<double>, si::pressure<double>>(relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>,Ty,0,1>(
                    math::spline::linear_spline<float>(kelvins, logys), si::kelvin, yunits));
    }

    // Lee Kesler method: https://en.wikipedia.org/wiki/Lee%E2%80%93Kesler_method
    GenericRelation<si::temperature<double>, si::pressure<double>> estimate_vapor_pressure_as_liquid_from_lee_kesler(
        const double acentric_factor, 
        const si::temperature<double> critical_temperature, 
        const si::pressure<double> critical_pressure
    ){
        return GenericRelation<si::temperature<double>, si::pressure<double>>(
            [=](const si::temperature<double> temperature){
                return correlation::estimate_vapor_pressure_as_liquid_from_lee_kesler(
                    acentric_factor, 
                    temperature, 
                    critical_temperature, 
                    critical_pressure
                );
            });
    }

    // 3 uses, for liquid surface tension
    template<typename Ty>
    GenericRelation<si::temperature<double>,Ty> get_linear_interpolation_anonymous_temperature_relation( 
        const si::temperature<double> Tunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys,
        const double xmin,
        const double xmax
    ){
        return GenericRelation<si::temperature<double>, si::surface_energy<double>>(
                PolynomialRailyardRelation<si::temperature<double>,Ty,0,1>(math::spline::linear_spline<double>(xs, ys), Tunits, yunits));
    }

    // 3 uses, for liquid surface tension
    template<typename Ty>
    GenericRelation<si::temperature<double>,Ty> get_linear_interpolation_anonymous_temperature_relation( 
        const si::celcius_type<double> Tunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys,
        const double xmin,
        const double xmax
    ){
        const auto f = math::spline::linear_spline<double>(xs, ys);
        return GenericRelation<si::temperature<double>, si::surface_energy<double>>(
                [=](const si::temperature<double> x) {return f(x/Tunits)*yunits;});
    }

    // 15 uses, for liquid surface tension
    GenericRelation<si::temperature<double>, si::surface_energy<double>> get_refprop_liquid_surface_tension_temperature_relation( 
        const si::temperature<double> Tunits, const si::surface_energy<double> yunits,
        const double Tc, const double sigma0, const double n0, const double sigma1, const double n1, const double sigma2, const double n2,
        const double xmin, const double xmax
    ){
        return GenericRelation<si::temperature<double>, si::surface_energy<double>>(
            [=]
            (const si::temperature<double> x)
            {
                double Tr = std::clamp(x/Tunits, xmin, xmax)/Tc;
                return ( sigma0*std::pow(1.0 - Tr, n0) + sigma1*std::pow(1.0 - Tr, n1) + sigma2*std::pow(1.0 - Tr, n2) )*yunits;
            });
    }
    // from Mulero (2012)

    // 3 uses, for liquid surface tension
    template<typename Tx>
    GenericRelation<si::temperature<double>, si::surface_energy<double>> get_linear_liquid_surface_tension_temperature_function(
        const Tx Tunits, const si::surface_energy<double> yunits,
        const double TL, const double gammaTL, const double dgamma_dT,
        const double xmin, const double xmax
    ){
        return GenericRelation<si::temperature<double>, si::surface_energy<double>>(
            [=]
            (const si::temperature<double> x)
            {
                double t = x/Tunits;
                return ( gammaTL + dgamma_dT * (t-TL) )*yunits;
            });
    }

    GenericRelation<si::wavenumber<double>,si::attenuation<double>> get_generic_spectral_function_from_reflectance_at_wavelengths(
        const si::length<double> lunits,
        const si::length<double> particle_diameter, 
        const std::vector<double>wavelengths, 
        const std::vector<double>reflectances
    ){
        /*
        NOTE: we assume extinction efficiency is close to 1, in which case the 
          single scattering albedo equals scattering efficiency
        In this case:
          absorption_coefficient = approx_single_scatter_albedo_from_reflectance ∘ approx_single_scatter_albedo_from_reflectance
        This is equivalent to:
          absorption_coefficient = max((reflectance + 1.0)*(reflectance + 1.0) / (4.0 * reflectance) - 1.0, 0.0) / De;
        where:
          De = 2*particle_diameter;
        We leverage the fact that 0 ≤ reflectance ≤ 1 to create a segmented 
          linear approximation of 1/reflectance that is suited for that range.
          It has an maximum error of 12% over the targeted range.
        We use a linear approximation since the result will be composed 
          with another segmented linear approximation for reflectance,
          and we don't want the degree of the resulting polynomial 
          to be so high that it produces nans upon evaluation.
        */
        auto reflectance_relation = math::spline::linear_spline<float>(wavelengths, reflectances);
        return GenericRelation<si::wavenumber<double>, si::attenuation<double>>(
            [=](si::wavenumber<double> n){
                auto R = reflectance_relation((1.0/n)/lunits);
                return std::max((R+1.0)*(R+1.0) / (4.0*R) - 1.0, 0.0) / (2.0*particle_diameter);
            });
    }

    GenericRelation<si::wavenumber<double>,si::attenuation<double>> get_generic_spectral_cubic_interpolation_of_wavenumber_for_log10_sample_output(
        const si::wavenumber<double> nunits, const si::attenuation<double> yunits,
        const std::vector<double>      ns, 
        const std::vector<double> log10ys
    ){
        assert(ns.size() == log10ys.size());
        std::vector<double> ns2(ns);
        std::vector<double> ys;
        for (std::size_t i=0; i<log10ys.size(); i++){
            ys.push_back(pow(10.0, log10ys[i]));
        }
        std::reverse(ns2.begin(), ns2.end());
        std::reverse(ys.begin(), ys.end());
        return GenericRelation<si::wavenumber<double>, si::attenuation<double>>(
            PolynomialRailyardRelation<si::wavenumber<double>,si::attenuation<double>,0,3> (
                math::spline::linear_spline<double>(ns2, ys), nunits, yunits));
    }

    template<typename Ty>
    GenericRelation<si::wavenumber<double>,Ty> get_generic_spectral_linear_interpolation_function_of_wavelength(
        const si::length<double> lunits, const Ty yunits,
        const std::vector<double> ls, 
        const std::vector<double> ys
    ){
        assert(ls.size() == ys.size());
        auto relation = math::spline::linear_spline<double>(ls, ys);
        return GenericRelation<si::wavenumber<double>,Ty>(
            [=](si::wavenumber<double> n){ 
                return relation((1.0/n)/lunits)*yunits;
            });
    }

    template<typename Ty>
    GenericRelation<si::wavenumber<double>,Ty> get_generic_spectral_linear_interpolation_function_of_wavenumber(
        const si::wavenumber<double> nunits, const Ty yunits,
        const std::vector<double> ns, 
        const std::vector<double> ys
    ){
        assert(ns.size() == ys.size());
        auto relation = math::spline::linear_spline<double>(ns, ys);
        return GenericRelation<si::wavenumber<double>,Ty>(
            [=](si::wavenumber<double> n){ 
                return relation(n/nunits)*yunits;
            });
    }






}}

