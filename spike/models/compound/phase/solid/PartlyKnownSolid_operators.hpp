#pragma once

// C libraries
#include "math.h"

// in-house libraries
#include <lerp.hpp>

#include <models/compound/property/elasticity.hpp>
#include <models/compound/property/strength.hpp>

#include "PartlyKnownCompound.hpp"

namespace compound
{
    
    
    /*
    Return a `PartlyKnownCompound` that has the properties of `known` where present, 
    in addition to properties that can derived from the properties of `known`. The function is idempotent.
    The function can be thought of as the traversal of the category defined within the `property` namespace.
    */
    PartlyKnownCompound infer(const PartlyKnownCompound& known){
        // copy what you do know
        PartlyKnownCompound guess = known;

        const si::pressure p = si::standard_pressure;
        const si::temperature T = si::standard_temperature;


        if (known.tensile_yield_strength.has_value())
        {
            guess.shear_yield_strength = known.shear_yield_strength.value_or(
                guess.tensile_yield_strength.map([](si::pressure S){ 
                    return property::get_shear_yield_strength_from_tensile_yield_strength(S); 
                })
            );
        }
        else if (known.shear_yield_strength.has_value())
        {
            guess.tensile_yield_strength = known.tensile_yield_strength.value_or(
                guess.shear_yield_strength.map([](si::pressure S){ 
                    return property::get_tensile_yield_strength_from_shear_yield_strength(S); 
                })
            );
        }
        else if (known.compressive_yield_strength.has_value())
        {
            // no correlation researched
        }
        else 
        {
            /* 
            If none of the yield strengths are known, the most common explanation 
            is that the material is too brittle to be measured.
            So in this case we assume yield strength equal to the ultimate strength equivalent
            */
            guess.tensile_yield_strength = guess.tensile_yield_strength.value_or(guess.tensile_fracture_strength);
            guess.compressive_yield_strength = guess.compressive_yield_strength.value_or(guess.compressive_fracture_strength);
            guess.shear_yield_strength = guess.shear_yield_strength.value_or(guess.shear_fracture_strength);
        }

        /* 
        Standardize on bulk and tensile modulus since they seem to be reported most often,
        then use those two to calculate the remaining modulii.
        To reduce the number of cases, consider only scenarios where bulk, tensile, shear, and lame are present, 
        since again, those are reported often.
        */
        if (known.bulk_modulus.has_value() && known.shear_modulus.has_value())
        {
            guess.tensile_modulus = guess.tensile_modulus.map([](si::pressure K, si::pressure G){ return get_lame_from_bulk_and_shear(K,G); }, known.bulk_modulus, known.shear_modulus);
        }
        else if (known.bulk_modulus.has_value() && known.lame_parameter.has_value())
        {
        }
        else if (known.tensile_modulus.has_value() && known.shear_modulus.has_value())
        {
        }
        else if (known.tensile_modulus.has_value() && known.lame_parameter.has_value())
        {
        }
        else if (known.shear_modulus.has_value() && known.lame_parameter.has_value())
        {
        }

        if (guess.bulk_modulus.has_value() && guess.tensile_modulus.has_value())
        {
            get_lame_from_bulk_and_tensile
            get_shear_from_bulk_and_tensile
            get_poisson_from_bulk_and_tensile
            get_pwave_from_bulk_and_tensile
        }

        get_tensile_from_bulk_and_shear
        get_lame_from_bulk_and_shear
        get_poisson_from_bulk_and_shear
        get_pwave_from_bulk_and_shear
        get_tensile_from_bulk_and_poisson
        get_lame_from_bulk_and_poisson
        get_shear_from_bulk_and_poisson
        get_pwave_from_bulk_and_poisson
        get_tensile_from_bulk_and_pwave
        get_lame_from_bulk_and_pwave
        get_shear_from_bulk_and_pwave
        get_poisson_from_bulk_and_pwave
        get_bulk_from_tensile_and_lame
        get_shear_from_tensile_and_lame
        get_poisson_from_tensile_and_lame
        get_pwave_from_tensile_and_lame
        get_bulk_from_tensile_and_shear
        get_lame_from_tensile_and_shear
        get_poisson_from_tensile_and_shear
        get_pwave_from_tensile_and_shear
        get_bulk_from_tensile_and_poisson
        get_lame_from_tensile_and_poisson
        get_shear_from_tensile_and_poisson
        get_pwave_from_tensile_and_poisson
        get_bulk1_from_tensile_and_pwave
        get_lame1_from_tensile_and_pwave
        get_shear1_from_tensile_and_pwave
        get_poisson1_from_tensile_and_pwave
        get_bulk2_from_tensile_and_pwave
        get_lame2_from_tensile_and_pwave
        get_shear2_from_tensile_and_pwave
        get_poisson2_from_tensile_and_pwave
        get_bulk_from_lame_and_shear
        get_tensile_from_lame_and_shear
        get_poisson_from_lame_and_shear
        get_pwave_from_lame_and_shear
        get_bulk_from_lame_and_poisson
        get_tensile_from_lame_and_poisson
        get_shear_from_lame_and_poisson
        get_pwave_from_lame_and_poisson
        get_bulk_from_lame_and_pwave
        get_tensile_from_lame_and_pwave
        get_shear_from_lame_and_pwave
        get_poisson_from_lame_and_pwave
        get_bulk_from_shear_and_poisson
        get_tensile_from_shear_and_poisson
        get_lame_from_shear_and_poisson
        get_pwave_from_shear_and_poisson
        get_bulk_from_shear_and_pwave
        get_tensile_from_shear_and_pwave
        get_lame_from_shear_and_pwave
        get_poisson_from_shear_and_pwave
        get_bulk_from_poisson_and_pwave
        get_tensile_from_poisson_and_pwave
        get_lame_from_poisson_and_pwave
        get_shear_from_poisson_and_pwave

        return guess;
    }

}