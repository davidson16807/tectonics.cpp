#pragma once

#include <.hpp>

#include <index/procedural/Map.hpp>     // procedural::map

#include <field/Compose.hpp>        // field::compose
#include <field/Affinity.hpp>       // field::affinity

#include <grid/dymaxion/series.hpp> // dymaxion::NearestVertexIds, dymaxion::VertexPositions

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

    frames        .localize  (plates, precip, summaries) // we'll need precipitation specificatlly for weathering, but otherwise weathering can be in-order
    */

    // NOTE: `M` is mineral count, `F` is formation count
    template <int M, int F>
    class LithosphereSummarization
    {

        const CrustSummarization summarize;
        const LithosphereSummaryTools tools;

    public:

        LithosphereSummarization(
            const CrustSummarization& summarize, const LithosphereSummaryTools tools
        ):
            summarize(summarize), tools(tools)
        {}

        void summarize(
            const Lithosphere<M,F>& plates,
            LithosphereSummary& out,
            CrustSummary& scratch_formation
        ) const {
            *CrustSummary scratch_formation = global_formation;
            for (std::size_t i = 0; i < plates.size(); ++i)
            {
                summarize(i, plates[i], out[i], scratch_formation);
            }
        }

        void flatten (
            const LithosphereSummary& formations,
            CrustSummary& out
        ) const {
            std::fill(out.begin(), out.end(), StratumSummary{});
            for (std::size_t i = 0; i < formations.size(); ++i)
            {
                tools.absorb(out, formations[i], out);
            }
        }

    };


}

