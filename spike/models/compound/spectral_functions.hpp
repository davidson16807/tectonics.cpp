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

    template<typename Ty>
    field::SpectralFunction<Ty> get_spectral_linear_interpolation_function_of_wavenumber(
        const si::wavenumber nunits, const Ty yunits,
        const std::vector<double> ls, 
        const std::vector<double> ys
    ){
        assert(ls.size() == ys.size());
        return field::SpectralFunction<Ty>(
            [nunits, yunits, ls, ys]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return (math::integral_of_lerp(ls, ys, (nlo/nunits), (nhi/nunits)) 
                    / (nhi/nunits - nlo/nunits)) * yunits;
            }
        );
    }

    template<typename Ty>
    field::SpectralFunction<Ty> get_spectral_linear_interpolation_function_of_wavelength(
        const si::length lunits, const Ty yunits,
        const std::vector<double> ls, 
        const std::vector<double> ys
    ){
        assert(ls.size() == ys.size());
        const si::wavenumber nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> nys;
        for (std::size_t i=0; i<ys.size(); i++){
            ns.push_back(1.0/(ls[i]));
            nys.push_back(ys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(nys.begin(), nys.end());
        return field::SpectralFunction<Ty>(
            [nunits, yunits, ns, nys]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return (math::integral_of_lerp(ns, nys, (nlo/nunits), (nhi/nunits)) 
                    / (nhi/nunits - nlo/nunits)) * yunits;
            }
        );
    }

    template<typename Ty>
    field::SpectralFunction<Ty> get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_output(
        const si::wavenumber lunits, const Ty yunits,
        const std::vector<double>     ls, 
        const std::vector<double>log10ys
    ){
        assert(ls.size() == log10ys.size());
        return field::SpectralFunction<Ty>(
            [lunits, yunits, ls, log10ys]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return std::pow(10.0, math::integral_of_lerp(
                        ls, log10ys, 
                        (nlo/lunits), 
                        (nhi/lunits)) 
                    / (nhi/lunits - nlo/lunits)) * yunits;
            }
        );
    }

    template<typename Ty>
    field::SpectralFunction<Ty> get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_output(
        const si::length lunits, const Ty yunits,
        const std::vector<double>     ls, 
        const std::vector<double>log10ys
    ){
        assert(ls.size() == log10ys.size());
        const si::wavenumber nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> nlog10ys;
        for (std::size_t i=0; i<log10ys.size(); i++){
            ns.push_back(1.0/(ls[i]));
            nlog10ys.push_back(log10ys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(nlog10ys.begin(), nlog10ys.end());
        return field::SpectralFunction<Ty>(
            [nunits, yunits, ns, nlog10ys]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return std::pow(10.0, math::integral_of_lerp(
                        ns, nlog10ys, 
                        (nlo/nunits), 
                        (nhi/nunits)) 
                    / (nhi/nunits - nlo/nunits)) * yunits;
            }
        );
    }

    template<typename Ty>
    field::SpectralFunction<Ty> get_spectral_linear_interpolation_function_of_wavenumber_for_log10_sample_input(
        const si::wavenumber lunits, const Ty yunits,
        const std::vector<double>log10ls, 
        const std::vector<double>     ys
    ){
        assert(log10ls.size() == ys.size());
        const si::wavenumber nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> nys;
        for (std::size_t i=0; i<ys.size(); i++){
            ns.push_back(1.0/std::pow(10.0, log10ls[i]));
            nys.push_back(ys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(nys.begin(), nys.end());
        return field::SpectralFunction<Ty>(
            [lunits, yunits, ns, nys]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return (math::integral_of_lerp(
                        ns, nys, 
                        nlo/lunits, 
                        nhi/lunits) 
                    / (nhi/lunits - nlo/lunits)) * yunits;
            }
        );
    }

    template<typename Ty>
    field::SpectralFunction<Ty> get_spectral_linear_interpolation_function_of_wavelength_for_log10_sample_input(
        const si::length lunits, const Ty yunits,
        const std::vector<double>log10ls, 
        const std::vector<double>     ys
    ){
        assert(log10ls.size() == ys.size());
        const si::wavenumber nunits = 1.0/lunits;
        std::vector<double> ns;
        std::vector<double> nys;
        for (std::size_t i=0; i<log10ls.size(); i++){
            ns.push_back(1.0/std::pow(10.0, log10ls[i]));
            nys.push_back(ys[i]);
        }
        std::reverse(ns.begin(), ns.end());
        std::reverse(nys.begin(), nys.end());
        return field::SpectralFunction<Ty>(
            [nunits, yunits, ns, nys]
            (const si::wavenumber nlo, 
             const si::wavenumber nhi, 
             const si::pressure p, 
             const si::temperature T)
            {
                return (math::integral_of_lerp(
                        ns, nys,
                        nlo/nunits, 
                        nhi/nunits) 
                    / (nhi/nunits - nlo/nunits)) * yunits;
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