#pragma once

#include <units/si.hpp>


namespace compound {
namespace phase {

    struct LinearPressureBoundary
    {
        Polynomial<float,0,1> kelvin_to_pascal;
        // constant constructor
        constexpr LinearPressureBoundary(const si::pressure<double> pressure):
            kelvin_to_pascal(float(pressure/si::pressure<double>))
        {

        }
        constexpr LinearPressureBoundary(const float intercept, const float slope):
            kelvin_to_pascal()
        {
            kelvin_to_pascal[0] = intercept;
            kelvin_to_pascal[1] = slope;
        }
        constexpr si::pascal<double> operator()(const si::temperature<double> temperature)
        {
            return pascal_to_kelvin(temperature / si::kelvin) * si::pascal;
        }
    };

    constexpr LinearPressureBoundary unbounded_low_pressure()
    {
        const oo = std::numeric_limits<float>::max();
        return LinearPressureBoundary(Polynomial<float,0,1>(-oo));
    }

    constexpr LinearPressureBoundary unbounded_high_pressure()
    {
        const oo = std::numeric_limits<float>::max();
        return LinearPressureBoundary(Polynomial<float,0,1>(oo));
    }

    constexpr LinearPressureBoundary pressure_lerp(
        const point<double> sample1, const point<double> sample2
    ){
        float intercept = float(sample1.pressure / si::kelvin);
        float rise = float((sample2.pressure - sample1.pressure) / si::kelvin);
        float run  = float((sample2.temperature - sample1.temperature) / si::pascal);
        return LinearPressureBoundary(intercept, rise/run);
    }

}}