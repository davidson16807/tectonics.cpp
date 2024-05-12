#pragma once

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
            const StratumSummary top, 
            const StratumSummary rest,
        ):
            top(top),
            rest(rest)
        {}

        inline si::length<float> thickness() const
        {
            return top.thickness() + bottom.thickness();
        }

        inline si::area_density<float> area_density() const
        {
            return top.area_density() + bottom.area_density();
        }

        inline si::density<float> density() const
        {
            return area_density() / thickness();
        }

        inline bool exists() const
        {
            return area_density() > si::area_density<float>(0.0f);
        }
        inline bool includes(const int plate_id) const 
        {
            return top.includes(plate_id) || bottom.includes(plate_id);
        }
        inline int plate_count() const
        {
            return top.count() + bottom.count();
        }
        inline bool is_top(const int plate_id) const 
        {
            return top.includes(plate_id);
        }

        inline bool is_subducted(const int plate_id) const
        {
            return bottom.includes(plate_id);
        }


    };

}

