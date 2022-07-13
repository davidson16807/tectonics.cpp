#pragma once

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "CompletedSpectralField.hpp"

namespace compound {
namespace field {
    template<typename T1>
    bool operator==(const CompletedSpectralField<T1> first, const CompletedSpectralField<T1> second)
    {
        // std::function does not and cannot have a comparator,
        // so equality can only be determined pragmatically by sampling at given pressures/temperatures
        if(first.index() != second.index()){
            return false;
        }
        auto max_llo = 1000.0 * si::nanometer;
        auto max_lhi = 1000.0 * si::nanometer;
        auto max_p = 2.0*si::standard_pressure;
        auto max_T = 2.0*si::standard_temperature;
        auto f = std::function<SpectralSample<T1>(const T1, const si::wavenumber<double>, const si::wavenumber<double>, const si::pressure<double>, const si::temperature<double>)>(
            [](const T1 x, const si::wavenumber<double> nlo, const si::wavenumber<double> nhi, const si::pressure<double> p, const si::temperature<double> T){ 
                return SpectralSample<T1>(x,nlo,nhi,p,T);
            }
        );
        for(si::pressure<double> p = 0.01*si::pascal; p<max_p; p+=max_p/3.0)
        {
            for(si::temperature<double> T = 0.01*si::kelvin; T<max_T; T+=max_T/3.0)
            {
                for(si::length<double> llo = 100.0*si::nanometer; llo<max_llo; llo+=200.0*si::nanometer)
                {
                    for(si::length<double> lhi = 100.0*si::nanometer; lhi<max_lhi; lhi+=200.0*si::nanometer)
                    {
                        if( first(1.0/lhi, 1.0/llo, p, T) != 
                            second(1.0/lhi, 1.0/llo, p, T) ){
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }
    template<typename T1>
    bool operator!=(const CompletedSpectralField<T1>& first, const CompletedSpectralField<T1>& second)
    {
        return !(first==second);
    }

    template<typename Ty>
    field::CompletedSpectralField<Ty> completed_spectral_invariant_test(
        const Ty value
    ){
        return field::SpectralFunction<Ty>(
            [value]
            (const si::wavenumber<double> nlo, 
             const si::wavenumber<double> nhi, 
             const si::pressure<double> p, 
             const si::temperature<double> T)
            {
                return value;
            }
        );
    }
}}
double test_CompletedSpectralField(const si::wavenumber<double> nlo, const si::wavenumber<double> nhi, const si::pressure<double> p, const si::temperature<double> T)
{
    return (si::mole*si::universal_gas_constant*T/p/si::liter);
}
