#pragma once

#include <unit/si.hpp>

#include "ColumnSummaryOps.hpp"

namespace rock{

    template <typename StratumProperty>
    class ColumnSummaryProperty
    {

        const StratumProperty property;
        const ColumnSummaryOps ops;

    public:

        ColumnSummaryProperty(const StratumProperty& property, const ColumnSummaryOps& ops): 
            property(property),
            ops(ops)
        {}

        si::length<float> operator()(const ColumnSummary& summary) const
        {
            return property(ops.flatten(summary));
        }

    };

    // TODO: move this to generic "summary/" folder
    /* 
    this should be a faster version of ColumnSummaryProperty<StratumSummaryIsostaticDisplacement>
    */
    class ColumnSummaryIsostaticDisplacement
    {
        const si::density<float> mantle_density;

    public:

        ColumnSummaryIsostaticDisplacement(const si::density<float> mantle_density):
            mantle_density(mantle_density)
        {}

        auto operator()(const ColumnSummary& summary) const
        {
            return summary.thickness() * (1.0f - summary.density()/mantle_density);
        }

    };

    // TODO: move this to generic "summary/" folder
    struct ColumnSummaryBuoyancyPressure
    {

        using length            = si::length<double>;
        using acceleration      = si::acceleration<double>;
        using density           = si::density<double>;
        using pressure          = si::pressure<double>;

        const acceleration gravity;
        const density mantle_density;

        ColumnSummaryBuoyancyPressure(
            const acceleration gravity,
            const density mantle_density
        ):
            gravity(gravity),
            mantle_density(mantle_density)
        {}
        auto operator()(const ColumnSummary& summary) const
        {
            auto density_difference = si::max(
                0.0*si::kilogram/si::meter3, 
                density(summary.density() - mantle_density)
            );
            return pressure(
                gravity * density_difference   // Δρ density difference
              * length(summary.thickness()) // V  volume
            );
        }
    };

    struct ColumnSummaryExists
    {
        constexpr ColumnSummaryExists(){}
        inline auto operator()(const ColumnSummary& summary) const { return summary.exists(); }
    };

    struct ColumnSummaryAreaDensity
    {
        constexpr ColumnSummaryAreaDensity(){}
        inline auto operator()(const ColumnSummary& summary) const { return summary.area_density(); }
    };

    struct ColumnSummaryThickness
    {
        constexpr ColumnSummaryThickness(){}
        inline auto operator()(const ColumnSummary& summary) const { return summary.thickness(); }
    };

    struct ColumnSummaryDensity
    {
        constexpr ColumnSummaryDensity(){}
        inline auto operator()(const ColumnSummary& summary) const { return summary.density(); }
    };

    struct ColumnSummaryPlateCount
    {
        constexpr ColumnSummaryPlateCount(){}
        inline auto operator()(const ColumnSummary& summary) const { return summary.plate_count(); }
    };

    struct ColumnSummaryIncludesPlate
    {
        const int plate_id;
        constexpr ColumnSummaryIncludesPlate(const int plate_id): plate_id(plate_id){}
        inline auto operator()(const ColumnSummary& summary) const { return summary.includes(plate_id); }
    };

    struct ColumnSummaryIsPlateOnTop
    {
        const int plate_id;
        constexpr ColumnSummaryIsPlateOnTop(const int plate_id): plate_id(plate_id){}
        inline auto operator()(const ColumnSummary& summary) const { return summary.is_top(plate_id); }
    };

    struct ColumnSummaryIsPlateSubducted
    {
        const int plate_id;
        constexpr ColumnSummaryIsPlateSubducted(const int plate_id): plate_id(plate_id){}
        inline auto operator()(const ColumnSummary& summary) const { return summary.is_subducted(plate_id); }
    };

}

