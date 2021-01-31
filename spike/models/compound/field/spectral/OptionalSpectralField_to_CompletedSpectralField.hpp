#pragma once

// in-house libraries
#include "OptionalSpectralField.hpp"
#include "CompletedSpectralField.hpp"

namespace compound { 
namespace field {
    /*
    Return a CompletedSpectralField<T1> field representing `a`. Underwrite with `b` if `a` is missing a entry.
    */
    template<typename T1>
    constexpr CompletedSpectralField<T1> complete(const OptionalSpectralField<T1> a, const CompletedSpectralField<T1> b)
    {
        return a.value_or(b.value());
    }
}}