#pragma once

// C libraries

// std libraries

// 3rd party libraries

// in-house libraries
#include "Formation.hpp"
#include "FormationSummary.hpp"

namespace rock
{

    template<typename Grid, typename StratumSummarization>
    class FormationSummarization
    {

        using area = si::area<float>;
        using length = si::length<float>;

        const StratumSummarization summarization;
        const Grid grid;
        const length radius_units;

    public:

        FormationSummarization(
            const StratumSummarization& summarization, 
            const Grid& grid, 
            const length world_radius
        ):
            summarization(summarization),
            grid(grid),
            radius_units(world_radius/grid.voronoi.radius)
        {}

        template<typename Formation>
        void operator() (const int plate_id, const Formation& formation, FormationSummary& out) const
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

    template<int M, typename Grid, typename StratumSummarization>
    auto formation_summarization(
        const StratumSummarization& summarization, 
        const Grid& grid, 
        const si::length<float> world_radius
    ){
        return FormationSummarization<Grid,StratumSummarization>(summarization, grid, world_radius);
    }

}
