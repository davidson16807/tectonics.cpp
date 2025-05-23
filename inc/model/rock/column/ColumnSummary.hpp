#pragma once

// in-house libraries
#include <model/rock/stratum/StratumSummary.hpp>

namespace rock{

    struct ColumnSummary
    {

        StratumSummary top; 
        StratumSummary rest;

        constexpr ColumnSummary():
            top(),
            rest()
        {}

        constexpr ColumnSummary(
            const StratumSummary top
        ):
            top(top),
            rest()
        {}

        constexpr ColumnSummary(
            const StratumSummary top, 
            const StratumSummary rest
        ):
            top(top),
            rest(rest)
        {}

        constexpr ColumnSummary(const ColumnSummary& summary):
            top(summary.top),
            rest(summary.rest)
        {}

        inline si::length<float> thickness() const
        {
            return top.thickness() + rest.thickness();
        }

        inline si::area_density<float> area_density() const
        {
            return top.area_density() + rest.area_density();
        }

        inline si::density<float> density() const
        {
            return area_density() / thickness();
        }

        inline bool exists() const
        {
            return plate_count()>0;
        }

        inline int plate_count() const
        {
            return top.plate_count() + rest.plate_count();
        }

        inline bool includes(const int plate_id) const 
        {
            return top.includes(plate_id) || rest.includes(plate_id);
        }

        inline bool is_top(const int plate_id) const 
        {
            return top.includes(plate_id);
        }

        inline bool is_subducted(const int plate_id) const
        {
            return rest.includes(plate_id);
        }


    };

}

