#pragma once

#include <index/procedural/Map.hpp>     // procedural::map

#include <field/Compose.hpp>        // field::compose
#include <field/Affinity.hpp>       // field::affinity

#include <grid/dymaxion/Grid.hpp>   // dymaxion::Grid

#include <grid/dymaxion/series.hpp> // dymaxion::VertexPositions
#include <grid/dymaxion/field.hpp> // dymaxion::NearestVertexId

// in house libraries

namespace rock{

    /*
    `CrustReferenceFrames` handles a common use case where 
    a raster for a plate must be resampled onto a single global grid system
    so that it can interact with others.
    */
    template<typename id, typename scalar, typename mat>
    class CrustReferenceFrames
    {

        const dymaxion::NearestVertexId<id,scalar> nearest;
        const dymaxion::VertexPositions<id,scalar> positions;
        const iterated::Index index;

    public:

        CrustReferenceFrames(const dymaxion::Grid<id,scalar>& grid):
            nearest(grid),
            positions(grid),
            index()
        {}

        template<typename Raster>
        void globalize(
            const mat& locals_to_global,
            const Raster& local,
            Raster& global
        ) const {
            /*
                        grid      rotation    grid
            resample = id ⟶ position ⟶ position ⟶ id
            */
            auto resample = 
                procedural::map(
                    field::compose(nearest, field::Affinity(locals_to_global)),
                    positions
                );
            index(local, resample, global);
        }

        template<typename Raster>
        void localize(
            const mat locals_to_global,
            const Raster& global,
            Raster& local
        ) const {
            /*
                        grid      rotation    grid
            resample = id ⟶ position ⟶ position ⟶ id
            */
            auto resample = 
                procedural::map(
                    field::compose(nearest, field::Affinity(glm::inverse(locals_to_global))),
                    positions
                );
            index(global, resample, local);
        }

    };

    /*
    basic use case:
    summarization.summarize (plate,    local, scratch)     // summarize each plate into (e.g.) a FormationSummary raster
    frames       .globalize (rotation, local, globals)     // resample plate-specific rasters onto a global grid
    summarization.flatten   (global,   master)              // flatten globalized rasters into (e.g.) a LithosphereSummary
    frames       .localize  (rotation, master, derivatives) // resample global raster to a plate-specific for each plate
    */

}

