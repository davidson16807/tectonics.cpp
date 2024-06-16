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
        const StratumDensity   density;
        const int plate_id;
    public:
        StratumSummarization(const StratumDensity& density, const int plate_id):
            density(density),
            plate_id(plate_id)
        {}
        StratumSummary operator() (const si::area<float> area, const StratumStore<M>& stratum) const
        {
            auto density_ = density(stratum);
            return StratumSummary(std::bitset<8>(1<<plate_id), density_, stratum.mass()/density_/area);
        }
    };

}

