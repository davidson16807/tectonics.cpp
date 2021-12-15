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
            (const si::pressure p, 
             const si::temperature T)
            {
                return value;
            }
        );
    }

    field::StateFunction<int> get_simon_glatzel_phase_function(
        const si::pressure        p0, // triple point pressure
        const si::temperature     t0, // triple point temperature
        const si::pressure        pc, // critical point pressure
        const si::temperature     tc, // critical point temperature
        const si::specific_energy L,  // latent heat of vaporization at boiling point
        const si::molar_mass      M,  // molar mass
        const si::pressure        a,  // simon glatzel slope
        const si::pressure        b,  // simon glatzel intercept
        const float               c   // simon glatzel exponent
    ){
        return field::StateFunction<int>(
            [p0, t0, pc, tc, L,  M, a, b, c]
            (const si::pressure p, 
             const si::temperature T)
            {
                return property::get_simon_glatzel_phase(p, T, p0, t0, pc, tc, L,  M, a, b, c);
            }
        );
    }

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_interpolated_temperature_function(
        const Tx xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        return field::StateFunction<Ty>(
            [xunits, yunits, xs, ys]
            (const si::pressure p, const si::temperature T)
            {
                return math::lerp(xs, ys, T/xunits) * yunits;
            }
        );
    }

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_interpolated_pressure_function(
        const Tx xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        return field::StateFunction<Ty>(
            [xunits, yunits, xs, ys]
            (const si::pressure p, const si::temperature T)
            {
                return math::lerp(xs, ys, p/xunits) * yunits;
            }
        );
    }

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_interpolated_inverse_temperature_function(
        const Tx xunits, const Ty yunits,
        const std::vector<double>xs, 
        const std::vector<double>ys
    ){
        return field::StateFunction<Ty>(
            [xunits, yunits, xs, ys]
            (const si::pressure p, const si::temperature T)
            {
                return math::lerp(xs, ys, 1.0/(T/xunits)) * yunits;
            }
        );
    }

    template<typename TT, typename Ty>
    field::StateFunction<Ty> get_interpolated_pressure_temperature_function(
        const TT Tunits,  const Ty yunits,
        const std::vector<double>Ts, 
        const si::pressure lop, const std::vector<double>lop_ys, 
        const si::pressure hip, const std::vector<double>hip_ys
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, Ts, lop, lop_ys, hip, hip_ys]
            (const si::pressure p, const si::temperature T)
            {
                return math::mix(math::lerp(Ts, lop_ys, T/Tunits), 
                                 math::lerp(Ts, hip_ys, T/Tunits), 
                                 math::linearstep(lop, hip, p)) * yunits;
            }
        );
    }

    template<typename TT, typename Ty>
    field::StateFunction<Ty> get_interpolated_pressure_temperature_function(
        const TT Tunits, const Ty yunits,
        const std::vector<double>Ts, 
        const si::pressure p0, const std::vector<double>yp0, 
        const si::pressure p1, const std::vector<double>yp1,
        const si::pressure p2, const std::vector<double>yp2
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, Ts, p0, p1, p2, yp0, yp1, yp2]
            (const si::pressure p, const si::temperature T)
            {
                return math::lerp(
                    std::vector<si::pressure>{p0, p1, p2},
                    std::vector<double>{
                        math::lerp(Ts, yp0, T/Tunits), 
                        math::lerp(Ts, yp1, T/Tunits), 
                        math::lerp(Ts, yp2, T/Tunits)
                    }, p) * yunits; 
            }
        );
    }

    template<typename TT, typename Tp, typename Ty>
    field::StateFunction<Ty> get_exponent_pressure_temperature_function(
        const TT Tunits, const Tp punits, const Ty yunits, 
        const double pslope, const double pexponent, 
        const double Tslope, const double Texponent, 
        const double intercept,
        const double Tmin, const double Tmax, 
        const double Pmin, const double Pmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, punits, yunits, 
             pslope, pexponent, Tslope, Texponent, intercept, 
             Tmin, Tmax, Pmin, Pmax]
            (const si::pressure p, const si::temperature T)
            {
                return (pslope*std::pow(std::clamp(p/punits, Pmin, Pmax), pexponent)
                      + Tslope*std::pow(std::clamp(T/Tunits, Tmin, Tmax), Texponent)
                      + intercept) * yunits;
            }
        );
    }

    template<typename TT, typename Tp, typename Ty>
    field::StateFunction<Ty> get_sigmoid_exponent_pressure_temperature_function(
        const TT Tunits, const Tp punits, const Ty yunits, 
        const double pslope, const double pexponent, 
        const double Tslope, const double Texponent, 
        const double Tsigmoid_max, const double Tsigmoid_scale, const double Tsigmoid_center,  
        const double intercept
    ){
        return field::StateFunction<Ty>(
            [Tunits, punits, yunits, 
             pslope, pexponent, Tslope, Texponent, 
             Tsigmoid_max, Tsigmoid_scale, Tsigmoid_center, 
             intercept]
            (const si::pressure p, const si::temperature T)
            {
                const double Tsigmoid_input = (T/Tunits - Tsigmoid_center)/Tsigmoid_scale;
                return (pslope*std::pow(p/punits, pexponent)
                      + Tslope*std::pow(T/Tunits, Texponent)
                      + Tsigmoid_max * Tsigmoid_input / std::sqrt(1.0 + Tsigmoid_input*Tsigmoid_input)
                      + intercept) * yunits;
            }
        );
    }

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_perry_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double intercept, const double linear, const double inverse_square, const double square,
        const double Tmin, const double Tmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, intercept, linear, inverse_square, square, Tmin, Tmax]
            (const si::pressure p, const si::temperature T)
            {
                double t = std::clamp(T/Tunits, Tmin, Tmax);
                return (intercept + linear*t + inverse_square/(t*t) + square*t*t)*yunits;
            }
        );
    }

    /*
    `get_perry_johnson_temperature_function()` uses Perry coefficients for high temperature,
    and interpolated values from Johnson (1960) for low temperature.
    */
    template<typename Tx>
    field::StateFunction<si::specific_heat_capacity> get_perry_johnson_temperature_function(
        const Tx Tunits, 
        const si::specific_heat_capacity y_units_johnson, 
        const double linear_johnson, const double cube_johnson, 
        const double T_max_johnson,
        const si::specific_heat_capacity y_units_perry,
        const double intercept_perry, const double linear_perry, const double inverse_square_perry, const double square_perry, 
        const double T_min_perry, const double T_max_perry
    ){
        return field::StateFunction<si::specific_heat_capacity>(
            [Tunits, 
             y_units_johnson, linear_johnson, cube_johnson, T_max_johnson,
             y_units_perry, intercept_perry, linear_perry, inverse_square_perry, square_perry, T_min_perry, T_max_perry]
            (const si::pressure p, const si::temperature T)
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

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_perry_temperature_function_for_specific_heat_capacity_as_solid(
        const Tx Tunits, const Ty yunits,
        const double intercept, const double linear, const double inverse_square, const double square,
        const double Tmin, const double Tmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, intercept, linear, inverse_square, square, Tmin, Tmax]
            (const si::pressure p, const si::temperature T)
            {
                double t = std::clamp(T/Tunits, Tmin, Tmax);
                auto y_perry = intercept + linear*t + inverse_square/(t*t) + square*t*t;
                return math::lerp(
                    std::vector<double>{0.0,    Tmin},
                    std::vector<double>{0.0, y_perry}, T/Tunits) * yunits;
            }
        );
    }

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_dippr_liquid_heat_capacity_temperature_function_114(
        const Tx Tc, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4, const double c5,
        const Tx Tmin, Tx Tmax
    ){
        return field::StateFunction<Ty>(
            [Tc, yunits, Tmin, Tmax, c1, c2, c3, c4, c5]
            (const si::pressure p, const si::temperature T)
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

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_dippr_liquid_heat_capacity_temperature_function_100(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4, const double c5,
        const Tx Tmin, Tx Tmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, Tmin, Tmax, c1, c2, c3, c4, c5]
            (const si::pressure p, const si::temperature T)
            {
                double t = std::clamp(T, Tmin, Tmax)/Tunits;
                return (c1+c2*t + c3*t*t + c4*t*t*t + c5*t*t*t*t)*yunits;
            }
        );
    }

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_dippr_liquid_density_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4,
        const double Tmin, const double Tmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4, Tmin, Tmax]
            (const si::pressure p, const si::temperature T)
            {
                double t = std::clamp(T/Tunits, Tmin, Tmax);
                return (c1 / std::pow(c2, 1+std::pow(1.0-(t/c3), c4)))*yunits;
            }
        );
    }

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_dippr_gas_viscosity_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4,
        const double Tmin, const double Tmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4, Tmin, Tmax]
            (const si::pressure p, const si::temperature T)
            {
                double t = std::clamp(T/Tunits, Tmin, Tmax);
                return (c1*std::pow(t, c2) / (1.0+c3/t + c4/(t*t)))*yunits;
            }
        );
    }

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_dippr_liquid_viscosity_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4, const double c5,
        const double Tmin, const double Tmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4, c5, Tmin, Tmax]
            (const si::pressure p, const si::temperature T)
            {
                double t = std::clamp(T/Tunits, Tmin, Tmax);
                return exp(c1 + c2/t + c3*std::log(t) + c4*std::pow(t,c5))*yunits;
            }
        );
    }

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_dippr_gas_thermal_conductivity_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4,
        const double Tmin, const double Tmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4, Tmin, Tmax]
            (const si::pressure p, const si::temperature T)
            {
                double t = std::clamp(T/Tunits, Tmin, Tmax);
                return (c1*std::pow(t,c2) / (1.0 + c3/t + c4/(t*t)))*yunits;
            }
        );
    }

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_dippr_liquid_thermal_conductivity_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4, const double c5,
        const double Tmin, const double Tmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4, c5, Tmin, Tmax]
            (const si::pressure p, const si::temperature T)
            {
                double t = std::clamp(T/Tunits, Tmin, Tmax);
                return (c1 + c2*t + c3*t*t + c4*t*t*t + c5*t*t*t*t)*yunits;
            }
        );
    }

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_dippr_liquid_vapor_pressure_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4, const double c5,
        const double Tmin, const double Tmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4, c5, Tmin, Tmax]
            (const si::pressure p, const si::temperature T)
            {
                double t = std::clamp(T/Tunits, Tmin, Tmax);
                return ( std::exp(c1 + c2/t + c3*std::log(t) + c4*std::pow(t,c5)) )*yunits;
            }
        );
    }
    // from Mulero (2012)

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_refprop_liquid_surface_tension_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double Tc, const double sigma0, const double n0, const double sigma1, const double n1, const double sigma2, const double n2,
        const double Tmin, const double Tmax
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, Tc, sigma0, n0, sigma1, n1, sigma2, n2, Tmin, Tmax]
            (const si::pressure p, const si::temperature T)
            {
                double Tr = std::clamp(T/Tunits, Tmin, Tmax)/Tc;
                return ( sigma0*std::pow(1.0 - Tr, n0) + sigma1*std::pow(1.0 - Tr, n1) + sigma2*std::pow(1.0 - Tr, n2) )*yunits;
            }
        );
    }
    // from Mulero (2012)


    field::StateFunction<si::surface_energy> get_jasper_temperature_function(
        const double intercept, const double linear, const double square
    ){
        return field::StateFunction<si::surface_energy>(
            [intercept, linear, square]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/si::celcius;
                return (intercept + linear*t + square*t*t)*si::dyne/si::centimeter;
            }
        );
    }
    // from Jasper (1972)

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_quadratic_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double intercept, const double linear, const double square
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, intercept, linear, square]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tunits;
                return (intercept + linear*t + square*t*t)*yunits;
            }
        );
    }

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_quadratic_pressure_function(
        const Tx Punits, const Ty yunits,
        const double intercept, const double linear, const double square
    ){
        return field::StateFunction<Ty>(
            [Punits, yunits, intercept, linear, square]
            (const si::pressure p, const si::temperature T)
            {
                double P = p/Punits;
                return (intercept + linear*P + square*P*P)*yunits;
            }
        );
    }

    template<typename Tx, typename Ty>
    field::StateFunction<Ty> get_linear_liquid_surface_tension_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double TL, const double gammaTL, const double dgamma_dT
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, TL, gammaTL, dgamma_dT]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tunits;
                return ( gammaTL + dgamma_dT * (t-TL) )*yunits;
            }
        );
    }

    template<typename TT>
    field::StateFunction<si::pressure> get_antoine_vapor_pressure_function(
        const TT Tunits, const si::pressure punits,
        const double A, const double B, const double C
    ){
        return field::StateFunction<si::pressure>(
            [Tunits, punits, A,B,C]
            (const si::pressure p, const si::temperature T)
            {
                return exp(A - B / (C+T/Tunits)) * punits;
            }
        );
    }

    template<typename TT>
    field::StateFunction<si::pressure> get_antoine_vapor_pressure_function(
        const TT Tunits, const si::pressure punits,
        const double A, const double B, const double C,
        const double Tmin, double Tmax
    ){
        return field::StateFunction<si::pressure>(
            [Tunits, punits, A,B,C, Tmin, Tmax]
            (const si::pressure p, const si::temperature T)
            {
                return exp(A - B / (C+std::clamp(T/Tunits,Tmin,Tmax))) * punits;
            }
        );
    }

} // end namespace compound