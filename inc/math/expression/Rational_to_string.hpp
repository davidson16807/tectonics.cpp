#pragma once

#include <string> // string
#include <cmath>  // log10

// in-house libraries
#include "Polynomial.hpp"
#include "Polynomial_to_string.hpp"

#include "Rational.hpp"

namespace math {
    template<typename T, int Plo, int Phi, int Qlo, int Qhi>
    constexpr std::string to_string(const Rational<T,Plo,Phi,Qlo,Qhi>& r)
    {
        std::string numerator   = to_string(r.p);
        std::string denominator = to_string(r.q);
        std::size_t line_length = std::max(numerator.size(), denominator.size());
        std::string output;
        output += "\r\n";
        output += numerator;
        output += "\r\n";
        for (std::size_t i = 0; i < line_length; ++i)
        {
            output += "-";
        }
        output += "\r\n";
        output += denominator;
        output += "\r\n";
        return output;
    }
}