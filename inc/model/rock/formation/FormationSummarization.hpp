#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries
#include <Formation.hpp>
#include <FormationSummary.hpp>

namespace rock
{

    template<typename Grid, typename StratumSummarization>
    class FormationSummarization
    {
        const StratumSummarization summarization;
        const Grid grid;
    public:
        FormationSummarization(const StratumSummarization& summarization, const Grid& grid):
            summarization(summarization),
            grid(grid),
        {}
        void operator() (const Formation<M>& formation, FormationSummary& out) const
        {
            for (std::size_t i = 0; i < formation.size(); ++i)
            {
                out[i] = summarization(grid.vertex_dual_area(i), formation[i]);
            }
        }
    };

}

