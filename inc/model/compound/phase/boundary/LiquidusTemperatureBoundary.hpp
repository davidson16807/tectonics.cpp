#pragma once

#include <unit/si.hpp>


namespace compound {
namespace phase {

    struct LiquidusTemperatureBoundary
    {
        point<double> reference;
        si::temperature<double> scale;
        double inverse_exponent;
        // constant constructor
        constexpr LinearTemperatureBoundary(
            const point<double> reference, 
            const si::pressure<double> scale, 
            const double inverse_exponent
        ):
            reference(reference),
            scale(scale),
            inverse_exponent(inverse_exponent)
        {

        }
        constexpr si::temperature<double> operator()(const si::pressure<double> pressure)
        {
            return reference.temperature * 
                std::pow(
                    std::max((pressure-reference.pressure)/scale + 1.0, 0.0), 
                    1.0/inverse_exponent);
        }
    };

    LinearTemperatureBoundary basic_liquidus(
        const point<double> triple, 
        const point<double> freezing
    ){
        return LiquidusTemperatureBoundary(triple, 
            (freezing.pressure-triple.pressure)/((freezing.temperature/triple.temperature)-1.0), 
            1.0);
    }
    LinearTemperatureBoundary simon_glatzel_liquidus(
        const point<double> reference, 
        const si::pressure<double> scale, 
        const double inverse_exponent
    ){
        return LiquidusTemperatureBoundary(reference, scale, inverse_exponent);
    }
}}