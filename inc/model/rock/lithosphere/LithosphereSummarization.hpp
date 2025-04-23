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
    template <int M, int F, typename mat, typename CrustSummarization, typename CrustReferenceFrames>
    class LithosphereSummarization
    {

        const CrustSummarization summarization;
        const CrustReferenceFrames frames;
        const CrustSummaryOps ops;

    public:

        LithosphereSummarization(
            const CrustSummarization& summarization,
            const CrustReferenceFrames& frames,
            const CrustSummaryOps& ops
        ):
            summarization(summarization),
            frames(frames),
            ops(ops)
        {}

        void globalize(
            const std::vector<mat>& locals_to_globals,
            const Lithosphere<M,F>& plates,
            CrustSummary& out,
            CrustSummary& scratch_local,
            CrustSummary& scratch_global,
            FormationSummary& scratch_formation
        ) const {
            for (std::size_t i = 0; i < plates.size(); ++i)
            {
                summarization(i, plates[i], scratch_local, scratch_formation);
                frames.globalize(locals_to_globals[i], scratch_local, scratch_global);
                ops.absorb(out, scratch_global, out);
            }
        }

        void localize(
            const std::vector<mat>& locals_to_globals,
            const CrustSummary& global,
            std::vector<CrustSummary>& locals
        ) const {
            for (std::size_t i = 0; i < locals.size(); ++i)
            {
                frames.localize(glm::inverse(locals_to_globals[i]), global, locals[i]);
            }
        }

    };

    template <int M, int F, typename mat, typename CrustSummarization, typename CrustReferenceFrames>
    auto lithosphere_summarization(
        const CrustSummarization& summarization, 
        const CrustReferenceFrames& frames, 
        const CrustSummaryOps& ops
    ){
        return LithosphereSummarization<M,F,mat,CrustSummarization,CrustReferenceFrames>(summarization, frames, ops);
    }

}

