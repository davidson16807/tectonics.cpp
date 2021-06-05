#pragma once

// in-house libraries
#include "OptionalStateField.hpp"
#include "CompletedStateField.hpp"

namespace compound { 
namespace field {
    /*
    Return a CompletedStateField<T1> field representing `a`. Underwrite with `b` if `a` is missing a entry.
    */
    template<typename T1>
    constexpr CompletedStateField<T1> complete(const OptionalStateField<T1> a, const CompletedStateField<T1> b)
    {
        return a.complete(b.value());
    }
}}