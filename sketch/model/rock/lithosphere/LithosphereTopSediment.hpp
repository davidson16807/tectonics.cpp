#pragma once

#include <.hpp>

#include <index/procedural/Map.hpp>     // procedural::map

#include <field/Compose.hpp>        // field::compose
#include <field/Affinity.hpp>       // field::affinity

#include <grid/dymaxion/series.hpp> // dymaxion::NearestVertexIds, dymaxion::VertexPositions

// in house libraries

namespace rock{


    /*
    We need a consolidated global raster for erosion, which requires knowledge of sediment in the top plate.
    If we add sediment to CrustSummary then we'd need to add a new function that 
    builds a new CrustSummary whose sediment attribute stores the deltas from erosion, 
    however this couples concerns, we'd rather just create a sediment object that erosion acts on.
    This requires knowledge of the topmost plate which we don't have until the call to condense,
    so we need a new function that creates sediment from the plates and the master raster:

    sediment.sediment (plates, summaries, sediments) // this function returns empty Strata if plate is not on top, sediment otherwise
    crust.condense    (sediments, sediment)          // this behaves similarly to summarization.condense, except it uses CrustOps.combine() internally
    erosion           (sediment,  delta)             // this is the erosion model, proper
    frames.localize   (plates, delta, deltas)

    FormationErosion
    CrustDisplacement
    CrustBuoyancy
    CrustVelocity
    */

    // NOTE: `M` is mineral count, `F` is formation count
    template <int M, int F>
    class LithosphereTopSediment
    {

        const CrustTopSediment top_sediment;
        const FormationOps ops;

    public:

        LithosphereTopSediment(
            const FormationOps& ops
        ):
            ops(ops)
        {}

        void summarize(
            const std::vector<Crust<M,F>>& plates,
            const std::vector<LithosphereSummary>& summaries,
            std::vector<Formation<M>>& out
        ) const {
            for (std::size_t i = 0; i < plates.size(); ++i)
            {
                top_sediment(i, plates[i], summaries[i], out[i]);
            }
        }

        void flatten (
            const std::vector<Formation<M>>& formations,
            Formation<M>& out
        ) const {
            std::fill(out.begin(), out.end(), StratumStore<M>());
            for (std::size_t i = 0; i < formations.size(); ++i)
            {
                ops.combine(out, formations[i], out);
            }
        }

    };

}

