#pragma once

#include <index/procedural/Map.hpp>     // procedural::map

#include <field/Compose.hpp>        // field::compose
#include <field/Affinity.hpp>       // field::affinity

#include <grid/dymaxion/series.hpp> // dymaxion::VertexPositions
#include <grid/dymaxion/field.hpp> // dymaxion::NearestVertexIds

#include <model/rock/crust/CrustSummaryOps.hpp>

#include "Lithosphere.hpp"
#include "LithosphereSummary.hpp"

// in house libraries

namespace rock{

    /*
    summarize: plateⁿ ⟶ summaryⁿ
    flatten:   summaryⁿ ⟶ summary
    
    sediment:  plateⁿ × summaryⁿ ⟶ formationⁿ
    flatten:   formationⁿ ⟶ formation

    basic use case:

    summarization .summarize (plates,    locals, scratch)   // summarize each plate into a (e.g.) CrustSummary raster
    frames        .globalize (rotations, locals, globals)   // resample plate-specific rasters onto a global grid
    summarization .flatten   (globals,   master)            // condense globalized rasters into e.g. LithosphereSummary
    frames        .localize  (rotations, master, summaries) // resample global raster to a plate-specific for each plate

    frames        .localize  (plates, precip, summaries) // we'll need precipitation specifically for weathering, but otherwise weathering can be in-order
    */

    // NOTE: `M` is mineral count, `F` is formation count
    template <int M, int F, typename CrustSummarization>
    class LithosphereSummarization
    {

        const CrustSummarization summarization;
        const CrustSummaryOps ops;

    public:

        LithosphereSummarization(
            const CrustSummarization& summarization,
            const CrustSummaryOps& ops
        ):
            summarization(summarization),
            ops(ops)
        {}

        void summarize(
            const Lithosphere<M,F>& plates,
            LithosphereSummary& out,
            FormationSummary& scratch_formation
        ) const {
            for (std::size_t i = 0; i < plates.size(); ++i)
            {
                summarization(i, plates[i], out[i], scratch_formation);
            }
        }

        void flatten (
            const LithosphereSummary& formations,
            CrustSummary& out
        ) const {
            std::fill(out.begin(), out.end(), StratumSummary{});
            for (std::size_t i = 0; i < formations.size(); ++i)
            {
                ops.absorb(out, formations[i], out);
            }
        }

    };

    template <int M, int F, typename CrustSummarization>
    auto lithosphere_summarization(
        const CrustSummarization& summarization, 
        const CrustSummaryOps& ops
    ){
        return LithosphereSummarization<M,F,CrustSummarization>(summarization, ops);
    }

}

