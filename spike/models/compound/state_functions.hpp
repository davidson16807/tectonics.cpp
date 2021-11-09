#pragma once

// in-house libraries
#include <math/lerp.hpp>
#include <units/si.hpp>

#include "field/state/OptionalStateField.hpp"
#include "field/spectral/OptionalSpectralField.hpp"

#include "field/state/CompletedStateField.hpp"
#include "field/spectral/CompletedSpectralField.hpp"

namespace compound {

    template<typename Tx, typename Ty>
    field::OptionalStateField<Ty> get_interpolated_temperature_function(
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
    field::OptionalStateField<Ty> get_interpolated_inverse_temperature_function(
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
    field::OptionalStateField<Ty> get_interpolated_pressure_temperature_function(
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
    field::OptionalStateField<Ty> get_interpolated_pressure_temperature_function(
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
    field::OptionalStateField<Ty> get_exponent_pressure_temperature_function(
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
    field::OptionalStateField<Ty> get_sigmoid_exponent_pressure_temperature_function(
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
    field::OptionalStateField<Ty> get_perry_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double intercept, const double linear, const double inverse_square, const double square
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, intercept, linear, inverse_square, square]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tunits;
                return (intercept + linear*t + inverse_square/(t*t) + square*t*t)*yunits;
            }
        );
    }
    template<typename Tx, typename Ty>
    field::OptionalStateField<Ty> get_dippr_liquid_heat_capacity_temperature_function_114(
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
    field::OptionalStateField<Ty> get_dippr_liquid_heat_capacity_temperature_function_100(
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
    field::OptionalStateField<Ty> get_dippr_liquid_density_temperature_function(
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
    field::OptionalStateField<Ty> get_dippr_gas_viscosity_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tunits;
                return (c1*std::pow(t, c2) / (1.0+c3/t + c4/(t*t)))*yunits;
            }
        );
    }
    template<typename Tx, typename Ty>
    field::OptionalStateField<Ty> get_dippr_liquid_viscosity_temperature_function(
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
    field::OptionalStateField<Ty> get_dippr_gas_thermal_conductivity_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tunits;
                return (c1*std::pow(t,c2) / (1.0 + c3/t + c4/(t*t)))*yunits;
            }
        );
    }
    template<typename Tx, typename Ty>
    field::OptionalStateField<Ty> get_dippr_liquid_thermal_conductivity_temperature_function(
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
    field::OptionalStateField<Ty> get_dippr_liquid_vapor_pressure_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4, const double c5
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4, c5]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tunits;
                return ( std::exp(c1 + c2/t + c3*std::log(t) + c4*std::pow(t,c5)) )*yunits;
            }
        );
    }
    // from Mulero (2012)
    template<typename Tx, typename Ty>
    field::OptionalStateField<Ty> get_refprop_liquid_surface_tension_temperature_function(
        const Tx Tc, const Ty yunits,
        const double sigma1, const double n1, const double sigma2, const double n2, const double sigma3, const double n3
    ){
        return field::StateFunction<Ty>(
            [Tc, yunits, sigma1, n1, sigma2, n2, sigma3, n3]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tc;
                return ( sigma1*std::pow(1.0 - t, n1) + sigma2*std::pow(1.0 - t, n2) + sigma3*std::pow(1.0 - t, n3) )*yunits;
            }
        );
    }
    // from Mulero (2012)
    template<typename Tx, typename Ty>
    field::OptionalStateField<Ty> get_linear_liquid_surface_tension_temperature_function(
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
    field::OptionalStateField<si::pressure> get_antoine_vapor_pressure_function(
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
    field::OptionalStateField<si::pressure> get_antoine_vapor_pressure_function(
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
    field::OptionalSpectralField<si::area> get_molecular_absorption_cross_section_function(
        const si::wavenumber xunits, const si::area yunits,
        const std::vector<double>     xs, 
        const std::vector<double>log10ys
    ){
        return field::SpectralFunction<si::area>(
            [xunits, yunits, xs, log10ys]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return std::pow(10.0, math::integral_of_lerp(xs, log10ys, (nlo*si::meter), (nhi*si::meter)) 
                    / (nhi/xunits - nlo/xunits)) * yunits;
            }
        );
    }
    field::OptionalSpectralField<double> get_interpolated_refractive_index_function(
        const si::length lunits, 
        const std::vector<double>log10ls, 
        const std::vector<double>     ns
    ){
        return field::SpectralFunction<double>(
            [lunits, log10ls, ns]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                double l = (2.0 / (nhi+nlo) / lunits);
                return math::lerp(log10ls, ns, log10(l));
            }
        );
    }









    template<typename Tx, typename Ty>
    field::CompletedStateField<Ty> get_completed_interpolated_temperature_function(
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
    template<typename TT, typename Ty>
    field::CompletedStateField<Ty> get_completed_interpolated_pressure_temperature_function(
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
    field::CompletedStateField<Ty> get_completed_interpolated_pressure_temperature_function(
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
    field::CompletedStateField<Ty> get_completed_exponent_pressure_temperature_function(
        const TT Tunits, const Tp punits, const Ty yunits, 
        const double pslope, const double pexponent, 
        const double Tslope, const double Texponent, 
        const double intercept
    ){
        return field::StateFunction<Ty>(
            [Tunits, punits, yunits, pslope, pexponent, Tslope, Texponent, intercept]
            (const si::pressure p, const si::temperature T)
            {
                return (pslope*std::pow(p/punits, pexponent)
                      + Tslope*std::pow(T/Tunits, Texponent)
                      + intercept) * yunits;
            }
        );
    }
    template<typename TT, typename Tp, typename Ty>
    field::CompletedStateField<Ty> get_completed_sigmoid_exponent_pressure_temperature_function(
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
    field::CompletedStateField<Ty> get_completed_perry_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double intercept, const double linear, const double inverse_square, const double square
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, intercept, linear, inverse_square, square]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tunits;
                return (intercept + linear*t + inverse_square/(t*t) + square*t*t)*yunits;
            }
        );
    }
    template<typename Tx, typename Ty>
    field::CompletedStateField<Ty> get_completed_dippr_liquid_heat_capacity_temperature_function_100(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4, const double c5,
        const Tx Tmin, const Tx Tmax
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
    field::CompletedStateField<Ty> get_completed_dippr_liquid_density_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tunits;
                return (c1 / std::pow(c2, 1+std::pow(1.0-(t/c3), c4)))*yunits;
            }
        );
    }
    template<typename Tx, typename Ty>
    field::CompletedStateField<Ty> get_completed_dippr_gas_viscosity_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tunits;
                return (c1*std::pow(t, c2) / (1.0+c3/t + c4/(t*t)))*yunits;
            }
        );
    }
    template<typename Tx, typename Ty>
    field::CompletedStateField<Ty> get_completed_dippr_liquid_viscosity_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4, const double c5
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4, c5]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tunits;
                return exp(c1 + c2/t + c3*std::log(t) + c4*std::pow(t,c5))*yunits;
            }
        );
    }
    template<typename Tx, typename Ty>
    field::CompletedStateField<Ty> get_completed_dippr_gas_thermal_conductivity_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tunits;
                return (c1*std::pow(t,c2) / (1.0 + c3/t + c4/(t*t)))*yunits;
            }
        );
    }
    template<typename Tx, typename Ty>
    field::CompletedStateField<Ty> get_completed_dippr_liquid_thermal_conductivity_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4, const double c5
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4, c5]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tunits;
                return (c1 + c2*t + c3*t*t + c4*t*t*t + c5*t*t*t*t)*yunits;
            }
        );
    }
    template<typename Tx, typename Ty>
    field::CompletedStateField<Ty> get_completed_dippr_liquid_vapor_pressure_temperature_function(
        const Tx Tunits, const Ty yunits,
        const double c1, const double c2, const double c3, const double c4, const double c5
    ){
        return field::StateFunction<Ty>(
            [Tunits, yunits, c1, c2, c3, c4, c5]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tunits;
                return ( std::exp(c1 + c2/t + c3*std::log(t) + c4*std::pow(t,c5)) )*yunits;
            }
        );
    }
    // from Mulero (2012)
    template<typename Tx, typename Ty>
    field::CompletedStateField<Ty> get_completed_refprop_liquid_surface_tension_temperature_function(
        const Tx Tc, const Ty yunits,
        const double sigma1, const double n1, const double sigma2, const double n2, const double sigma3, const double n3
    ){
        return field::StateFunction<Ty>(
            [Tc, yunits, sigma1, n1, sigma2, n2, sigma3, n3]
            (const si::pressure p, const si::temperature T)
            {
                double t = T/Tc;
                return ( sigma1*std::pow(1.0 - t, n1) + sigma2*std::pow(1.0 - t, n2) + sigma3*std::pow(1.0 - t, n3) )*yunits;
            }
        );
    }
    // from Mulero (2012)
    template<typename Tx, typename Ty>
    field::CompletedStateField<Ty> get_completed_linear_liquid_surface_tension_temperature_function(
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
    field::CompletedStateField<si::pressure> get_completed_antoine_vapor_pressure_function(
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
    field::CompletedStateField<si::pressure> get_completed_antoine_vapor_pressure_function(
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
    field::CompletedSpectralField<si::area> get_completed_molecular_absorption_cross_section_function(
        const si::wavenumber xunits, const si::area yunits,
        const std::vector<double>     xs, 
        const std::vector<double>log10ys
    ){
        return field::SpectralFunction<si::area>(
            [xunits, yunits, xs, log10ys]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return std::pow(10.0, math::integral_of_lerp(xs, log10ys, (nlo*si::meter), (nhi*si::meter)) 
                    / (nhi/xunits - nlo/xunits)) * yunits;
            }
        );
    }
    field::CompletedSpectralField<double> get_completed_interpolated_refractive_index_function(
        const si::length lunits, 
        const std::vector<double>log10ls, 
        const std::vector<double>     ns
    ){
        return field::SpectralFunction<double>(
            [lunits, log10ls, ns]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                double l = (2.0 / (nhi+nlo) / lunits);
                return math::lerp(log10ls, ns, log10(l));
            }
        );
    }

} // end namespace compound