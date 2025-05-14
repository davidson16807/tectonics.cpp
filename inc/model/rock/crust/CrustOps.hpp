#pragma once

#include <algorithm> // std::max

// in house libraries
#include <index/adapted/boolean/BooleanBitset.hpp>
#include <index/iterated/Bitset.hpp>
#include <index/iterated/Nary.hpp>
#include <index/procedural/Uniform.hpp>

#include <model/rock/strata/Strata.hpp>
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
        const iterated::Ternary ternary_;
        const iterated::Identity copy_;

    public:
        CrustOps():
            ops(),
            predicates(),
            morphology(adapted::BooleanBitset{}),
            ternary_(),
            copy_()
        {}

        void absorb (const Crust<M,F>& top, const Crust<M,F>& bottom, Crust<M,F>& out, Formation<M>& scratch) const
        {

            bools empty          (top[0].size());
            bools meta_empty     (top[0].size());
            bools newly_occupied (top[0].size());
            bools empty_below    (top[0].size(), true);

            copy_(top, out);

            const std::size_t MI = formations::metaigneous;
            const std::size_t S  = formations::sediment;

            for (std::size_t i(MI); i > S; i-=2) // all nonsediment layers
            {
                predicates.empty(top[i],   meta_empty);
                predicates.empty(top[i-1], empty);
                morphology.intersect(empty, meta_empty,  empty);
                morphology.differ   (empty_below, empty, newly_occupied);

                // deposit anything remaining in the current layer immediately
                ops.mask(bottom[i],   empty_below, scratch);
                ops.combine(out[i],   scratch,     out[i]); // meta
                ops.mask(bottom[i-1], empty_below, scratch);
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

                morphology.intersect(empty, empty_below, empty_below);
            }
            // sediment doesn't follow a meta/nonmeta paradigm so it must be handled separately
            ops.mask(bottom[S],   empty_below, scratch);
            ops.combine(out[S],   scratch,     out[S]); // meta

        }

        // AKA, the identity function.
        void copy(const Crust<M,F>& crust, Crust<M,F>& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                ops.copy(crust[i], out[i]);
            }
        }

        // AKA, the identity function.
        template<typename Grid, typename CrustField>
        void copy(const Grid& grid, const CrustField crust, Crust<M,F>& out) const {
            for (std::size_t i = 0; i < out.size(); ++i)
            {
                ops.copy(crust[i], out[i]);
            }
        }

        template<typename Grid, typename CrustField>
        void ternary(const Grid& grid, const bools& condition, const Crust<M,F>& crust, const CrustField& field, Crust<M,F>& out) const 
        {
            for (std::size_t i(0); i < F; ++i)
            {
                for (auto j = 0*grid.vertex_count(); j < grid.vertex_count(); ++j)
                {
                    out[i][j] = condition[i]? crust[i][j] : field(grid,i,j);
                }
            }
        }

        template<typename Grid, typename CrustField>
        void ternary(const Grid& grid, const bools& condition, const CrustField& field, const Crust<M,F>& crust, Crust<M,F>& out) const 
        {
            for (std::size_t i(0); i < F; ++i)
            {
                for (auto j = 0*grid.vertex_count(); j < grid.vertex_count(); ++j)
                {
                    out[i][j] = condition[i]? field(grid,i,j) : crust[i][j];
                }
            }
        }

        void ternary(const bools& condition, const Crust<M,F>& crust1, const Crust<M,F>& crust2, Crust<M,F>& out) const 
        {
            for (std::size_t i(0); i < F; ++i)
            {
                for (std::size_t j(0); j < out[i].size(); ++j)
                {
                    out[i][j] = condition[i]? crust1[i][j] : crust2[i][j];
                }
            }
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

