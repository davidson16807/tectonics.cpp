#pragma once

#include <string> // string

// in-house libraries
#include "Railcar.hpp"

namespace math {

    template<typename T, typename F>
    constexpr std::string to_string(const Railcar<T,F>& p)
    {
        std::string output;
        const T oo (std::numeric_limits<T>::max());
        output += p.lo == -oo? "-∞" : std::to_string(p.lo);
        output += " < x ≤ ";
        output += p.hi == oo? "∞" : std::to_string(p.hi);
        output += ": ";
        output += to_string(p.content);
        return output;
    }

}