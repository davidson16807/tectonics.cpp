#pragma once

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "OptionalSpectralField.hpp"

namespace compound{
namespace field{
    template<typename T1>
    bool operator==(const OptionalSpectralField<T1> first, const OptionalSpectralField<T1> second)
    {
        // std::function does not and cannot have a comparator,
        // so equality can only be determined pragmatically by sampling at given pressures/temperatures
        if(first.index() != second.index())
        {
            return false;
        }
        if((first.index() | second.index()) == 0)
        {
            return true;
        }
        if(first.parameters() != second.parameters())
        {
            return false;
        }
        for(si::pressure p = 0.01*si::pascal; p<2.0*si::standard_pressure; p+=(si::standard_pressure/2.0))
        {
            for(si::temperature T = 0.01*si::kelvin; T<2.0*si::standard_temperature; T+=(si::standard_temperature/2.0))
            {
                for(si::length llo = 100.0*si::nanometer; llo<1000.0 * si::nanometer; llo+=200.0*si::nanometer)
                {
                    for(si::length lhi = 100.0*si::nanometer; lhi<1000.0 * si::nanometer; lhi+=200.0*si::nanometer)
                    {
                        auto difference = (first(1.0/lhi, 1.0/llo, p, T).value() - second(1.0/lhi, 1.0/llo, p, T).value()) / T1(1.0);
                        if( difference*difference > 1e-4 ){
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }
    template<typename T1>
    bool operator!=(const OptionalSpectralField<T1>& first, const OptionalSpectralField<T1>& second)
    {
        return !(first==second);
    }
}}

double test_OptionalSpectralField(const si::wavenumber nlo, const si::wavenumber nhi, si::pressure p, const si::temperature T)
{
    return (si::mole*si::universal_gas_constant*T/p/si::liter);
}