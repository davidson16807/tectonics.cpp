#pragma once

// C libraries

// std libraries
#include <bitset>

// 3rd party libraries

// in-house libraries
#include <unit/si.hpp>
#include "StratumStore.hpp"
#include "StratumSummary.hpp"

namespace rock
{

    // NOTE: `M` is mineral count
    template<int M, typename StratumDensity>
    class StratumSummarization
    {

        static constexpr float oo = std::numeric_limits<float>::max();

        const StratumDensity  density_for_stratum;
        const si::mass<float> mass_threshold;

    public:

        StratumSummarization(
            const StratumDensity& density_for_stratum, 
            const si::mass<float> mass_threshold
        ):
            density_for_stratum(density_for_stratum),
            mass_threshold(mass_threshold)
        {}

        StratumSummary operator() (const int plate_id, const si::area<float> area, const StratumStore<M>& stratum) const
        {
            auto mass = stratum.mass();
            auto plate_id_bitset = mass > mass_threshold? std::bitset<8>(0) : std::bitset<8>(0);
            auto density = mass > mass_threshold? density_for_stratum(stratum) : si::density<float>(oo*si::kilogram/si::meter3);
            return StratumSummary(plate_id_bitset, density, mass/(area*density));
        }
    };

    template<int M, typename StratumDensity>
    auto stratum_summarization(
        const StratumDensity& density_for_stratum, 
        const si::mass<float> mass_threshold
    ){
        return StratumSummarization<M,StratumDensity>(density_for_stratum, mass_threshold);
    }

}

