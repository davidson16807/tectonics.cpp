#pragma once

#include "PartlyKnownCompound.hpp"

namespace compound
{
    struct StateParameterSamples{
        field::StateParameters solid;   
        field::StateParameters melting; 
        field::StateParameters liquid;  
        field::StateParameters boiling; 
        field::StateParameters gas;     
        field::StateParameters standard;
        field::StateParameters critical_point;

        StateParameterSamples(const PartlyKnownCompound& guess) :
            standard(si::standard_pressure, si::standard_temperature),
            critical_point(guess.critical_point_pressure, guess.critical_point_temperature)
        {
            field::OptionalStateParameters optional_gas_sample;
            field::OptionalStateParameters optional_boiling_sample;
            field::OptionalStateParameters optional_liquid_sample;
            field::OptionalStateParameters optional_melting_sample;
            field::OptionalStateParameters optional_solid_sample;

            // CALCULATE PRESSURE/TEMPERATURE SAMPLE POINTS
            if (guess.freezing_point_sample_pressure == guess.boiling_point_sample_pressure)
            {
                optional_liquid_sample = optional_liquid_sample.value_or(
                    []( si::pressure freezing_point_sample_pressure, si::temperature freezing_point_sample_temperature, 
                        si::pressure boiling_point_sample_pressure, si::temperature boiling_point_sample_temperature)
                    {
                        if (freezing_point_sample_temperature < si::standard_temperature && 
                                                                si::standard_temperature < boiling_point_sample_temperature)
                        {
                            return field::OptionalStateParameters(
                                field::StateParameters(
                                    freezing_point_sample_pressure, 
                                    si::standard_temperature
                                )
                            );
                        }
                        else
                        {
                            return field::OptionalStateParameters(
                                field::StateParameters(
                                    freezing_point_sample_pressure, 
                                    (freezing_point_sample_temperature + boiling_point_sample_temperature) / 2.0
                                )
                            );
                        }
                    },
                    guess.freezing_point_sample_pressure,
                    guess.freezing_point_sample_temperature,
                    guess.boiling_point_sample_pressure,
                    guess.boiling_point_sample_temperature
                );
            }

            if (guess.freezing_point_sample_temperature.has_value())
            {
                field::StateParameters sample(
                    guess.freezing_point_sample_pressure.complete(si::standard_pressure), 
                    guess.freezing_point_sample_temperature.complete(si::standard_temperature)
                );
                if (sample.pressure == si::standard_pressure)
                {
                    if (si::standard_temperature < sample.temperature)
                    {
                        optional_solid_sample = optional_solid_sample.complete(standard);
                    }
                    if (sample.temperature < si::standard_temperature)
                    {
                        optional_liquid_sample = optional_liquid_sample.complete(standard);
                    }
                }
                optional_solid_sample   = optional_solid_sample.complete(sample);
                optional_melting_sample = optional_melting_sample.complete(sample);
                optional_liquid_sample  = optional_liquid_sample.complete(sample);
            }

            if (guess.boiling_point_sample_temperature.has_value())
            {
                field::StateParameters sample(
                    guess.boiling_point_sample_pressure.complete(si::standard_pressure), 
                    guess.boiling_point_sample_temperature.complete(si::standard_temperature)
                );
                if (sample.pressure == si::standard_pressure)
                {
                    if (si::standard_temperature < sample.temperature)
                    {
                        optional_solid_sample = optional_solid_sample.complete(standard);
                    }
                    if (sample.temperature < si::standard_temperature)
                    {
                        optional_liquid_sample = optional_liquid_sample.complete(standard);
                    }
                }
                optional_liquid_sample  = optional_liquid_sample.complete(sample);
                optional_boiling_sample = optional_boiling_sample.complete(sample);
                optional_gas_sample     = optional_gas_sample.complete(sample);
            }

            if (guess.triple_point_temperature.has_value() && guess.triple_point_pressure.has_value())
            {
                field::StateParameters sample(
                    guess.triple_point_pressure.complete(si::standard_pressure), 
                    guess.triple_point_temperature.complete(si::standard_temperature)
                );
                optional_solid_sample   = optional_solid_sample.complete(sample);
                optional_melting_sample = optional_melting_sample.complete(sample);
                optional_liquid_sample  = optional_liquid_sample.complete(sample);
                optional_boiling_sample = optional_boiling_sample.complete(sample);
                optional_gas_sample     = optional_gas_sample.complete(sample);
            }

            solid   = optional_solid_sample.complete(standard);
            melting = optional_melting_sample.complete(standard);
            liquid  = optional_liquid_sample.complete(critical_point);
            boiling = optional_boiling_sample.complete(critical_point);
            gas     = optional_gas_sample.complete(critical_point);
        }
    };

}