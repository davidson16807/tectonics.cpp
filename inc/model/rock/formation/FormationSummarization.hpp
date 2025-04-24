#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries
#include "Formation.hpp"
#include "FormationSummary.hpp"

namespace rock
{

    template<typename StratumSummarization>
    class FormationSummarization
    {

        using area = si::area<float>;
        using length = si::length<float>;

        const StratumSummarization summarization;
        const length radius_units;

    public:

        FormationSummarization(
            const StratumSummarization& summarization, 
            const length radius_units
        ):
            summarization(summarization),
            radius_units(radius_units)
        {}

        template<typename Grid, typename Formation>
        void operator() (const Grid& grid, const int plate_id, const Formation& formation, FormationSummary& out) const
        {
            // area average_area(radius_units*radius_units*grid.total_area()/grid.vertex_count());
            for (std::size_t i = 0; i < formation.size(); ++i)
            {
                out[i] = summarization(
                    plate_id, 
                    grid.vertex_dual_area(i)*radius_units*radius_units, 
                    formation[i]);
            }
        }

    };

    template<int M, typename StratumSummarization>
    auto formation_summarization(
        const StratumSummarization& summarization, 
        const si::length<float> radius_units
    ){
        return FormationSummarization<StratumSummarization>(summarization, radius_units);
    }

}
