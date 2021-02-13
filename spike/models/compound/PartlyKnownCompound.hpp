#pragma once

// std libraries
#include <optional>          // optional

// in-house libraries
#include "phase/gas/PartlyKnownGas.hpp"
#include "phase/liquid/PartlyKnownLiquid.hpp"
#include "phase/solid/PartlyKnownSolid.hpp"

#include "property/published.hpp"


namespace compound
{

    /*
    "BasicPhases" tracks phase ids for phases that commonly occur in compounds.

    Some compounds may have complex phase diagrams with many different 
    phases of solids (for instance, water ice, silicon, or graphite/diamond)
    These phases may each carry different properties, and we want to be able to
    track all these properties if they are available.
    
    However the number of phases differs between compounds,
    so we can't have one attribute within `PartlyKnownCompound` for each phase
    (e.g. `density_as_solid`, `density_as_iceIX`, etc.)
    Since most properties are often reported as a single value for each phase 
    (e.g. density, refractive index) we cannot simply 

    We could get around this problem using polymorphism, 
    and we tried this initially, but we found that it what we can do with the
    `PartlyKnownCompound` class. `PartlyKnownCompound` only has mappings to
    the `CompletedCompound` class, so to have a polymorphic `Water` class
    for instance we would either have to know the compound entirely,
    or we'd have to handwave estimates for it that would pollute the 
    dataset for water that we got from published research, 
    or we'd have to create a duplicate of the `PartlyKnownCompound` 
    class just for that polymorphic class.

    Our solution is to index properties in unordered_maps that are hashed 
    by an id indicating a unique phase for that compound.
    This leaves the compound free to use however many phases it wants.
    For practical considerations we limit phases to the number that 
    can be represented in a uint8, or 256. 

    There are nevertheless phases that frequently occur in compounds,
    namely a basic solid, a liquid, and a gas. So that's why we have `BasicPhases`.
    */
    enum BasicPhases
    {
        supercritical,
        gas,
        liquid,
        solid
    };
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
        si::molar_mass   molar_mass;
        int              atoms_per_molecule;
        si::length       molecular_diameter;
        /* used to determine specific heat capacity 
        from http://hyperphysics.phy-astr.gsu.edu/hbase/Kinetic/shegas.html */
        field::OptionalConstantField<double> molecular_degrees_of_freedom;
        // derived by Klincewicz -> Ihmels -> Tee-Gotoh-Steward1 -> Tee-Gotoh-Steward2
        field::OptionalConstantField<double> acentric_factor;

        // CRITICAL PROPERTIES
        // derived by Klincewicz
        si::pressure     critical_point_pressure;
        si::molar_volume critical_point_volume;
        // derived by Klincewicz -> Ihmels
        si::temperature  critical_point_temperature;
        double           critical_point_compressibility;

        // PHASE TRANSITION PROPERTIES
        // derived by Klincewicz -> Ihmels -> Tee-Gotoh-Steward1 -> Tee-Gotoh-Steward2 -> Pitzer
        field::OptionalConstantField<si::specific_energy>  latent_heat_of_vaporization;
        field::OptionalConstantField<si::specific_energy>  latent_heat_of_fusion;
        field::OptionalConstantField<si::pressure>        triple_point_pressure;
        field::OptionalConstantField<si::temperature>     triple_point_temperature;
        field::OptionalConstantField<si::pressure>        freezing_point_sample_pressure;
        field::OptionalConstantField<si::temperature>     freezing_point_sample_temperature;

        /* 
        OPTIONAL PHASE CHANGE PROPERTIES
        These change the behavior of `get_phase_ids_for_pressures_and_temperatures()`,
        so we include them in `CompletedCompound` rather than map them from 
        attributes within `PartlyKnownCompound` as per usual. 
        Please note the presence/absence of values for these parameters 
        does not indicate missing data. 
        Some compounds are not known to have supercritical phases,
        and some compounds legitimately cannot be modeled by simon glatzel parameters.
        Missing data for these values should be indicated by using the 
        equivalent optional parameters a `PartlyKnownCompound` object
        */
        field::OptionalConstantField<float> simon_glatzel_slope;
        field::OptionalConstantField<float> simon_glatzel_exponent;

        // MISCELLANEOUS PROPERTIES
        field::OptionalSpectralField<si::area> molecular_absorption_cross_section;

        // PHASE PROPERTIES
        phase::PartlyKnownGas gas;
        phase::PartlyKnownLiquid liquid;
        std::vector<phase::PartlyKnownSolid> solids;



