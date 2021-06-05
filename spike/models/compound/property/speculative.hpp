#pragma once

// C libraries
#include <math.h>       // log10 

// in-house libraries
#include <units/si.hpp>  // estimate_compressibility_factor
#include <math/lerp.hpp> // lerp

#include "compressibility.hpp"  // estimate_compressibility_factor

namespace compound{
    namespace property
    {

        /*
        NOTE: 
        `compound::property` serves as a "category of primitives" where objects are variables of primitive types.
        More information about this category can be found under `published.hpp`.
        `speculative.hpp` contains only mappings within this category which are not peer reviewed.
        We mostly create these correlations out of desperation when no other peer reviewed derivations exist.
        We store these correlations in a separate file so that we can clearly delineatly what is or is not peer-reviewed,
        and we append them with the prefix `guess_*` (as opposed to `estimate_*`) to warn about their limited use.
        */

        /*
        SOLID/GAS VISCOSITY: 
        In the future, if we were trying to be rigorous, we would use Chapman-Enskog theory to predict 
        the viscosity of a phase of matter. However, reading about that theory left me with the impression that
        I had stumbled into one of the seedy back alleys of physics and was likely to get roughed up by the locals 
        at any minute. I will retreat into the comfort of my toy models for now.

        For now, the relationship we use is really speculative. We don't have the viscosities for many solids:

        Granite has a viscosity on the order of 1e19 Pa*s (Kumagai 1978) and is composed mostly of orthoclase.
        Itô (1987) makes an estimate of 1e20 Pa*s and states gabbro is higher, but on the same order of magnitude.
        Granite is a mix of mostly orthoclase (1e8 Pa*s molten) and silica (1e3 Pa*s molten),
        but since both are a blend of coarse mineral grains it is hard to map the estimate for viscoity as a solid
        back to those of its components. We would ideally like to deal with viscosities of single compounds,
        so that we may compose the estimates for their viscosities back to estimates for the viscosities 
        of complex mineral mixtures.

        Silica is a well known mineral that is composed of a single compound.
        We know glass silica has a viscosity on the order of 1e12 Pa*s, and molten silica has a viscosity 
        on the order of 1e3 Pa*s. However, glass is an amorphous solid and may not be the best representative compound
        for making observations on viscosity.

        Halite is another mineral that is known to behave as a viscous fluid.
        We have estimates of its viscosity anywhere from 1e17-1e21 Pa*s, and its molten form has a viscosity of 
        1 Pa*s. This is unfortunately the best we have to go on. We decide upon 1e17 as a conversion factor since it is
        consistent with estimates for both granite and halite.
        */
        double guess_viscosity_as_liquid_from_viscosity_as_solid(const double viscosity_as_solid)
        {
            return viscosity_as_solid / 1e17;
        }
        double guess_viscosity_as_solid_from_viscosity_as_liquid(const double viscosity_as_liquid)
        {
            return viscosity_as_liquid * 1e17;
        }

        /*
        SOLID/LIQUID DENSITY:
        The following functions map to and from the densities of solids and liquids.
        These should only be used when density cannot be derived by any other peer-reviewed means.
        (i.e. using any function that starts with "estimate_*")

        We assume a linear relation between solid and liquid density.
        We derive the parameters for the relation from a correlation we find 
        between known the examples in our standard `CompoundTable`. 
        The following R code was used to generate it:
        ```
        liquid.density=c(0807.0,1141.0,1101.0,0422.4,1401.0,124.901,0070.8,0681.97,1349.0,1022.0,2180.0,2180.0,2180.0,2800.0,17310.0,9320.0,8020.0)
        solid.density=c(0850.0,1524.0,1562.0,0522.0,1680.7,0187.0,0077.0,0870.0,1354.0,0916.9,2633.0,2685.0,2560.0,3225.0,19300.0,10500.0,8960.0)
        plot(solid.density,liquid.density)
        lm(solid.density ~ liquid.density)
        summary(lm(solid.density ~ liquid.density))
        ```
        */
        namespace 
        {
            const float liquid_solid_density_slope = 84.587f;
            const si::density liquid_solid_density_intercept = 1.112 * si::kilogram/si::meter3;
            constexpr si::density kilogram_per_meter3 = si::kilogram/si::meter3; 
        }

        si::density guess_density_as_solid_from_density_as_liquid(const si::density density_as_liquid)
        {
            return liquid_solid_density_slope * density_as_liquid + liquid_solid_density_intercept;
        }

        si::density guess_density_as_liquid_from_density_as_solid(const si::density density_as_solid)
        {
            return (density_as_solid - liquid_solid_density_intercept) / liquid_solid_density_slope;
        }

        /*
        This is a handwave I wrote while noticing some patterns in known values.
        Acentric factor is a reflection of how asymmetric the molecule is, 
        and asymmetry along an axis increases the degrees of freedom for a molecule,
        so it makes sense there should be a relation between the two properties.
        */
        double guess_molecular_degrees_of_freedom(const double acentric_factor, const double atoms_per_molecule)
        {
            return
              atoms_per_molecule == 1u? 3.0
            : atoms_per_molecule == 2u? 5.0
            : math::mix(5.0, 6.8, math::linearstep(0.2, 0.345, acentric_factor));
        }
    }
}