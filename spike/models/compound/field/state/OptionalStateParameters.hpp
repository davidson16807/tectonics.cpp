#pragma once

#include "StateParameters.hpp"

namespace compound { 
namespace field{

    using OptionalStateParameters = std::variant<std::monostate, StateParameters, StateParametersAggregate>;

    OptionalStateParameters aggregate(const OptionalStateParameters a, const OptionalStateParameters b) const 
    {
        bool has_a = a.index() == 1;
        bool has_b = b.index() == 1;
        return !has_a? b : !has_b? a : StateParametersAggregate();
    }
}}