#pragma once

// in-house libraries
#include <units/si.hpp>

namespace compound {
namespace property {
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
    namespace BasicPhases
    {
        const int supercritical = -3;
        const int vapor = -2;
        const int liquid = -1;
        const int solid = 0;
    }

    /*
    `get_basic_phase` returns the phase of a basic phase diagram given a set of parameters 
    that should be applicable to virtually any solid/liquid/vapor phase diagram.
    It models boiling/sublimation points using the Clapeyron equation,
    and a linear relation for melting points.
    */
    constexpr int get_basic_phase(
        si::pressure<double>        p,  // current pressure
        si::temperature<double>     T,  // current temperature
        si::pressure<double>        p0, // triple point pressure
        si::temperature<double>     T0, // triple point temperature
        si::pressure<double>        pc, // critical point pressure
        si::temperature<double>     Tc, // critical point temperature
        si::pressure<double>        pf, // freezing point pressure
        si::temperature<double>     Tf, // freezing point temperature
        si::specific_energy<double> L,  // latent heat of vaporization at boiling point
        si::molar_mass<double>      M   // molar mass
    ){
        const auto Ru = si::universal_gas_constant;
        auto R = Ru/M;
        auto m = -(R/L);
        auto b = 1/Tc - m * std::log(pc/si::pascal);
        auto A = (Tf-T0) / (pf-p0);
        if (T > Tc && p > pc)
        {
            return BasicPhases::supercritical;
        }
        else if ( T > 1/(m*std::log(p/si::pascal)+b) )
        {
            return BasicPhases::vapor;
        }
        else if ( T > T0 + A*p ) // linear relation, for when simon glatzel exponent can't be determined
        {
            return BasicPhases::liquid;
        }
        else
        {
            return BasicPhases::solid;
        }
    }

    /*
    `get_simon_glatzel_phase` returns the phase of a basic phase diagram given a set of parameters 
    that should be applicable to virtually any solid/liquid/vapor phase diagram.
    It models boiling/sublimation points using the Clapeyron equation,
    and melting points using the empirical Simon-Glatzel equation.
    */
    constexpr int get_simon_glatzel_phase(
        const si::pressure<double>        p,  // current pressure
        const si::temperature<double>     T,  // current temperature
        const si::pressure<double>        p0, // triple point pressure
        const si::temperature<double>     T0, // triple point temperature
        const si::pressure<double>        pc, // critical point pressure
        const si::temperature<double>     Tc, // critical point temperature
        const si::specific_energy<double> L,  // latent heat of vaporization at boiling point
        const si::molar_mass<double>      M,  // molar mass
        const si::pressure<double>        a,  // simon glatzel slope
        const si::pressure<double>        b,  // simon glatzel intercept
        const double              c   // simon glatzel exponent
    ){
        const auto Ru = si::universal_gas_constant;
        auto R = Ru/M;
        auto slope = -(R/L);
        auto intercept = 1.0/Tc - slope * std::log(pc/si::pascal);
        if (T > Tc && p > pc)
        {
            return BasicPhases::supercritical;
        }
        else if ( T > 1/(slope*std::log(p/si::pascal)+intercept) )
        {
            return BasicPhases::vapor;
        }
        else if ( T >  T0 * std::pow(std::max((p-p0)/a + 1.0, 0.0), 1.0/c) )
        {
            return BasicPhases::liquid;
        }
        else
        {
            return BasicPhases::solid;
        }
    }
}}
