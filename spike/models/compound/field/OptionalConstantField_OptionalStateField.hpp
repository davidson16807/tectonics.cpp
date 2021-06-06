#pragma once

// in-house libraries
#include "state/OptionalStateParameters.hpp"
#include "constant/EmptyParameters.hpp"

namespace compound { 
namespace field {

    constexpr OptionalStateParameters aggregate(const OptionalStateParameters a, const EmptyParameters b)
    {
        return a.has_value()? a : std::monostate();
    }
    constexpr OptionalStateParameters aggregate(const EmptyParameters b, const OptionalStateParameters a)
    {
        return a.has_value()? a : std::monostate();
    }

}}


