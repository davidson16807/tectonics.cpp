#pragma once

#include <algorithm> // std::max

// in house libraries
#include <model/rock/formation/Formation.hpp>

namespace rock{

    /*
    `FormationSummaryMass`
    */
    class FormationSummaryMass
    {

        using length = si::length<float>;
        using mass = si::mass<float>;

        using masses = std::vector<mass>;

        static constexpr length meter = length(si::meter);

    public:

        FormationSummaryMass()
        {}

        template<typename Grid>
        void operator()(const Grid& grid, const FormationSummary& summary, masses& out) const
        {
            for (auto i = 0*summary.size(); i < summary.size(); ++i)
            {
                out[i] = summary[i].area_density() * grid.vertex_dual_area(i) * meter*meter;
            }
        }

        template<typename Grid>
        mass operator()(const Grid& grid, const FormationSummary& summary) const
        {
            mass out(0);
            for (auto i = 0*summary.size(); i < summary.size(); ++i)
            {
                out += summary[i].area_density() * grid.vertex_dual_area(i) * meter*meter;
            }
            return out;
        }

    };

}

