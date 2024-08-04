#pragma once

#include <algorithm> // std::max

// in house libraries
#include <index/adapted/boolean/BooleanBitset.hpp>
#include <index/iterated/Bitset.hpp>

#include <model/rock/formation/FormationOps.hpp>
#include <model/rock/formation/FormationPredicates.hpp>
#include <model/rock/crust/FormationType.hpp>

#include "Crust.hpp"

namespace rock{

    // NOTE: `M` is mineral count, `F` is formation count
    template <int M>
    class CrustOps
    {

        using bools = std::vector<bool>;

        static constexpr int F = 5;

        const FormationOps<M> ops;
        const FormationPredicates<M> predicates;
        const iterated::Bitset<adapted::BooleanBitset> morphology;

    public:
        CrustOps():
            ops(),
            predicates(),
            morphology(adapted::BooleanBitset{})
        {}

        // AKA, the identity function.
        void copy(const Crust<M,F>& crust, Crust<M,F>& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                ops.copy(crust[i], out[i]);
            }
        }

        void absorb (const Crust<M,F>& top, const Crust<M,F>& bottom, Crust<M,F>& out, Formation<M>& scratch) const
        {

            bools occupied       (top[0].size());
            bools meta_occupied  (top[0].size());
            bools occupied_below (top[0].size(), false);
            bools newly_occupied (top[0].size());
            bools still_empty    (top[0].size());

            copy(top, out);

            const std::size_t MI = formations::metaigneous;
            const std::size_t S  = formations::sediment;

            for (std::size_t i(MI); i > S; i-=2) // all nonsediment layers
            {
                predicates.exists(top[i], meta_occupied);
                predicates.exists(top[i-1], occupied);
                morphology.unite (occupied, meta_occupied,  occupied);
                morphology.differ(occupied, occupied_below, newly_occupied);
                morphology.negate(occupied_below,           still_empty);
                morphology.copy  (occupied,                 occupied_below);

                // deposit anything remaining in the current layer immediately
                ops.mask(bottom[i],   still_empty, scratch);
                ops.combine(out[i],   scratch,     out[i]); // meta
                ops.mask(bottom[i-1], still_empty, scratch);
                ops.combine(out[i-1], scratch,     out[i-1]); // nonmeta
                // any bottom layer for which `nonempty_below` needs to be deposited to one of the two immediate layers below
                for (std::size_t j(i-2); j > S; j-=2) // all nonsediment layers
                {
                    ops.mask(bottom[j],   newly_occupied, scratch);
                    ops.combine(out[i],   scratch,        out[i]); // meta
                    ops.mask(bottom[j-1], newly_occupied, scratch);
                    ops.combine(out[i-1], scratch,        out[i-1]); // nonmeta
                }
                // sediment doesn't follow a meta/nonmeta paradigm so it must be handled separately
                ops.mask(bottom[S], newly_occupied, scratch);
                ops.combine(out[i-1], scratch,      out[i-1]); // nonmeta
            }
            // sediment doesn't follow a meta/nonmeta paradigm so it must be handled separately
            ops.mask(bottom[S],   still_empty, scratch);
            ops.combine(out[S],   scratch,     out[S]); // meta

        }

        void flatten (const Crust<M,F>& crust, Formation<M>& out) const
        {
            ops.copy(crust[0], out);
            for (std::size_t i = 1; i < crust.size(); ++i)
            {
                ops.combine(crust[i], out, out);
            }
        }
    };

}
