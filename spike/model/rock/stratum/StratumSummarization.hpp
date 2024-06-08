#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries

namespace rock
{

    template<int M, typename StratumDensity, typename StratumThickness>
    class StratumSummarization
    {
        const StratumDensity   density;
        const StratumThickness thickness;
    public:
        StratumSummarization(const StratumDensity& density, const StratumThickness& thickness):
            density(density),
            thickness(thickness)
        {}
        StratumSummary operator() (const int plate_id, const Stratum<M>& stratum) const
        {
            return StratumSummary(1<<plate_id, density(stratum), thickness(stratum));
        }
    };

}

