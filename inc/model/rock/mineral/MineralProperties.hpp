#pragma once

// C libraries
#include <cstddef> // std::size_t

namespace rock{

    struct MineralMass
    {
        constexpr MineralMass(){}
        template<typename Minerallike>
        float operator() (const std::size_t i, const Minerallike& constituent) const 
        {
            return constituent.mass;
        }
    };

    template<typename MolarMassTable>
    struct MineralMoles
    {
        MolarMassTable molar_masses;
        constexpr MineralMoles(const MolarMassTable& molar_masses):
            molar_masses(molar_masses)
        {}
        template<typename Minerallike>
        float operator() (const std::size_t i, const Minerallike& constituent) const 
        {
            return constituent.mass / molar_masses[i];
        }
    };

    template<typename DensityTable>
    struct MineralVolume
    {
        DensityTable densities;
        constexpr MineralVolume(const DensityTable& densities):
            densities(densities)
        {}
        template<typename Minerallike>
        float operator() (const std::size_t i, const Minerallike& constituent) const 
        {
            return constituent.mass / densities[i];
        }
    };

}

