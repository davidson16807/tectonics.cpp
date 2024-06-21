#pragma once

// C libraries

// std libraries
#include <bitset>

// 3rd party libraries

// in-house libraries
#include <unit/si.hpp>

namespace rock
{

    template<int M, typename StratumDensity>
    class StratumSummarization
    {
        const StratumDensity   density_for_stratum;
        const int plate_id;
    public:
        StratumSummarization(const StratumDensity& density_for_stratum, const int plate_id):
            density_for_stratum(density_for_stratum),
            plate_id(plate_id)
        {}
        StratumSummary operator() (const si::area<float> area, const StratumStore<M>& stratum) const
        {
            auto density = density_for_stratum(stratum);
            return StratumSummary(std::bitset<8>(1<<plate_id), density, stratum.mass()/density/area);
        }
    };

    template<int M, typename StratumDensity>
    auto stratum_summarization(const StratumDensity& density_for_stratum, const int plate_id){
        return StratumSummarization<M,StratumDensity>(density_for_stratum, plate_id);
    }

}

