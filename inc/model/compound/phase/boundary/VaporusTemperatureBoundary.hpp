#pragma once

#include <units/si.hpp>

namespace compound {
namespace phase {

    struct VaporusTemperatureBoundary
    {
        Polynomial<float,0,1> log_pascal_to_inverse_kelvin;
        // constant constructor
        constexpr LinearTemperatureBoundary(const si::temperature<double> temperature):
            log_pascal_to_inverse_kelvin(float(si::temperature<double>/temperature))
        {

        }
        constexpr LinearTemperatureBoundary(const float intercept, const float slope):
            log_pascal_to_inverse_kelvin()
        {
            log_pascal_to_inverse_kelvin[0] = intercept;
            log_pascal_to_inverse_kelvin[1] = slope;
        }
        constexpr si::temperature<double> operator()(const si::pressure<double> pressure)
        {
            return si::kelvin / log_pascal_to_inverse_kelvin(float(std::log(pressure / si::pascal)));
        }
    };

    LinearTemperatureBoundary basic_vaporus(
        const si::specific_energy<double> latent_heat_of_vaporization_at_boiling_point,
        const si::molar_mass<double> molar_mass,
        const point<double> critical
    ){
        const auto Tc = critical.temperature;
        const auto pc = critical.pressure;
        const auto L = latent_heat_of_vaporization_at_boiling_point;
        const auto M = molar_mass;
        const auto Ru = si::universal_gas_constant;
        constexpr auto R = Ru/M;
        constexpr auto m = -(R/L);
        constexpr auto b = 1/Tc - m * std::log(pc/si::pascal);
        return VaporusTemperatureBoundary(b, m);
    }
}}