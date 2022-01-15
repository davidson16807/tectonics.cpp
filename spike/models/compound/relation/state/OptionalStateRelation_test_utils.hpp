#pragma once

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "OptionalStateRelation.hpp"

namespace compound{
namespace relation{
    template<typename T1>
    bool operator==(const OptionalStateRelation<T1> first, const OptionalStateRelation<T1> second)
    {
        // std::function does not and cannot have a comparator,
        // so equality can only be determined pragmatically by sampling at given pressures/temperatures
        if(first.has_value() != second.has_value())
        {
            return false;
        }
        if((!first.has_value() && !second.has_value()))
        {
            return true;
        }
        si::pressure max_p = 2.0*si::standard_pressure;
        si::temperature max_T = 2.0*si::standard_temperature;
        for(si::pressure p = 0.01*si::pascal; p<max_p; p+=(max_p/3.0))
        {
            for(si::temperature T = 0.01*si::kelvin; T<max_T; T+=(max_T/3.0))
            {
                auto difference = (first(p, T).value() - second(p, T).value()) / typename T1::value_type(1.0);
                if( difference*difference > 1e-4 ){
                    return false;
                }
            }
        }
        return true;
    }
    template<typename T1>
    bool operator!=(const OptionalStateRelation<T1>& first, const OptionalStateRelation<T1>& second)
    {
        return !(first==second);
    }
}}
