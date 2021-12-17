#pragma once

// in-house libraries
#include <math/lerp.hpp>
#include <units/si.hpp>

#include "property/reflectance.hpp"

#include "field/state/OptionalStateField.hpp"
#include "field/spectral/OptionalSpectralField.hpp"

#include "field/state/CompletedStateField.hpp"
#include "field/spectral/CompletedSpectralField.hpp"

namespace compound {

    template<typename Ty>
    field::SpectralFunction<Ty> spectral_invariant(
        const Ty value
    ){
        return field::SpectralFunction<Ty>(
            [value]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return value;
            }
        );
    }

    template<typename Tx, typename Ty>
    field::SpectralFunction<Ty> get_dewaele2003_pressure_function(
        const Tx xunits, const Ty yunits,
        const double a, const double b, const double c, const double d,
        const double Pmin, const double Pmax
    ){
        return field::SpectralFunction<Ty>(
            [xunits, yunits, a,b,c,d, Pmin, Pmax]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return (a+b*std::pow(1.0+c*std::clamp(p/xunits, Pmin, Pmax), d)) * yunits;
            }
        );
    }

    field::SpectralFunction<si::area> get_molecular_absorption_cross_section_function(
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
                return std::pow(10.0, math::integral_of_lerp(xs, log10ys, (nlo/xunits), (nhi/xunits)) 
                    / (nhi/xunits - nlo/xunits)) * yunits;
            }
        );
    }

    
    field::SpectralFunction<double> get_interpolated_refractive_index_function(
        const si::length lunits, 
        const std::vector<double>log10ls, 
        const std::vector<double>     ns
    ){
        std::vector<double> wavenumbers;
        std::vector<double> refractive_indices = ns;
        for (std::size_t i=0; i<log10ls.size(); i++){
            wavenumbers.push_back(1.0/(std::pow(10.0, log10ls[i])*lunits/si::meter));
        }
        std::reverse(wavenumbers.begin(), wavenumbers.end());
        std::reverse(refractive_indices.begin(), refractive_indices.end());
        return field::SpectralFunction<double>(
            [lunits, wavenumbers, refractive_indices]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return math::integral_of_lerp(wavenumbers, refractive_indices, (nlo*si::meter), (nhi*si::meter)) 
                    / (nhi*si::meter - nlo*si::meter);
            }
        );
    }
    
    
    field::SpectralFunction<double> get_linear_interpolated_refractive_index_function(
        const si::length lunits, 
        const std::vector<double> ls, 
        const std::vector<double> ns
    ){
        std::vector<double> wavenumbers;
        std::vector<double> refractive_indices = ns;
        for (std::size_t i=0; i<ls.size(); i++){
            wavenumbers.push_back(1.0/(ls[i]));
        }
        std::reverse(wavenumbers.begin(), wavenumbers.end());
        std::reverse(refractive_indices.begin(), refractive_indices.end());

        return field::SpectralFunction<double>(
            [lunits, wavenumbers, refractive_indices]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return math::integral_of_lerp(wavenumbers, refractive_indices, (nlo*lunits), (nhi*lunits)) 
                    / (nhi*lunits - nlo*lunits);
            }
        );
    }
    
    field::SpectralFunction<double> get_linear_interpolated_refractive_index_function_of_wavenumbers(
        const si::attenuation xunits, 
        const std::vector<double> xs, 
        const std::vector<double> ns
    ){
        return field::SpectralFunction<double>(
            [xunits, xs, ns]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return math::integral_of_lerp(xs, ns, nlo/xunits, nhi/xunits) / (nhi/xunits - nlo/xunits);
            }
        );
    }
    
    field::SpectralFunction<si::attenuation> get_absorption_coefficient_function_from_reflectance_at_wavelengths(
        const si::length lunits,
        const si::length particle_diameter, 
        const std::vector<double>wavelengths, 
        const std::vector<double>reflectances
    ){
        std::vector<double> wavenumbers;
        std::vector<double> wavenumber_reflectances = reflectances;
        for (std::size_t i=0; i<reflectances.size(); i++){
            wavenumbers.push_back(1.0/wavelengths[i]);
        }
        std::reverse(wavenumbers.begin(), wavenumbers.end());
        std::reverse(wavenumber_reflectances.begin(), wavenumber_reflectances.end());

        return field::SpectralFunction<si::attenuation>(
            [lunits, wavenumbers, wavenumber_reflectances, particle_diameter]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                double reflectance = math::integral_of_lerp(wavenumbers, wavenumber_reflectances, (nlo*lunits), (nhi*lunits)) 
                    / (nhi*lunits - nlo*lunits);
                double single_scatter_albedo = compound::property::approx_single_scatter_albedo_from_reflectance(reflectance);
                double scattering_efficiency = single_scatter_albedo; // we assume extinction efficiency is close to 1, in which case the two are equal
                return compound::property::approx_absorption_coefficient_from_scattering_efficiency(scattering_efficiency, particle_diameter);
            }
        );
    }
    
    field::SpectralFunction<si::attenuation> get_absorption_coefficient_function_from_reflectance_at_wavenumbers(
        const si::wavenumber xunits,
        const si::length particle_diameter, 
        const std::vector<double>wavenumbers, 
        const std::vector<double>reflectances
    ){
        return field::SpectralFunction<si::attenuation>(
            [xunits, wavenumbers, reflectances, particle_diameter]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                double reflectance = math::integral_of_lerp(wavenumbers, reflectances, (nlo/xunits), (nhi/xunits)) / (nhi/xunits - nlo/xunits);
                double single_scatter_albedo = compound::property::approx_single_scatter_albedo_from_reflectance(reflectance);
                double scattering_efficiency = single_scatter_albedo; // we assume extinction efficiency is close to 1, in which case the two are equal
                return compound::property::approx_absorption_coefficient_from_scattering_efficiency(scattering_efficiency, particle_diameter);
            }
        );
    }
    
    field::SpectralFunction<si::attenuation> get_absorption_coefficient_function_at_wavelengths(
        const si::length lunits, 
        const si::attenuation yunits, 
        const std::vector<double>wavelengths, 
        const std::vector<double>alphas
    ){
        std::vector<double> wavenumbers;
        std::vector<double> wavenumber_alphas = alphas;
        for (std::size_t i=0; i<wavelengths.size(); i++){
            wavenumbers.push_back(1.0/wavelengths[i]);
        }
        std::reverse(wavenumbers.begin(), wavenumbers.end());
        std::reverse(wavenumber_alphas.begin(), wavenumber_alphas.end());

        return field::SpectralFunction<si::attenuation>(
            [lunits, yunits, wavenumbers, wavenumber_alphas]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return  math::integral_of_lerp(wavenumbers, wavenumber_alphas, (nlo*lunits), (nhi*lunits)) 
                    / (nhi*lunits - nlo*lunits) * yunits;
            }
        );
    }
    
    field::SpectralFunction<si::attenuation> get_absorption_coefficient_function_at_wavenumbers(
        const si::wavenumber xunits, 
        const si::attenuation yunits, 
        const std::vector<double>wavenumbers, 
        const std::vector<double>alphas
    ){
        return field::SpectralFunction<si::attenuation>(
            [xunits, yunits, wavenumbers, alphas]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return math::integral_of_lerp(wavenumbers, alphas, (nlo/xunits), (nhi/xunits)) / (nhi/xunits - nlo/xunits) * yunits;
            }
        );
    }
    
    field::SpectralFunction<si::attenuation> get_absorption_coefficient_function_from_log_at_wavelengths(
        const si::length lunits, 
        const si::attenuation yunits, 
        const std::vector<double>wavelengths, 
        const std::vector<double>log_alphas
    ){
        std::vector<double> wavenumbers;
        std::vector<double> wavenumber_log_alphas = log_alphas;
        for (std::size_t i=0; i<wavenumbers.size(); i++){
            wavenumbers.push_back(1.0/wavelengths[i]);
        }
        std::reverse(wavenumbers.begin(), wavenumbers.end());
        std::reverse(wavenumber_log_alphas.begin(), wavenumber_log_alphas.end());

        return field::SpectralFunction<si::attenuation>(
            [lunits, yunits, wavenumbers, wavenumber_log_alphas]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return std::pow(10.0, math::integral_of_lerp(wavenumbers, wavenumber_log_alphas, (nlo*lunits), (nhi*lunits)) / (nhi*lunits - nlo*lunits)) * yunits;
            }
        );
    }
    
    field::SpectralFunction<si::attenuation> get_absorption_coefficient_function_from_log_at_wavenumbers(
        const si::wavenumber xunits, 
        const si::attenuation yunits, 
        const std::vector<double>wavenumbers, 
        const std::vector<double>log_alphas
    ){
        return field::SpectralFunction<si::attenuation>(
            [xunits, yunits, wavenumbers, log_alphas]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return std::pow(10.0, math::integral_of_lerp(wavenumbers, log_alphas, (nlo/xunits), (nhi/xunits)) / (nhi/xunits - nlo/xunits)) * yunits;
            }
        );
    }


} // end namespace compound