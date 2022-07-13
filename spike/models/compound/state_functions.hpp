#pragma once

// in-house libraries
#include <math/lerp.hpp>
#include <units/si.hpp>

#include "property/phase.hpp"
#include "property/reflectance.hpp"

#include "field/state/OptionalStateField.hpp"
#include "field/spectral/OptionalSpectralField.hpp"

#include "field/state/CompletedStateField.hpp"
#include "field/spectral/CompletedSpectralField.hpp"

namespace compound {
    using missing = std::monostate;

    template<typename Ty>
    field::StateFunction<Ty> state_invariant(
        const Ty value
    ){
        return field::StateFunction<Ty>(
            [value]
            (const si::pressure<double> p, 
             const si::temperature<double> T)
            {
                return value;
            }
        );
    }

    // 7 uses
    
    field::StateFunction<int> get_simon_glatzel_phase_function( 
        const si::pressure<double>        p0, /*triple point pressure*/
        const si::temperature<double>     t0, /*triple point temperature*/
        const si::pressure<double>        pc, /*critical point pressure*/
        const si::temperature<double>     tc, /*critical point temperature*/
        const si::specific_energy<double> L,  /*latent heat of vaporization at boiling point*/
        const si::molar_mass<double>      M,  /*molar mass*/
        const si::pressure<double>        a,  /*simon glatzel slope*/
        const si::pressure<double>        b,  /*simon glatzel intercept*/
        const float               c   /*simon glatzel exponent*/
    ){
        return field::StateFunction<int>(
            [p0, t0, pc, tc, L,  M, a, b, c]
            (const si::pressure<double> p, 
             const si::temperature<double> T)
            {
                return property::get_simon_glatzel_phase(p, T, p0, t0, pc, tc, L,  M, a, b, c);
            }
        );
    }

    // 175 uses
    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_interpolated_temperature_function(
        const Tx xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        return field::StateFunction<Ty>(
            [xunits, yunits, xs, ys]
            (const si::pressure<double> p, const si::temperature<double> T)
            {
                return math::lerp(xs, ys, T/xunits) * yunits;
            }
        );
    }

    // 3 uses: ice6 and ice7, for thermal conductivity and density
    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_interpolated_pressure_function(
        const Tx xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        return field::StateFunction<Ty>(
            [xunits, yunits, xs, ys]
            (const si::pressure<double> p, const si::temperature<double> T)
            {
                return math::lerp(xs, ys, p/xunits) * yunits;
            }
        );
    }

    // 3 uses: gold, silver, and copper, for dynamic viscosity of liquids
    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_interpolated_inverse_temperature_function( 
        const Tx xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        return field::StateFunction<Ty>(
            [xunits, yunits, xs, ys]
            (const si::pressure<double> p, const si::temperature<double> T)
            {
                return math::lerp(xs, ys, 1.0/(T/xunits)) * yunits;
            }
        );
    }

