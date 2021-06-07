#pragma once

// in-house libraries
#include "state/OptionalStateParameters.hpp"
#include "spectral/OptionalSpectralParameters.hpp"

namespace compound { 
namespace field {

    constexpr OptionalSpectralParameters aggregate(const OptionalSpectralParameters a, const OptionalStateParameters b)
    {
        return a.has_value()? a : SpectralParametersAggregate();
    }
    constexpr OptionalSpectralParameters aggregate(const OptionalStateParameters b, const OptionalSpectralParameters a)
    {
        return a.has_value()? a : SpectralParametersAggregate();
    }

}}


