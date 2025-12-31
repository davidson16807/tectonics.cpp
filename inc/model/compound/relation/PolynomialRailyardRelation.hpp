#pragma once

// in-house libraries
#include <math/analytic/Scaling.hpp>
#include <math/analytic/rails/PolynomialRailyard.hpp>
#include <math/analytic/rails/PolynomialRailyard_library.hpp>

#include <unit/si.hpp>

#include <relation/PolynomialRailyardRelation.hpp>

namespace compound {
namespace relation {

    template<typename Tx, typename Ty, int Plo, int Phi>
    using PolynomialRailyardRelation = ::relation::PolynomialRailyardRelation<Tx,Ty,Plo,Phi>;

    // TODO: rename `spectral_linear_yard`
    template<typename T, typename Ty>
    relation::PolynomialRailyardRelation<si::temperature<T>,Ty,0,1> get_linear_interpolation_function(
        const si::celsius_type<T> celsius, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        return relation::PolynomialRailyardRelation<si::temperature<T>,Ty,0,1>(
                analytic::compose(
                    analytic::spline::linear_spline<double>(xs, ys), 
                    analytic::Shifting(-si::standard_temperature/si::kelvin)), 
                si::kelvin, yunits);
    }

    // TODO: rename `spectral_linear_yard`
    template<typename T, typename Ty>
    relation::PolynomialRailyardRelation<si::temperature<T>,Ty,0,1> get_linear_interpolation_function(
        const si::temperature<T> xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        return relation::PolynomialRailyardRelation<si::temperature<T>,Ty,0,1>(analytic::spline::linear_spline<double>(xs, ys), xunits, yunits);
    }

    // TODO: rename `spectral_linear_yard`
    template<typename T, typename Ty>
    relation::PolynomialRailyardRelation<si::pressure<T>,Ty,0,1> get_linear_interpolation_function(
        const si::pressure<T> xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        assert(xs.size() == ys.size());
        return relation::PolynomialRailyardRelation<si::pressure<T>,Ty,0,1>(analytic::spline::linear_spline<double>(xs, ys), xunits, yunits);
    }
    
    /*
    `get_dippr_quartic_temperature_function_100()` is equivalent to dippr function 100,
    for liquid thermal conductivity, heat capacity, and solid density
    */
    // 26 uses, for liquid thermal conductivity and heat capacity
    template<typename Ty>
    relation::PolynomialRailyardRelation<si::temperature<double>, Ty, 0,4> get_dippr_quartic_temperature_relation_100( 
        const si::temperature<double> Tunits, const Ty yunits,
        const double intercept, const double slope, const double square, const double cube, const double fourth,
        const double Tmin, double Tmax
    ){
        using P = analytic::Polynomial<double,0,4>;
        using C = analytic::Clamped<double,P>;
        using R = analytic::Railyard<double,P>;
        analytic::Polynomial<double,0,4> p = analytic::Polynomial<double,0,4>({intercept, slope, square, cube, fourth});
        return relation::PolynomialRailyardRelation<si::temperature<double>,Ty,0,4>(R(C(Tmin,Tmax, p)), Tunits, yunits);
    }

    // Sheffy Johnson: https://chemicals.readthedocs.io/chemicals.thermal_conductivity.html#pure-low-pressure-liquid-correlations
    relation::PolynomialRailyardRelation<si::temperature<double>, si::thermal_conductivity<double>,0,1> estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(
        const si::molar_mass<double> molar_mass,
        const si::temperature<double> normal_melting_point,
        const si::temperature<double> critical_point_temperature
    ){
        double M = (molar_mass / (si::gram / si::mole));
        double Tm = (normal_melting_point / si::kelvin);
        double Tc = (critical_point_temperature / si::kelvin);
        using P = analytic::Polynomial<double,0,1>;
        using C = analytic::Clamped<double,P>;
        using R = analytic::Railyard<double,P>;
        auto T = analytic::Identity<double>();
        P kl = 1.951 * (1.0 - 0.00126 * (T - Tm)) / (std::pow(Tm, 0.216)*std::pow(M, 0.3));
        // Tmax is set either to give the lowest thermal conductivity ever observed, or Tc, whichever comes first
        double Tmax = std::min(Tc, analytic::solution(kl,0.011)); 
        return relation::PolynomialRailyardRelation<si::temperature<double>, si::thermal_conductivity<double>,0,1>(
            R(C(0.0, Tmax, kl)),
            si::kelvin, si::watt / (si::meter*si::kelvin));
    }

