#pragma once

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
        void copy(const Crust<M,F>& a, Crust<M,F>& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                ops.copy(a[i], out[i]);
            }
        }
        void absorb (const Crust<M,F>& top, const Crust<M,F>& bottom, Crust<M,F>& out, Formation<M>& scratch) const
        {

            bools empty          (top[0].size());
            bools meta_empty     (top[0].size());
            bools empty_below    (top[0].size());
            bools nonempty_below (top[0].size());

            copy(top, out);

            const auto MI = formations::metaigneous;
            const auto I = formations::igneous;

            for (std::size_t i(MI); i >= formations::sedimentary; i-=2) // all nonsediment layers
            {

                // if `empty_below` at this line, then the ith layer of bottom can merge with the same i from top, otherwise it merges with on of the igenous layers

                ops.mask(bottom[i],   empty_below,    scratch);
                ops.combine(out[i],   scratch,        out[i]); // meta
                ops.mask(bottom[i],   nonempty_below, scratch);
                ops.combine(out[MI],  scratch,        out[MI]); // meta

                ops.mask(bottom[i-1], empty_below,    scratch);
                ops.combine(out[i-1], scratch,        out[i]); // nonmeta
                ops.mask(bottom[i],   nonempty_below, scratch);
                ops.combine(out[I],   scratch,        out[I]); // nonmeta

                predicates.empty(top[i], meta_empty);                  // meta
                predicates.empty(top[i-1], empty);                     // nonmeta
                morphology.unite(empty, meta_empty, empty);            // meta ∩ nonmeta
                morphology.intersect(empty, empty_below, empty_below); // empty_below ∪= (meta ∩ nonmeta)
                morphology.negate(empty_below, nonempty_below);        // ¬empty_below

            }

            // sediment doesn't follow a meta/nonmeta paradigm so it must be handled separately
            ops.mask(bottom[formations::sediment], empty_below,   scratch);
            ops.combine(out[formations::sediment], scratch,       out[formations::sediment]);
            ops.mask(bottom[formations::sediment], nonempty_below, scratch);
            ops.combine(out[I],  scratch,        out[I]); 

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
