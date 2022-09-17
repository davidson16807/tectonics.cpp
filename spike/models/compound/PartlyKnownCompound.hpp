#pragma once

// std libraries
#include <vector>            // vector

// in-house libraries
#include "field/constant/OptionalConstantField.hpp"
#include "field/state/OptionalStateField.hpp"
#include "field/spectral/OptionalSpectralField.hpp"

#include "phase/gas/PartlyKnownGas.hpp"
#include "phase/liquid/PartlyKnownLiquid.hpp"
#include "phase/solid/PartlyKnownSolid.hpp"

#include "property/published.hpp"

namespace compound
{

    /*
    `PartlyKnownCompound` stores all the properties of a given compound
    for which some data may be missing. 

    Some context:

    The model we are constructing is meant to work over a very wide range of '
    compounds, temperatures, and pressures, so ideally we want data for every 
    chemical property of every phase of every compound as a function of 
    temperature and pressure.

    We may not be able to reach this goal, but we do not wish to preclude it.

    There are a few problem we encounter while reaching this goal.
    First, there are huge gaps of data in published research.
    Second, even if there is extensive published research,
    the API users may not have time to enter all the published research,
    and it may not be necessary for them to enter everything. 
    The API user should be able to enter chemical properties as he sees fit,
    and then expand upon these properties if needed.
    For instance, he may start with describing a single representative value,
    then expand to a function describing a property's relationship to 
    temperature and pressure.

    Since the model is not used for making peer reviewed predictions,
    we allow ourselves to handwave values for properties that are poorly known, 
    either by extrapolating from known chemical properties,
    or borrowing from known properties of other compounds.
    
    However, we have taken the time to compile a high quality dataset of the 
    properties we do know, so we want to make sure the properties that we
    handwave do not wind up polluting this dataset. 

    This is where `PartlyKnownCompound` comes in. 
    Unlike, `CompletedCompound`, `PartlyKnownCompound` is missing data 
    and cannot be directly used within the model.
    Instead, it must be explicitly mapped to an `CompletedCompound` before use.
    This mapping requires providing an existing `CompletedCompound` to
    act as a template, which underwrites properties that are not known 
    so that we can guarantee the result of the map will at least have 
    estimated values for all properties needed.
    This allows us to separate our high quality dataset while still allowing 
    us to handwave missing properties for use within the model. 
    It also allows us to clearly document the techniques we are using 
    to handwave missing properties, in case we should happen on better methods 
    in the future, and it prevents errors that might be introduced from 
    manually entering estimates.
    */
    struct PartlyKnownCompound
    {

        // MOLECULAR PROPERTIES
        si::molar_mass<double>   molar_mass;
        unsigned int              atoms_per_molecule;
        si::length<double>       molecular_diameter;
        /* used to determine specific heat capacity 
        from http://hyperphysics.phy-astr.gsu.edu/hbase/Kinetic/shegas.html */
        field::OptionalConstantField<double> molecular_degrees_of_freedom;
        // derived by Klincewicz -> Ihmels -> Tee-Gotoh-Steward1 -> Tee-Gotoh-Steward2
        field::OptionalConstantField<double> acentric_factor;

        // CRITICAL PROPERTIES
        // derived by Klincewicz
        si::pressure<double>     critical_point_pressure;
        si::molar_volume<double> critical_point_volume;
        // derived by Klincewicz -> Ihmels
        si::temperature<double>  critical_point_temperature;
        double           critical_point_compressibility;

        // PHASE TRANSITION PROPERTIES
        // derived by Klincewicz -> Ihmels -> Tee-Gotoh-Steward1 -> Tee-Gotoh-Steward2 -> Pitzer
        field::OptionalConstantField<si::specific_energy<double>> latent_heat_of_vaporization;
        field::OptionalConstantField<si::specific_energy<double>> latent_heat_of_fusion;
        field::OptionalConstantField<si::pressure<double>>        triple_point_pressure;
        field::OptionalConstantField<si::temperature<double>>     triple_point_temperature;
        field::OptionalConstantField<si::pressure<double>>        freezing_point_sample_pressure;
        field::OptionalConstantField<si::temperature<double>>     freezing_point_sample_temperature;
        field::OptionalConstantField<si::pressure<double>>        boiling_point_sample_pressure;
        field::OptionalConstantField<si::temperature<double>>     boiling_point_sample_temperature;

        field::OptionalStateField<int> phase;

        // MISCELLANEOUS PROPERTIES
        relation::OptionalStateRelation<relation::PolynomialRailyardRelation<si::wavenumber<double>,si::area<double>,0,1>> molecular_absorption_cross_section;
        // field::OptionalSpectralField<si::area<double>> molecular_absorption_cross_section;

        // PHASE PROPERTIES
        phase::PartlyKnownGas gas;
        phase::PartlyKnownLiquid liquid;
        std::vector<phase::PartlyKnownSolid> solids;

