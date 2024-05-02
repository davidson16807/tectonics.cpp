#pragma once

#include "CrustValues.hpp"

namespace crust
{
    /*
    NOTE: A naïve approach to calculating strata boundary positions would
    require traversing a `Crust` several times, once for each intermixed liquid body.
    Crust has a very large memory footprint, so this would be a very expensive operation.
    However, we only need crust to find a few derived attributes, totaling 4*4=16 bytes.
    This is much better than the 188 bytes that's needed to store a `StratumStore` object.
    If we want to further optimize this in the future we may also use lower preciions types,
    since deriving strata boundary positions does not require much precision. 
    Most attributes are self explanatory, except for maybe `liquid_id_below`.
    "liquid_id_below" is the id of the lightest liquid that is dense enough to settle beneath the stratum.
    If a liquid_id_below is `M`, it indicates that no liquid exists which can settle beneath the stratum.

    I was going to call this `StratumBoundaryPrerequisites`, 
    but the name is long and it suggests the structure only exists for performance reasons.
    While it was created for that reason, 
    I later realized it can also be used to find which pools of liquid mix together and which strata exist contiguously.
    I felt there was a concept that existed in my head but was sorely missing from the model, the "density tower"
    (see image for illustration). On further reflection, I found the structure here conveyed all the information needed 
    to express this concept, and so it was named.
    */
    struct DensityTowerStratum
    {
        float stratum_thickness;
        float overburden_density;
        float overburden_thickness;
        unsigned int liquid_id_below;
    };
    template <std::size_t L, typename Tgrid>
    using DensityTower = CrustValues<DensityTowerStratum, L, Tgrid>;

    template<std::size_t L, typename Tgrid>
    DensityTower<L,Tgrid> make_DensityTower(const Tgrid& grid)
    {
        return DensityTower<L,Tgrid>(grid);
    }
}
