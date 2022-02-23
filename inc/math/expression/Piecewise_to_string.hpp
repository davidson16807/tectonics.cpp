#pragma once

#include <string> // string

// in-house libraries
#include "Piecewise.hpp"

namespace math {
    template<typename T, typename F>
    constexpr std::string to_string(const Piecewise<T,F>& p)
    {
        std::string output;
        output += std::to_string(p.lo);
        output += " < x ≤ ";
        output += std::to_string(p.hi);
        output += ": ";
        output += to_string(p.f);
        return output;
    }
}