#pragma once

#include <cmath>

#include <vector>

#include <unit/si.hpp>

#include "CrustSummary.hpp"

/*
*/

namespace rock {

    /*
    `CrustSummaryProperties` contains properties (mostly predicates) 
    that are intended as inputs for the methods of `CrustSimulation`.
    */

    class CrustSummaryProperties
    {
        using mass      = si::mass<float>;
        using area      = si::area<float>;
        using density   = si::density<float>;
        using force     = si::force<float>;
        using acceleration = si::acceleration<float>;
        using area_density = si::area_density<float>;

        using bools     = std::vector<bool>;

        using masses    = std::vector<mass>;
        using areas     = std::vector<area>;
        using forces    = std::vector<force>;

    public:
        CrustSummaryProperties(){}

        /*
        NOTE: We are guaranteed to generate CrustSummary at least once every iteration,
        This is done in order to performantly calculate plate gaps and collisions.
        Unless we need information that is only contained in StratumStore, it is always more performant to operate on CrustSummary,
        since StratumStore has a bigger footprint in memory, and operating on StratumStore requires iterating through MineralStores.
        To simplify design, encourage performant usage, and encourage cache reuse in the CPU,
        all operations will be implemented exclusively on CrustSummary, unless required for out-of-order traversal or otherwise impossible.
        Exceptions will be noted in the comments. 
        */

        /*
        `alone` returns a boolean raster indicating where cells are occupied by one or fewer plate
        */
        void alone(
            const CrustSummary& crust,
            bools& out
        ) const {
            for (std::size_t i = 0; i < crust.size(); ++i)
            {
                out[i] = crust[i].plate_count() <= 1;
            }
        }

        /*
        `exists` returns a boolean raster indicating where cells are occupied by a plate of given id
        */
        void exists(
            const int plate_id,
            const CrustSummary& crust,
            bools& out
        ) const {
            for (std::size_t i = 0; i < crust.size(); ++i)
            {
                out[i] = crust[i].includes(plate_id);
            }
        }

        /*
        `top` returns a boolean raster indicating cells where the topmost plate is of given id
        */
        void top(
            const int plate_id,
            const CrustSummary& crust,
            bools& out
        ) const {
            for (std::size_t i = 0; i < crust.size(); ++i)
            {
                out[i] = crust[i].is_top(plate_id);
            }
        }

        /*
        `foundering` returns a boolean raster indicating where an entire rock column is foundering
        */
        void foundering(
            const density mantle_density,
            const CrustSummary& crust,
            bools& out
        ) const {
            for (std::size_t i = 0; i < crust.size(); ++i)
            {
                out[i] = crust[i].density() > mantle_density;
            }
        }

    };

    /*
    `CrustSummaryProperty` allows creation of a generic Unary CrustSummary callable given an arbitrary StratumSummaryProperty.
    It provides a more generic alternative to `CrustSummaryProperties`
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


}



