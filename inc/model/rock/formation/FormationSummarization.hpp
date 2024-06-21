#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries
#include "FormationSummary.hpp"

namespace rock
{

    template<typename Grid, typename StratumSummarization>
    class FormationSummarization
    {
        using area = si::area<float>;

        const StratumSummarization summarization;
        const Grid grid;
    public:
        FormationSummarization(const StratumSummarization& summarization, const Grid& grid):
            summarization(summarization),
            grid(grid)
        {}
        template<typename Formation>
        void operator() (const Formation& formation, FormationSummary& out) const
        {
            for (std::size_t i = 0; i < formation.size(); ++i)
            {
                out[i] = summarization(grid.vertex_dual_area(i)*area(si::meter2), formation[i]);
            }
        }
    };

    template<int M, typename Grid, typename StratumSummarization>
    auto formation_summarization(const StratumSummarization& summarization, const Grid& grid){
        return FormationSummarization<Grid,StratumSummarization>(summarization, grid);
    }

}

