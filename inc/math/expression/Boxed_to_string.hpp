#pragma once

#include <string> // string

// in-house libraries
#include "Boxed.hpp"

namespace math {
    template<typename T, typename F>
    constexpr std::string to_string(const Boxed<T,F>& p)
    {
        const T oo = std::numeric_limits<T>::max();
        std::string output;
        output += p.lo == -oo? "-∞" : std::to_string(p.lo);
        output += " < x ≤ ";
        output += p.hi ==  oo? "∞" : std::to_string(p.hi);
        output += ": ";
        output += to_string(p.f);
        return output;
    }
}