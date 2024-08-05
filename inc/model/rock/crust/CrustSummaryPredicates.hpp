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

    template<typename Morphology, typename Bitset>
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
        `alone` returns a boolean raster indicating where cells are occupied by one or fewer plate
        */
        void alone(
            const CrustSummary& crust,
            bools& out
        ) const {
            for (int i = 0; i < crust.size(); ++i)
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
            for (int i = 0; i < crust.size(); ++i)
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
            for (int i = 0; i < crust.size(); ++i)
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
            for (int i = 0; i < crust.size(); ++i)
            {
                out[i] = crust[i].density() > mantle_density;
            }
        }


        /*
        `rifting` returns a scalar raster indicating where gaps in the plates should be filled by a given plate
        */
        void rifting(
            const bools& alone,
            const bools& top,
            const bools& exists,
            bools& out,
            bools& scratch
        ) const {
            /*
            The implementation below is equivalent to the following set logic:
                rifting = erode(alone ∩ top) ∩ margin(exists)
            */
            bools* riftable           = &out;
            bools* will_stay_riftable = &scratch;
            bools* just_outside       = &out;
            bitsets.intersect    (alone, top,         riftable);
            morphology.erode   (riftable,           will_stay_riftable);
            morphology.outshell(exists,             just_outside);
            bitsets.intersect    (will_stay_riftable, just_outside, out);
        }

        /*
        `detaching` returns a scalar raster indicating where cells of a plate should be destroyed
        */
        void detaching(
            const bools& alone,
            const bools& top,
            const bools& exists,
            const bools& foundering,
            bools& out,
            bools& scratch
        ) const {
            /*
            The implementation below is equivalent to the following set logic:
                detaching = erode(!alone ∩ !top) ∩ padding(exists) ∩ foundering
            */
            bools* not_alone            = &out;
            bools* not_top              = &scratch;
            bools* subducting           = &out;
            // bools* will_stay_subducting = &scratch;
            bools* just_inside          = &out;
            bools* detachable           = &out;
            bitsets.negate      (alone,                not_alone);
            bitsets.negate      (top,                  not_top);
            bitsets.intersect   (not_alone, not_top,   subducting);
            // morphology.erode  (subducting, will_stay_subducting); // NOTE: this was in the js implementation but it is suspected to be wrong
            morphology.inshell(exists,               just_inside);
            bitsets.intersect   (subducting, just_inside, detachable);
            bitsets.intersect   (detachable, foundering, out);
        }

    };

}



