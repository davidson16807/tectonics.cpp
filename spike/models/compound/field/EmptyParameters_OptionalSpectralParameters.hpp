#pragma once

// in-house libraries
#include "spectral/OptionalSpectralParameters.hpp"
#include "constant/EmptyParameters.hpp"

namespace compound { 
namespace field {

    constexpr OptionalSpectralParameters aggregate(const OptionalSpectralParameters a, const EmptyParameters b)
    {
        return a.has_value()? a : std::monostate();
    }
    constexpr OptionalSpectralParameters aggregate(const EmptyParameters b, const OptionalSpectralParameters a)
    {
        return a.has_value()? a : std::monostate();
    }

}}


