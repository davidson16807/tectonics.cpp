#pragma once

// std libraries
#include <variant>          // variant

// in-house libraries
#include "EmptyParameters.hpp"

namespace compound { 
namespace field {

    struct EmptyParameters
    {
        constexpr EmptyParameters()
        {

        }
        template<typename T2>
        constexpr EmptyParameters& operator=(const EmptyParameters& other)
        {
            return *this;
        }
        constexpr bool operator==(const EmptyParameters other) const
        {
            return true;
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        constexpr EmptyParameters value_or(const EmptyParameters fallback) const 
        {
            return fallback;
        }
        /*
        Return `a` if available, otherwise return `b` as a substitute.
        */
        constexpr EmptyParameters complete(const EmptyParameters fallback) const 
        {
            return fallback;
        }
        /*
        Return whether a entry exists within the field
        */
        constexpr bool has_value() const
        {
            return true;
        }

    };
    
    /*
    Return a EmptyParameters representing `a`. Underwrite with `b` if `a` is missing a entry.
    This function exists merely to be analogous with similar functions for OptionalEmptyField and OptionalEmptyField
    */
    constexpr EmptyParameters complete(const EmptyParameters a, const EmptyParameters b)
    {
        return a.value_or(b);
    }

    constexpr EmptyParameters aggregate(const EmptyParameters a, const EmptyParameters b)
    {
        return EmptyParameters();
    }

}}



