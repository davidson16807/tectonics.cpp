#pragma once

#include <algorithm> // std::max
#include <vector>    // std::vector

// in house libraries
#include <unit/si.hpp>

#include <model/rock/formation/FormationSummary.hpp>

namespace rock{


    /*
    `FormationSummaryProperty` allows creation of a generic Unary FormationSummary callable given an arbitrary StratumSummaryProperty.
    It provides a more generic alternative to other dedicated formation summary properties
    */
    template <typename StratumSummaryProperty>
    class FormationSummaryProperty
    {
        const StratumSummaryProperty property;
    public:
        FormationSummaryProperty(const StratumSummaryProperty& property):
            property(property)
        {}
        void operator()(const FormationSummary& summary, auto& out) const
        {
            for (auto i = 0*summary.size(); i < summary.size(); ++i)
            {
                out[i] = property(summary[i]);
            }
        }
    };

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

