#pragma once

// std libraries
#include <vector>            // vector
#include <optional>          // optional

// in-house libraries
#include "field/state/CompletedStateField.hpp"
#include "field/spectral/CompletedSpectralField.hpp"

#include "phase/gas/CompletedGas.hpp"
#include "phase/liquid/CompletedLiquid.hpp"
#include "phase/solid/CompletedSolid.hpp"

namespace compound
{
    /*
    `CompletedCompound` objects store all the properties of a given set of compounds 
     needed to express all aspects of their behavior in the model for all phases.
    */
    struct CompletedCompound
    {
        // MOLECULAR PROPERTIES
        si::molar_mass   molar_mass;
        int              atoms_per_molecule;
        si::length       molecular_diameter;
        /* used to determine specific heat capacity 
        from http://hyperphysics.phy-astr.gsu.edu/hbase/Kinetic/shegas.html */
        double molecular_degrees_of_freedom;
        // derived by Klincewicz -> Ihmels -> Tee-Gotoh-Steward1 -> Tee-Gotoh-Steward2
        double acentric_factor;

        // CRITICAL PROPERTIES
        // derived by Klincewicz
        si::pressure     critical_point_pressure;
        si::molar_volume critical_point_volume;
        // derived by Klincewicz -> Ihmels
        si::temperature  critical_point_temperature;
        double           critical_point_compressibility;

        // PHASE TRANSITION PROPERTIES
        // derived by Klincewicz -> Ihmels -> Tee-Gotoh-Steward1 -> Tee-Gotoh-Steward2 -> Pitzer
        si::specific_energy latent_heat_of_vaporization;
        si::specific_energy latent_heat_of_fusion;
        si::pressure        triple_point_pressure;
        si::temperature     triple_point_temperature;
        si::pressure        freezing_point_sample_pressure;
        si::temperature     freezing_point_sample_temperature;
        si::pressure        boiling_point_sample_pressure;
        si::temperature     boiling_point_sample_temperature;

        field::CompletedStateField<int> phase;

        // generic 
        field::CompletedSpectralField<si::area> molecular_absorption_cross_section;

        // PHASE PROPERTIES
        phase::CompletedGas gas;
        phase::CompletedLiquid liquid;
        std::vector<phase::CompletedSolid> solids;

        CompletedCompound(
            const si::molar_mass molar_mass,
            const int            atoms_per_molecule,
            const si::length        molecular_diameter,
            const double            molecular_degrees_of_freedom,
            const double            acentric_factor,

            const si::pressure      critical_point_pressure,
            const si::molar_volume  critical_point_volume,
            const si::temperature   critical_point_temperature,
            const double            critical_point_compressibility,

            const si::specific_energy  latent_heat_of_vaporization,
            const si::specific_energy  latent_heat_of_fusion,
            const si::pressure         triple_point_pressure,
            const si::temperature      triple_point_temperature,
            const si::pressure         freezing_point_sample_pressure,
            const si::temperature      freezing_point_sample_temperature,
            const si::pressure         boiling_point_sample_pressure,
            const si::temperature      boiling_point_sample_temperature,
            const field::CompletedStateField<int> phase,
            const field::CompletedSpectralField<si::area> molecular_absorption_cross_section,
            const phase::CompletedGas                     gas,
            const phase::CompletedLiquid                  liquid,
            const std::vector<phase::CompletedSolid>      solids
        ):
            molar_mass                         (molar_mass),
            atoms_per_molecule                 (atoms_per_molecule),
            molecular_diameter                 (molecular_diameter), 
            molecular_degrees_of_freedom       (molecular_degrees_of_freedom),
            acentric_factor                    (acentric_factor),

            critical_point_pressure            (critical_point_pressure),
            critical_point_volume              (critical_point_volume),
            critical_point_temperature         (critical_point_temperature), 
            critical_point_compressibility     (critical_point_compressibility), 

            latent_heat_of_vaporization        (latent_heat_of_vaporization),
            latent_heat_of_fusion              (latent_heat_of_fusion),
            triple_point_pressure              (triple_point_pressure),
            triple_point_temperature           (triple_point_temperature),
            freezing_point_sample_pressure     (freezing_point_sample_pressure),
            freezing_point_sample_temperature  (freezing_point_sample_temperature),
            boiling_point_sample_pressure      (boiling_point_sample_pressure),
            boiling_point_sample_temperature   (boiling_point_sample_temperature),
            phase                              (phase),
            molecular_absorption_cross_section (molecular_absorption_cross_section),
            gas                                (gas),
            liquid                             (liquid),
            solids                             (solids)
        {
        }
    };
}
