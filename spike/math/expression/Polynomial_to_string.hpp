#pragma once

#include <string> // string
#include <cmath>  // log10

// in-house libraries
#include "Polynomial.hpp"

namespace math {
    template<typename T, int Plo, int Phi>
    constexpr std::string to_string(const Polynomial<T,Plo,Phi>& p)
    {
        // const std::string exponents("⁰¹²³⁴⁵⁶⁷⁸⁹");
        std::string exponent;
        std::string output;
        for (int i = Plo; i <= Phi; ++i)
        {
            output += std::to_string(p[i]);
            output += i>0? "x" : "";
            output += "^" + std::to_string(i);
            output += i<Phi? " + " : "";
        }
        return output;
    }
}