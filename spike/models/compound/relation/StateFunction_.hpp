#pragma once

// std libraries
#include <functional>          // function

// in-house libraries
#include <math/lerp.hpp>
#include <models/compound/property/phase.hpp>

namespace compound { 
namespace relation {
    // 7 uses
    relation::StateFunction<int> get_simon_glatzel_phase_function( 
        const si::pressure<double>        p0, /*triple point pressure*/
        const si::temperature<double>     t0, /*triple point temperature*/
        const si::pressure<double>        pc, /*critical point pressure*/
        const si::temperature<double>     tc, /*critical point temperature*/
        const si::specific_energy<double> L,  /*latent heat of vaporization at boiling point*/
        const si::molar_mass<double>      M,  /*molar mass*/
        const si::pressure<double>        a,  /*simon glatzel slope*/
        const si::pressure<double>        b,  /*simon glatzel intercept*/
        const float               c   /*simon glatzel exponent*/
    ){
        return relation::StateFunction<int>(
            [p0, t0, pc, tc, L,  M, a, b, c]
            (const si::pressure<double> p, 
             const si::temperature<double> T)
            {
                return property::get_simon_glatzel_phase(p, T, p0, t0, pc, tc, L,  M, a, b, c);
            }
        );
    }

}}