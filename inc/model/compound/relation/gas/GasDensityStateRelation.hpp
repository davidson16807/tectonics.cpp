#pragma once

// in-house libraries
#include <unit/si.hpp>

namespace compound {
namespace relation {

    /*
    `GasDensityStateRelation` represents the 
    */
    class GasDensityStateRelation
    {
        si::molar_mass<double>  molar_mass;
        si::pressure<double>    critical_pressure;
        si::temperature<double> critical_temperature;
        double                  critical_compressibility;

    public:

        constexpr GasDensityStateRelation(
            const si::molar_mass<double>  molar_mass,
            const si::pressure<double>    critical_pressure,
            const si::temperature<double> critical_temperature,
            const double                  critical_compressibility
        ):
            molar_mass(molar_mass),
            critical_pressure(critical_pressure),
            critical_temperature(critical_temperature),
            critical_compressibility(critical_compressibility)
        {
        }

        constexpr GasDensityStateRelation& operator=(const GasDensityStateRelation other)
        {
            molar_mass = other.molar_mass;
            critical_pressure = other.critical_pressure;
            critical_temperature = other.critical_temperature;
            critical_compressibility = other.critical_compressibility;
            return *this;
        }

        si::density<double> operator()(const si::pressure<double> pressure, const si::temperature<double> temperature) const
        {
            return molar_mass / correlation::get_molar_volume_from_compressibility(pressure, temperature,
                    correlation::estimate_compressibility_factor(pressure, temperature, critical_pressure, critical_temperature, critical_compressibility));
        }

    };

}}

