#pragma once

#include <.hpp>

#include <index/each.hpp>           // each::get
#include <index/series/Map.hpp>     // series::map

#include <field/Compose.hpp>        // field::compose
#include <field/Affinity.hpp>       // field::affinity

#include <grid/dymaxion/series.hpp> // dymaxion::NearestVertexIds, dymaxion::VertexPositions

// in house libraries

namespace rock{

    class LithosphereSummarization
    {

        const CrustSummarization summarize;
        const LithosphereSummaryTools ops;

    public:

        LithosphereSummarization(
            const CrustSummarization& summarize, const LithosphereSummaryTools ops
        ):
            summarize(summarize), ops(ops)
        {}

        void summarize(
            const std::vector<Plate>& plates,
            std::vector<FormationSummary>& formations,
            FormationSummary& scratch_formation
        ) const {
            *FormationSummary scratch_formation = global_formation;
            ops.reset(out);
            for (std::size_t i = 0; i < plates.size(); ++i)
            {
                summarize(i, plates[i].crust, formations[i], scratch_formation);
            }
        }

        void condense() (
            const std::vector<FormationSummary>& formations,
            LithosphereSummary& lithosphere,
        ) const {
            *FormationSummary scratch_formation = global_formation;
            ops.reset(lithosphere);
            for (std::size_t i = 0; i < plates.size(); ++i)
            {
                ops.absorb(lithosphere, formations[i], lithosphere);
            }
        }

    };

    template <typename Summarization, typename Ops>
    class LithosphereSummarization
    {

        const Summarization summarize;
        const Ops ops;

    public:

        LithosphereSummarization(
            const Summarization& summarize, const Ops ops
        ):
            summarize(summarize), ops(ops)
        {}

        template <typename Summarizable, typename Summary>
        void summarize(
            const std::vector<Summarizable>& summarizables,
            std::vector<Summary>& summaries,
            Summary& scratch
        ) const {
            ops.reset(out);
            for (std::size_t i = 0; i < summarizables.size(); ++i)
            {
                summarize(i, summarizables[i], summaries[i], scratch);
            }
        }

        template <typename Summary, typename Combination>
        void condense() (
            const std::vector<Combination>& summaries,
            Combination& combination,
        ) const {
            ops.reset(combination);
            for (std::size_t i = 0; i < summarizables.size(); ++i)
            {
                ops.absorb(combination, summaries[i], combination);
            }
        }

    };

    /*

    basic use case:

    summarization .summarize (plates, locals, scratch)   // summarize each plate into a (e.g.) FormationSummary raster
    frames        .globalize (plates, locals, globals)   // resample plate-specific rasters onto a global grid
    summarization .condense  (globals,master)            // condense globalized rasters into e.g. LithosphereSummary
    frames        .localize  (plates, master, summaries) // resample global raster to a plate-specific for each plate

    frames        .localize  (plates, precip, summaries) // we'll need precipitation specificatlly for weathering, but otherwise weathering can be in-order

    We also need a consolidated global raster for erosion, which requires knowledge of sediment in the top plate.
    If we add sediment to CrustSummary then we'd need to add a new function that 
    builds a new CrustSummary whose sediment attribute stores the deltas from erosion, 
    however this couples concerns, we'd rather just create a sediment object that erosion acts on.
    This requires knowledge of the topmost plate which we don't have until the call to condense,
    so we need a new function that creates sediment from the plates and the master raster:

    top_sediment    (plates, summaries, sediments) // this function returns empty Strata if plate is not on top, sediment otherwise
    crust.condense  (sediments, sediment)          // this behaves similarly to summarization.condense, except it uses CrustOps.combine() internally
    erosion         (sediment,  delta)             // this is the erosion model, proper
    frames.localize (plates, delta, deltas)

    */

}

