#pragma once

#include <string> // string
#include <cmath>  // log10

// in-house libraries
#include "ArbitraryDegreePolynomial.hpp"

namespace analytic {
    template<typename T, typename I>
    constexpr std::string to_string(const ArbitraryDegreePolynomial<T,I>& p)
    {
        // const std::string exponents("⁰¹²³⁴⁵⁶⁷⁸⁹");
        std::string output;
        for (auto pair1 : p.k)
        {
            I i = pair1.first;
            output += std::to_string(p[i]);
            output += i==0?             "" : "x";
            output += 0<=i&&i<=1?       "" : "^" + std::to_string(i);
            output += pair1!=p.k.end()? "" : " + ";
        }
        return output;
    }
}