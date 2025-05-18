#pragma once

#include <cmath>

#include <vector>

#include <unit/si.hpp>

#include "CrustSummary.hpp"

/*
*/

namespace rock {

    /*
    `CrustSummaryPredicates` contains predicates
    that are intended as inputs for the methods of `CrustSimulation`.
    */

    template<typename Morphology, typename Bitsets>
    class CrustSummaryPredicates
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

        const Morphology morphology;
        const Bitsets bitsets;

    public:
        CrustSummaryPredicates(const Morphology& morphology, const Bitsets& bitsets):
            morphology(morphology),
            bitsets(bitsets)
        {}

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
        `includes` returns a boolean raster indicating cells where the topmost plate is of given id
        */
        void includes(
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
        `ownable` returns a boolean raster indicating cells that can be owned by a plate of given id
        */
        void ownable(
            const int plate_id,
            const CrustSummary& crust,
            bools& out
        ) const {
            for (std::size_t i = 0; i < crust.size(); ++i)
            {
                out[i] = 
                    crust[i].plate_count() == 0
                || (crust[i].plate_count() == 1 &&  
                    crust[i].is_top(plate_id))
                ;
            }
        }

        /*
        `detachable` returns a boolean raster indicating where an entire rock column is detachable
        */
        void detachable(
            const int plate_id,
            const density mantle_density,
            const CrustSummary& crust,
            bools& out
        ) const {
            for (std::size_t i = 0; i < crust.size(); ++i)
            {
                out[i] = 
                    crust[i].plate_count() > 1
                 && !crust[i].is_top(plate_id)
                 // && crust[i].is_subducted(plate_id)
                 // && crust[i].density() > mantle_density
                ;
            }
        }

        /*
        `rifting` returns a scalar raster indicating where gaps in the plates should be filled by a given plate
        */
        template<typename Grid>
        void rifting(
            const Grid& grid,
            const bools& ownable,
            const bools& exists,
            bools& out,
            bools& scratch
        ) const {
            /*
            The implementation below is equivalent to the following set logic:
                rifting = erode(ownable) ∩ margin(exists)
            */
            bools* will_stay_ownable = &scratch;
            bools* just_outside      = &out;
            morphology.erode   (grid, ownable,      *will_stay_ownable);
            morphology.outshell(grid, exists,       *just_outside);
            bitsets.intersect  (*will_stay_ownable, *just_outside, out);
        }

        /*
        `detaching` returns a scalar raster indicating where cells of a plate should be destroyed
        */
        template<typename Grid>
        void detaching(
            const Grid& grid,
            const bools& detachable,
            const bools& exists,
            bools& out,
            bools& scratch
        ) const {
            bools* will_stay_detachable = &scratch;
            bools* just_inside          = &out;
            morphology.erode  (grid, detachable, *will_stay_detachable);
            morphology.inshell(grid, exists,     *just_inside);
            bitsets.intersect  (*will_stay_detachable, *just_inside, out);
        }

    };

}

