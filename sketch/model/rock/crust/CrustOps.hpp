#pragma once

// in house libraries
#include <model/rock/formation/FormationOps.hpp>
#include <model/rock/crust/FormationType.hpp>

#include "Crust.hpp"

namespace rock{

    // NOTE: `M` is mineral count, `F` is formation count
    template <int M>
    class CrustOps
    {
        static constexpr int F = 5;

        const FormationOps<M> ops;
    public:
        CrustOps():
            ops()
        {}
        // AKA, the identity function.
        void copy(const Crust<M,F>& a, Crust<M,F>& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                ops.copy(a[i], out[i]);
            }
        }
        void absorb (const Crust<M,F>& top, const Crust<M,F>& bottom, Crust<M,F>& out) const
        {
            using bools = std::vector<bool>;
            bools empty       (top[0].size());
            bools meta_empty  (top[0].size());
            bools empty_below (top[0].size());
            for (std::size_t i(formations::metaigneous); i >= 0; i-=2)
            {
                // if `empty_below` at this line, then the ith layer of bottom can merge with the same i from top, otherwise it merges with igenous layers
                ops.combine(empty_below,    top[i],   bottom[i],   out[i]  ); // meta
                ops.combine(empty_below,    top[i-1], bottom[i-1], out[i-1]); // nonmeta
                ops.combine(nonempty_below, top[formations::igneous],     bottom[i],   out[formations::igneous]  ); // meta
                ops.combine(nonempty_below, top[formations::metaigneous], bottom[i-1], out[formations::metaigneous]); // nonmeta
                predicates.empty(top[i], meta_empty);                  // meta
                predicates.empty(top[i-1], empty);                     // nonmeta
                morphology.union(empty, meta_empty, empty);            // meta ∩ nonmeta
                morphology.intersect(empty, empty_below, empty_below); // empty_below ∪= (meta ∩ nonmeta)
                morphology.negate(empty_below, nonempty_below);        // ¬empty_below
            }
            // TODO: combine sediment layer
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

