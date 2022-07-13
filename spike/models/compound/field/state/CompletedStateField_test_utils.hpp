#pragma once

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "OptionalStateField.hpp"

namespace compound {
namespace field {
    template<typename T1>
    bool operator==(const CompletedStateField<T1> first, const CompletedStateField<T1> second)
    {
        // std::function does not and cannot have a comparator,
        // so equality can only be determined pragmatically by sampling at given pressures/temperatures
        if(first.index() != second.index()){
            return false;
        }
        si::pressure<double> max_p = 2.0*si::standard_pressure;
        si::temperature<double> max_T = 2.0*si::standard_temperature;
        for(si::pressure<double> p = 0.01*si::pascal; p<max_p; p+=(max_p/3.0))
        {
            for(si::temperature<double> T = 0.01*si::kelvin; T<max_T; T+=(max_T/3.0))
            {
                if( first(p, T) != second(p, T)){ return false; }
            }
        }
        return true;
    }
    template<typename T1>
    bool operator!=(const CompletedStateField<T1>& first, const CompletedStateField<T1>& second)
    {
        return !(first==second);
    }

}}

double test_ideal_gas_law_completed(const si::pressure<double> p, const si::temperature<double> T)
{
    return (si::mole*si::universal_gas_constant*T/p/si::liter);
}