        PartlyKnownCompound(
            const si::molar_mass<double> molar_mass,
            const unsigned int            atoms_per_molecule,
            const field::OptionalConstantField<si::length<double>>        molecular_diameter,
            const field::OptionalConstantField<double>            molecular_degrees_of_freedom,
            const field::OptionalConstantField<double>            acentric_factor,

            const field::OptionalConstantField<si::pressure<double>>      critical_point_pressure,
            const field::OptionalConstantField<si::molar_volume<double>>  critical_point_volume,
            const field::OptionalConstantField<si::temperature<double>>   critical_point_temperature,
            const field::OptionalConstantField<double>            critical_point_compressibility,

            const field::OptionalConstantField<si::specific_energy<double>> latent_heat_of_vaporization,
            const field::OptionalConstantField<si::specific_energy<double>> latent_heat_of_fusion,
            const field::OptionalConstantField<si::pressure<double>>      triple_point_pressure,
            const field::OptionalConstantField<si::temperature<double>>   triple_point_temperature,
            const field::OptionalConstantField<si::pressure<double>>      freezing_point_sample_pressure,
            const field::OptionalConstantField<si::temperature<double>>   freezing_point_sample_temperature,
            const field::OptionalConstantField<si::pressure<double>>      boiling_point_sample_pressure,
            const field::OptionalConstantField<si::temperature<double>>   boiling_point_sample_temperature,
            const field::OptionalStateField<int>                  phase,
            const relation::OptionalStateRelation<relation::PolynomialRailyardRelation<si::wavenumber<double>,si::area<double>,0,1>> molecular_absorption_cross_section,
            const phase::PartlyKnownGas                           gas,
            const phase::PartlyKnownLiquid                        liquid,
            const std::vector<phase::PartlyKnownSolid>            solids
        ):
            molar_mass                         (molar_mass),
            atoms_per_molecule                 (atoms_per_molecule),
            molecular_diameter                 (0.0*si::meter), // guaranteed to populate within constructor
            molecular_degrees_of_freedom       (molecular_degrees_of_freedom),
            acentric_factor                    (acentric_factor),

            critical_point_pressure            (critical_point_pressure
                .complete(property::estimate_critical_pressure_from_klincewicz(molar_mass, atoms_per_molecule))),
            critical_point_volume              (critical_point_volume
                .complete(property::estimate_critical_molar_volume_from_klincewicz(molar_mass, atoms_per_molecule))),
            critical_point_temperature         (0.0*si::kelvin), // guaranteed to populate within constructor
            critical_point_compressibility     (0.0), // guaranteed to populate within constructor

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

            // derived by Klincewicz -> Ihmels
            this->critical_point_temperature     = critical_point_temperature     
                .complete(property::estimate_critical_temperature_from_ihmels(this->critical_point_pressure, this->critical_point_volume));

            // derived by Klincewicz -> Ihmels, then by definition
            this->critical_point_compressibility = critical_point_compressibility 
                .complete(property::get_critical_compressibility(this->critical_point_pressure, this->critical_point_temperature, this->critical_point_volume));

            // derived by Klincewicz -> Ihmels -> Tee-Gotoh-Steward1 
            this->molecular_diameter = molecular_diameter
                .complete(property::estimate_molecular_diameter_from_tee_gotoh_steward(this->critical_point_temperature, this->critical_point_pressure));

        }


        /*
        Return a `PartlyKnownCompound` that has the properties of `known` where present, otherwise substitute with properties of `fallback`
        */
        PartlyKnownCompound value_or(const PartlyKnownCompound& fallback) const {
            // copy what you do know
            PartlyKnownCompound guess = *this;

            guess.molecular_degrees_of_freedom      = molecular_degrees_of_freedom      .value_or(fallback.molecular_degrees_of_freedom   );
            guess.acentric_factor                   = acentric_factor                   .value_or(fallback.acentric_factor                );

            guess.latent_heat_of_vaporization       = latent_heat_of_vaporization       .value_or(fallback.latent_heat_of_vaporization    );
            guess.latent_heat_of_fusion             = latent_heat_of_fusion             .value_or(fallback.latent_heat_of_fusion          );
            guess.triple_point_pressure             = triple_point_pressure             .value_or(fallback.triple_point_pressure          );
            guess.triple_point_temperature          = triple_point_temperature          .value_or(fallback.triple_point_temperature       );
            guess.freezing_point_sample_pressure    = freezing_point_sample_pressure    .value_or(fallback.freezing_point_sample_pressure );
            guess.freezing_point_sample_temperature = freezing_point_sample_temperature .value_or(fallback.freezing_point_sample_temperature);
            guess.boiling_point_sample_pressure     = boiling_point_sample_pressure     .value_or(fallback.boiling_point_sample_pressure  );
            guess.boiling_point_sample_temperature  = boiling_point_sample_temperature  .value_or(fallback.boiling_point_sample_temperature);

            // NOTE: we do not populate the `phase` function with fallbacks, 
            // since the phase function of the fallback may refer to indices that do not exist for the compound
            // We instead guarantee that the phase function is always populated with a default upon the call to complete().

            guess.molecular_absorption_cross_section = molecular_absorption_cross_section.value_or(fallback.molecular_absorption_cross_section);

            guess.liquid = liquid.value_or(fallback.liquid);
            guess.gas    = gas   .value_or(fallback.gas);
            guess.solids[0] = solids[0].value_or(fallback.solids[0]);
            /*
            NOTE: We only perform `value_or()` on the first phase.
            This is done foremost to preserve the associativity of `value_or()`.
            If the api user wants to populate the remaining solids for the compound, 
            they would be better served to propagate properties from the first phase, 
            since the remaining phases are likely to have more in common with the first phase of the same compound.
            */

            return guess;
        }
    };
}
