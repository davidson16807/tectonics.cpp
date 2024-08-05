#pragma once

#include <algorithm> // std::max

// in house libraries
#include <model/rock/formation/Formation.hpp>

namespace rock{

    /*
    `FormationSummaryMass`
    */
    template<typename Grid>
    class FormationSummaryMass
    {

        using length = si::length<float>;
        using mass = si::mass<float>;

        using masses = std::vector<mass>;

        static constexpr length meter = length(si::meter);

        const Grid grid;

    public:

        FormationSummaryMass(const Grid& grid):
            grid(grid)
        {}

        void operator()(const FormationSummary& summary, masses& out) const
        {
            for (auto i = 0*summary.size(); i < summary.size(); ++i)
            {
                out[i] = summary[i].area_density() * grid.vertex_dual_area(i) * meter*meter;
            }
        }

    };

}

