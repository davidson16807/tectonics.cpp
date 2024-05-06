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

    /*
    basic use case:
    summarization.summarize (plates, locals, scratch)      // summarize each plate into a (e.g.) FormationSummary raster
    frames     .globalize   (plates, locals, globals)      // resample plate-specific rasters onto a global grid
    summarization.condense  (globals, master)              // condense globalized rasters into e.g. LithosphereSummary
    frames     .localize    (plates,  master, derivatives) // resample global raster to a plate-specific for each plate
    */

}

