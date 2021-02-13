#pragma once

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "OptionalStateField.hpp"

namespace compound {
namespace field {
    template<typename T1>
	bool operator==(const OptionalStateField<T1> first, const OptionalStateField<T1> second)
	{
	    // std::function does not and cannot have a comparator,
	    // so equality can only be determined pragmatically by sampling at given pressures/temperatures
	    if(first.index() != second.index()){
	        return false;
	    }
	    if((first.index() | second.index()) == 0){
	        return true;
	    }
	    si::pressure max_p = 2.0*si::standard_pressure;
	    si::temperature max_T = 2.0*si::standard_temperature;
	    auto f = std::function<StateSample<T1>(const T1, const si::pressure, const si::temperature)>(
	        [](const T1 x, const si::pressure p, const si::temperature T) -> const StateSample<T1>{
	            return StateSample<T1>(x,p,T); 
	        }
	    );
	    for(si::pressure p = 0.01*si::pascal; p<max_p; p+=(max_p/3.0))
	    {
	        for(si::temperature T = 0.01*si::kelvin; T<max_T; T+=(max_T/3.0))
	        {
	            if( first.map_to_constant(p, T, f) != second.map_to_constant(p, T, f)){ return false; }
	        }
	    }
	    return true;
	}

}}

double test_ideal_gas_law_optional(const si::pressure p, const si::temperature T)
{
	return (si::mole*si::universal_gas_constant*T/p/si::liter);
}