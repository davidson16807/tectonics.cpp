#pragma once

#include <string> // string

// in-house libraries
#include "Railcar.hpp"

namespace math {
    template<typename T, typename F>
    constexpr std::string to_string(const Railcar<T,F>& p)
    {
        std::string output;
        output += p.lo == std::numeric_limits<T>::min()? "-∞" : std::to_string(p.lo);
        output += " < x ≤ ";
        output += p.hi == std::numeric_limits<T>::max()? "∞" : std::to_string(p.hi);
        output += ": ";
        output += to_string(p.f);
        return output;
    }
}