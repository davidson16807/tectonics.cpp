#pragma once

#include <cmath>

#include <vector>

#include <unit/si.hpp>

#include "CrustSummary.hpp"

/*
*/

namespace rock {

    /*
    `CrustSummaryProperty` allows creation of a generic Unary CrustSummary callable given an arbitrary StratumSummaryProperty.
    It provides a more generic alternative to `CrustSummaryPredicates`
    */
    template <typename ColumnSummaryProperty>
    class CrustSummaryProperty
    {
        const ColumnSummaryProperty property;
    public:
        CrustSummaryProperty(const ColumnSummaryProperty& property):
            property(property)
        {}
        void operator()(const CrustSummary& summary, auto& out) const
        {
            for (auto i = 0*summary.size(); i < summary.size(); ++i)
            {
                out[i] = property(summary[i]);
            }
        }
    };

    template <typename ColumnSummaryProperty>
    auto crust_summary_property(const ColumnSummaryProperty& property)
    {
        return CrustSummaryProperty<ColumnSummaryProperty>(property);
    }

    /*
    `CrustSummaryMass`
    */
    template<typename Grid>
    class CrustSummaryMass
    {

        using length = si::length<float>;
        using mass = si::mass<float>;

        using masses = std::vector<mass>;

        static constexpr length meter = length(si::meter);

        const Grid grid;

    public:

        CrustSummaryMass(const Grid& grid):
            grid(grid)
        {}

        void operator()(const CrustSummary& summary, masses& out) const
        {
            for (auto i = 0*summary.size(); i < summary.size(); ++i)
            {
                out[i] = summary[i].area_density() * grid.vertex_dual_area(i) * meter*meter;
            }
        }

        mass operator()(const CrustSummary& summary) const
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