    /*
    `get_perry_johnson_temperature_function()` uses Perry coefficients for high temperature,
    and interpolated values from Johnson (1960) for low temperature.
    */
    // 23 uses, all for heat capacity of solids
    relation::PolynomialRailyardRelation<si::temperature<double>, si::specific_heat_capacity<double>,  -2,3> get_perry_johnson_temperature_function( 
        const si::temperature<double> Tunits, 
        const si::specific_heat_capacity<double> y_units_johnson, 
        const double linear_johnson, const double cube_johnson, 
        const double T_max_johnson,
        const si::specific_heat_capacity<double> y_units_perry,
        const double intercept_perry, const double linear_perry, const double inverse_square_perry, const double square_perry, 
        const double T_min_perry, const double T_max_perry
    ){
            const double oo = std::numeric_limits<double>::max();
            using P = analytic::Polynomial<double,-2,3>;
            using R = analytic::Railcar<double,P>;
            analytic::Polynomial<double,1,3>  johnson    = analytic::Polynomial<double,1,3> ({linear_johnson, 0.0, cube_johnson});
            analytic::Polynomial<double,-2,2> perry      = y_units_perry/y_units_johnson * analytic::Polynomial<double,-2,2>({inverse_square_perry, 0.0, intercept_perry, linear_perry, square_perry});
            analytic::Polynomial<double,0,1>  transition = analytic::linear_newton_polynomial(T_max_johnson, T_min_perry, johnson(T_max_johnson), perry(T_min_perry));
            return relation::PolynomialRailyardRelation<si::temperature<double>,si::specific_heat_capacity<double>,-2,3>(
                analytic::PolynomialRailyard<double,-2,3>({
                    R(0.0, T_max_johnson,         P(johnson)),            // johnson polynomial
                    R(T_max_johnson, T_min_perry, P(transition)),         // linear perry/johnson interpolation
                    R(T_min_perry, T_max_perry,   P(perry)),              // perry polynomial
                    R(T_max_perry, oo,            P(perry(T_max_perry))), // upper bound of perry
                }), Tunits, y_units_johnson);
    }
    
    template<typename Ty>
    relation::PolynomialRailyardRelation<si::spatial_frequency<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavelength(
        const si::length<double> lunits, const Ty yunits,
        const std::vector<double> ls, 
        const std::vector<double> lys
    ){
        assert(ls.size() == lys.size());
        const si::spatial_frequency<double> nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> ys;
        for (std::size_t i=0; i<lys.size(); i++){
            ns.push_back(1.0/(ls[i]));
            ys.push_back(lys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(ys.begin(), ys.end());
        return relation::PolynomialRailyardRelation<si::spatial_frequency<double>,Ty,0,1>(analytic::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    relation::PolynomialRailyardRelation<si::spatial_frequency<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavenumber(
        const si::spatial_frequency<double> nunits, const Ty yunits,
        const std::vector<double> ns, 
        const std::vector<double> ys
    ){
        assert(ns.size() == ys.size());
        return relation::PolynomialRailyardRelation<si::spatial_frequency<double>,Ty,0,1>(analytic::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    relation::PolynomialRailyardRelation<si::spatial_frequency<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output(
        const si::spatial_frequency<double> nunits, const Ty yunits,
        const std::vector<double>      ns, 
        const std::vector<double> log10ys
    ){
        assert(ns.size() == log10ys.size());
        std::vector<double> ys;
        for (std::size_t i=0; i<log10ys.size(); i++){
            ys.push_back(pow(10.0, log10ys[i]));
        }
        return relation::PolynomialRailyardRelation<si::spatial_frequency<double>,Ty,0,1> (analytic::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    relation::PolynomialRailyardRelation<si::spatial_frequency<double>,Ty,0,3> get_spectral_cubic_interpolation_function_of_wavenumber_for_log10_sample_output(
        const si::spatial_frequency<double> nunits, const Ty yunits,
        const std::vector<double>      ns, 
        const std::vector<double> log10ys
    ){
        assert(ns.size() == log10ys.size());
        std::vector<double> ys;
        for (std::size_t i=0; i<log10ys.size(); i++){
            ys.push_back(pow(10.0, log10ys[i]));
        }
        return relation::PolynomialRailyardRelation<si::spatial_frequency<double>,Ty,0,3> (analytic::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    relation::PolynomialRailyardRelation<si::spatial_frequency<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_output(
        const si::length<double> lunits, const Ty yunits,
        const std::vector<double>      ls, 
        const std::vector<double>log10lys
    ){
        assert(ls.size() == log10lys.size());
        const si::spatial_frequency<double> nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> log10ys;
        for (std::size_t i=0; i<log10lys.size(); i++){
            ns.push_back(1.0/(ls[i]));
            log10ys.push_back(log10lys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(log10ys.begin(), log10ys.end());
        std::vector<double> ys;
        for (std::size_t i=0; i<log10ys.size(); i++){
            ys.push_back(pow(10.0, log10ys[i]));
        }
        return relation::PolynomialRailyardRelation<si::spatial_frequency<double>,Ty,0,1> (analytic::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }


    template<typename Ty>
    relation::PolynomialRailyardRelation<si::spatial_frequency<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_input(
        const si::spatial_frequency<double> nunits, const Ty yunits,
        const std::vector<double> log10ns, 
        const std::vector<double>      ys
    ){
        assert(log10ns.size() == ys.size());
        std::vector<double> ns;
        for (std::size_t i=0; i<log10ns.size(); i++){
            ns.push_back(pow(10.0, log10ns[i]));
        }
        return relation::PolynomialRailyardRelation<si::spatial_frequency<double>,Ty,0,1> (analytic::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }

    template<typename Ty>
    relation::PolynomialRailyardRelation<si::spatial_frequency<double>,Ty,0,1> get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_input(
        const si::length<double> lunits, const Ty yunits,
        const std::vector<double> log10ls, 
        const std::vector<double>      lys
    ){
        assert(log10ls.size() == lys.size());
        const si::spatial_frequency<double> nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> ys;
        for (std::size_t i=0; i<lys.size(); i++){
            ns.push_back(1.0/(pow(10.0, log10ls[i])));
            ys.push_back(lys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(ys.begin(), ys.end());
        return relation::PolynomialRailyardRelation<si::spatial_frequency<double>,Ty,0,1> (analytic::spline::linear_spline<double>(ns, ys), nunits, yunits);
    }


}}

