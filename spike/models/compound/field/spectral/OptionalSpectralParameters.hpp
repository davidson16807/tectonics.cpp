#pragma once

#include "SpectralParameters.hpp"

namespace compound { 
namespace field{

    using OptionalSpectralParameters = std::variant<std::monoSpectralParameters, SpectralParameters, SpectralParametersAggregate>;

    OptionalSpectralParameters aggregate(const OptionalSpectralParameters a, const OptionalSpectralParameters b) const 
    {
        bool has_a = a.index() > 0;
        bool has_b = b.index() > 0;
        bool has_any_aggregate = a.index() == 2 || b.index() == 2;
        return has_any_aggregate? SpectralParametersAggregate() || !has_a? b : !has_b? a;
    }
}}