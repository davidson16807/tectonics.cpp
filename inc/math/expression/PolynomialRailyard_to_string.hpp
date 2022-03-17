#pragma once

#include "PolynomialRailyard.hpp"

#include "PolynomialRailcar_to_string.hpp"

namespace math {

    template<typename T, int Plo, int Phi>
    constexpr std::string to_string(const PolynomialRailyard<T,Plo,Phi>& yard)
    {
        std::string output("\r\n");
        for (std::size_t i=0; i<yard.cars.size(); i++)
        {
            output += to_string(yard.cars[i]);
            output += "\r\n";
        }
        return output;
    }
}
