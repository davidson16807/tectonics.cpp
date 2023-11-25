#pragma once

#include <unit/si.hpp>

namespace compound {
namespace phase {

    struct LinearTemperatureBoundary
    {
        Polynomial<float,0,1> pascal_to_kelvin;
        // constant constructor
        constexpr LinearTemperatureBoundary(const si::temperature<double> temperature):
            pascal_to_kelvin(float(temperature/si::temperature<double>))
        {

        }
        constexpr LinearPressureBoundary(const float intercept, const float slope):
            pascal_to_kelvin()
        {
            pascal_to_kelvin[0] = intercept;
            pascal_to_kelvin[1] = slope;
        }
        constexpr si::temperature<double> operator()(const si::pressure<double> pressure)
        {
            return pascal_to_kelvin(float(pressure / si::pascal)) * si::kelvin;
        }
    };

    constexpr LinearTemperatureBoundary unbounded_low_temperature()
    {
        const oo = std::numeric_limits<float>::max();
        return LinearTemperatureBoundary(-oo*si::pascal);
    }

    constexpr LinearTemperatureBoundary unbounded_high_temperature()
    {
        const oo = std::numeric_limits<float>::max();
        return LinearTemperatureBoundary(oo*si::pascal);
    }

    constexpr LinearTemperatureBoundary temperature_lerp(
        const point<double> sample1, const point<double> sample2
    ){
        float intercept = float(sample1.temperature / si::kelvin);
        float rise = float((sample2.temperature - sample1.temperature) / si::kelvin);
        float run  = float((sample2.pressure - sample1.pressure) / si::pascal);
        return LinearTemperatureBoundary(intercept, rise/run);
    }

}}