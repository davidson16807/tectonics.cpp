#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries

namespace rock
{

    template<typename StratumDensity, typename StratumThickness>
    class FormationSummarization
    {
        const StratumDensity   density;
        const StratumThickness thickness;
    public:
        FormationSummarization(const StratumDensity& density, const StratumThickness& thickness):
            density(density),
            thickness(thickness)
        {}
        void operator() (const int plate_id, const Formation<M>& formation, std::vector<StratumSummary>& out) const
        {
            for (std::size_t i = 0; i < formation.size(); ++i)
            {
                out[i] = StratumSummary(1<<plate_id, density(formation[i]), thickness(formation[i]));
            }
        }
    };

}