        PartlyKnownCompound(
            const si::molar_mass molar_mass,
            const int            atoms_per_molecule,
            const field::OptionalConstantField<si::length>        molecular_diameter,
            const field::OptionalConstantField<double>            molecular_degrees_of_freedom,
            const field::OptionalConstantField<double>            acentric_factor,

            const field::OptionalConstantField<si::pressure>      critical_point_pressure,
            const field::OptionalConstantField<si::molar_volume>  critical_point_volume,
            const field::OptionalConstantField<si::temperature>   critical_point_temperature,
            const field::OptionalConstantField<double>            critical_point_compressibility,

            const field::OptionalConstantField<si::specific_energy> latent_heat_of_vaporization,
            const field::OptionalConstantField<si::specific_energy> latent_heat_of_fusion,
            const field::OptionalConstantField<si::pressure>      triple_point_pressure,
            const field::OptionalConstantField<si::temperature>   triple_point_temperature,
            const field::OptionalConstantField<si::pressure>      freezing_point_sample_pressure,
            const field::OptionalConstantField<si::temperature>   freezing_point_sample_temperature,
            const field::OptionalConstantField<float>             simon_glatzel_slope,
            const field::OptionalConstantField<float>             simon_glatzel_exponent,
            const field::OptionalSpectralField<si::area>          molecular_absorption_cross_section,
            const phase::PartlyKnownGas                    gas,
            const phase::PartlyKnownLiquid                 liquid,
            const std::vector<phase::PartlyKnownSolid>     solids
        ):
            molar_mass                         (molar_mass),
            atoms_per_molecule                 (atoms_per_molecule),
            molecular_diameter                 (0.0*si::meter), // guaranteed to populate within constructor
            molecular_degrees_of_freedom       (molecular_degrees_of_freedom),
            acentric_factor                    (acentric_factor),

            critical_point_pressure            (critical_point_pressure
                .value_or(property::estimate_critical_pressure_from_klincewicz(molar_mass, atoms_per_molecule))),
            critical_point_volume              (critical_point_volume
                .value_or(property::estimate_critical_molar_volume_from_klincewicz(molar_mass, atoms_per_molecule))),
            critical_point_temperature         (0.0*si::kelvin), // guaranteed to populate within constructor
            critical_point_compressibility     (0.0), // guaranteed to populate within constructor

            latent_heat_of_vaporization        (latent_heat_of_vaporization),
            latent_heat_of_fusion              (latent_heat_of_fusion),
            triple_point_pressure              (triple_point_pressure),
            triple_point_temperature           (triple_point_temperature),
            freezing_point_sample_pressure     (freezing_point_sample_pressure),
            freezing_point_sample_temperature  (freezing_point_sample_temperature),
            simon_glatzel_slope                (simon_glatzel_slope),
            simon_glatzel_exponent             (simon_glatzel_exponent),
            molecular_absorption_cross_section (molecular_absorption_cross_section),
            gas                                (gas),
            liquid                             (liquid),
            solids                             (solids)
        {

            // derived by Klincewicz -> Ihmels
            this->critical_point_temperature     = critical_point_temperature     
                .value_or(property::estimate_critical_temperature_from_ihmels(this->critical_point_pressure, this->critical_point_volume));

            // derived by Klincewicz -> Ihmels, then by definition
            this->critical_point_compressibility = critical_point_compressibility 
                .value_or(property::get_critical_compressibility(this->critical_point_pressure, this->critical_point_temperature, this->critical_point_volume));

            // derived by Klincewicz -> Ihmels -> Tee-Gotoh-Steward1 
            this->molecular_diameter = molecular_diameter
                .value_or(property::estimate_molecular_diameter_from_tee_gotoh_steward(this->critical_point_temperature, this->critical_point_pressure));

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
            guess.simon_glatzel_slope               = simon_glatzel_slope               .value_or(fallback.simon_glatzel_slope            );
            guess.simon_glatzel_exponent            = simon_glatzel_exponent            .value_or(fallback.simon_glatzel_exponent         );

            guess.molecular_absorption_cross_section = molecular_absorption_cross_section.value_or(fallback.molecular_absorption_cross_section);

            guess.liquid = liquid.value_or(fallback.liquid);
            guess.gas    = gas   .value_or(fallback.gas);
            for (std::size_t i=0; i<solids.size(); i++){
                guess.solids[i] = guess.solids[i].value_or(fallback.solids[i]);
            }

            return guess;
        }
    };
}