    /*
    `get_perry_johnson_temperature_function()` uses Perry coefficients for high temperature,
    and interpolated values from Johnson (1960) for low temperature.
    */
    // 23 uses, all for heat capacity of solids
    template<typename Tx>
    field::StateFunction<si::specific_heat_capacity<double>> get_perry_johnson_temperature_function( 
        const Tx Tunits, 
        const si::specific_heat_capacity<double> y_units_johnson, 
        const double linear_johnson, const double cube_johnson, 
        const double T_max_johnson,
        const si::specific_heat_capacity<double> y_units_perry,
        const double intercept_perry, const double linear_perry, const double inverse_square_perry, const double square_perry, 
        const double T_min_perry, const double T_max_perry
    ){
        return field::StateFunction<si::specific_heat_capacity<double>>(
            [Tunits, 
             y_units_johnson, linear_johnson, cube_johnson, T_max_johnson,
             y_units_perry, intercept_perry, linear_perry, inverse_square_perry, square_perry, T_min_perry, T_max_perry]
            (const si::pressure<double> p, const si::temperature<double> T)
            {
                double tj = std::clamp(T/Tunits, 0.0, T_max_johnson);
                auto y_johnson = (linear_johnson*tj + cube_johnson*tj*tj*tj) * y_units_johnson;
                double tp = std::clamp(T/Tunits, T_min_perry, T_max_perry);
                auto y_perry   = (intercept_perry + linear_perry*tp + inverse_square_perry/(tp*tp) + square_perry*tp*tp) * y_units_perry;
                return math::lerp(
                    std::vector<double>{T_max_johnson,             T_min_perry            },
                    std::vector<double>{y_johnson/y_units_johnson, y_perry/y_units_johnson}, T/Tunits) * y_units_johnson;
            }
        );
    }
    /*
    `get_dippr_quartic_temperature_function_100()` is equivalent to dippr function 100,
    for liquid thermal conductivity, heat capacity, and solid density
    */
    // 26 uses, for liquid thermal conductivity and heat capacity
    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_dippr_quartic_temperature_function_100( 
        const Tx Tunits, const Ty yunits,
        const double intercept, const double slope, const double square, const double cube, const double fourth,
        const double Tmin, double Tmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, Tmin, Tmax, intercept, slope, square, cube, fourth]
            (const si::pressure<double> p, const si::temperature<double> T)
            {
                double t = std::clamp(T/Tunits, Tmin, Tmax);
                return (intercept + slope*t + square*t*t + cube*t*t*t + fourth*t*t*t*t)*yunits;
            }
        );
    }

    // 42 uses, for viscosity and vapor pressures of liquids
    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_dippr_temperature_function_101( 
        const Tx Tunits, const Ty yunits,
        const double log_intercept, const double log_slope, const double log_log, const double log_exponentiated, const double exponent,
        const double Tmin, const double Tmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, log_intercept, log_slope, log_log, log_exponentiated, exponent, Tmin, Tmax]
            (const si::pressure<double> p, const si::temperature<double> T)
            {
                double t = std::clamp(T/Tunits, Tmin, Tmax);
                return std::exp(log_intercept + log_slope/t + log_log*std::log(t) + log_exponentiated*std::pow(t,exponent))*yunits;
            }
        );
    }

    // 20 uses, for density of liquids
    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_dippr_temperature_function_105( 
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4,
        const double Tmin, const double Tmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4, Tmin, Tmax]
            (const si::pressure<double> p, const si::temperature<double> T)
            {
                double t = std::clamp(T/Tunits, Tmin, Tmax);
                return (c1 / std::pow(c2, 1+std::pow(1.0-(t/c3), c4)))*yunits;
            }
        );
    }

    // 5 uses, for heat capacities of liquids
    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_dippr_liquid_heat_capacity_temperature_function_114( 
        const Tx Tc, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4, const double c5,
        const Tx Tmin, Tx Tmax
    ){
        return field::StateFunction<Ty>(
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
            }
        );
    }

    // 15 uses, for liquid surface tension
    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_refprop_liquid_surface_tension_temperature_function( 
        const Tx Tunits, const Ty yunits,
        const double Tc, const double sigma0, const double n0, const double sigma1, const double n1, const double sigma2, const double n2,
        const double Tmin, const double Tmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, Tc, sigma0, n0, sigma1, n1, sigma2, n2, Tmin, Tmax]
            (const si::pressure<double> p, const si::temperature<double> T)
            {
                double Tr = std::clamp(T/Tunits, Tmin, Tmax)/Tc;
                return ( sigma0*std::pow(1.0 - Tr, n0) + sigma1*std::pow(1.0 - Tr, n1) + sigma2*std::pow(1.0 - Tr, n2) )*yunits;
            }
        );
    }
    // from Mulero (2012)

    // 5 uses, for solid densities
    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_quadratic_pressure_function( 
        const Tx Punits, const Ty yunits,
        const double intercept, const double linear, const double square
    ){
        return field::StateFunction<Ty>(
            [Punits, yunits, intercept, linear, square]
            (const si::pressure<double> p, const si::temperature<double> T)
            {
                double P = p/Punits;
                return (intercept + linear*P + square*P*P)*yunits;
            }
        );
    }

    // 3 uses, for liquid surface tension
    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_linear_liquid_surface_tension_temperature_function( 
        const Tx Tunits, const Ty yunits,
        const double TL, const double gammaTL, const double dgamma_dT
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, TL, gammaTL, dgamma_dT]
            (const si::pressure<double> p, const si::temperature<double> T)
            {
                double t = T/Tunits;
                return ( gammaTL + dgamma_dT * (t-TL) )*yunits;
            }
        );
    }

} // end namespace compound