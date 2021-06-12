#pragma once

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "OptionalConstantField.hpp"

namespace compound{
namespace field{
    template<typename T1>
    bool operator==(const OptionalConstantField<T1> first, const OptionalConstantField<T1> second)
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
        auto difference = (first().value() - second().value()) / T1(1.0);
        if( difference*difference > 1e-4 ){
            return false;
        }
        return true;
    }
    template<typename T1>
    bool operator!=(const OptionalConstantField<T1>& first, const OptionalConstantField<T1>& second)
    {
        return !(first==second);
    }
}}
