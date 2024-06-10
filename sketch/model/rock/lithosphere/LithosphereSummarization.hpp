#pragma once

#include <.hpp>

#include <index/each.hpp>           // each::get
#include <index/series/Map.hpp>     // series::map

#include <field/Compose.hpp>        // field::compose
#include <field/Affinity.hpp>       // field::affinity

#include <grid/dymaxion/series.hpp> // dymaxion::NearestVertexIds, dymaxion::VertexPositions

// in house libraries

namespace rock{

    template <int M>
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
            const std::vector<Crust<M>>& plates,
            std::vector<FormationSummary>& out,
            FormationSummary& scratch_formation
        ) const {
            *FormationSummary scratch_formation = global_formation;
            for (std::size_t i = 0; i < plates.size(); ++i)
            {
                summarize(i, plates[i], out[i], scratch_formation);
            }
        }

        void flatten (
            const std::vector<FormationSummary>& formations,
            FormationSummary& out
        ) const {
            std::fill(out.begin(), out.end(), StratumSummary());
            for (std::size_t i = 0; i < formations.size(); ++i)
            {
                tools.absorb(out, formations[i], out);
            }
        }

    };

    /*
    summarize: plateⁿ ⟶ summaryⁿ
    flatten:   summaryⁿ ⟶ summary
    
    sediment:  plateⁿ × summaryⁿ ⟶ formationⁿ
    flatten:   formationⁿ ⟶ formation

    basic use case:

    summarization .summarize (plates,    locals, scratch)   // summarize each plate into a (e.g.) FormationSummary raster
    frames        .globalize (rotations, locals, globals)   // resample plate-specific rasters onto a global grid
    summarization .flatten   (globals,   master)            // condense globalized rasters into e.g. LithosphereSummary
    frames        .localize  (rotations, master, summaries) // resample global raster to a plate-specific for each plate

    frames        .localize  (plates, precip, summaries) // we'll need precipitation specificatlly for weathering, but otherwise weathering can be in-order

    We also need a consolidated global raster for erosion, which requires knowledge of sediment in the top plate.
    If we add sediment to CrustSummary then we'd need to add a new function that 
    builds a new CrustSummary whose sediment attribute stores the deltas from erosion, 
    however this couples concerns, we'd rather just create a sediment object that erosion acts on.
    This requires knowledge of the topmost plate which we don't have until the call to condense,
    so we need a new function that creates sediment from the plates and the master raster:

    sediment.sediment (plates, summaries, sediments) // this function returns empty Strata if plate is not on top, sediment otherwise
    crust.condense    (sediments, sediment)          // this behaves similarly to summarization.condense, except it uses CrustOps.combine() internally
    erosion           (sediment,  delta)             // this is the erosion model, proper
    frames.localize   (plates, delta, deltas)

    */

}

