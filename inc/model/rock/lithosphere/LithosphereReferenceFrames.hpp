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
    `LithosphereReferenceFrames` handles a common use case where 
    the rasters of multiple plates must be resampled onto a single global grid system
    so that they can interact with one another.
    */
    template<typename id, typename scalar, typename mat>
    class LithosphereReferenceFrames
    {

        const dymaxion::NearestVertexId<id,scalar> nearest;
        const dymaxion::VertexPositions<id,scalar> positions;
        const iterated::Index index;

    public:

        LithosphereReferenceFrames(const dymaxion::Grid<id,scalar>& grid):
            nearest(grid),
            positions(grid),
            index()
        {}

        template<typename Raster>
        void globalize(
            const std::vector<mat>& locals_to_globals,
            const std::vector<Raster>& locals,
            std::vector<Raster>& globals
        ) const {
            for (std::size_t i = 0; i < locals_to_globals.size(); ++i)
            {
                /*
                            grid      rotation    grid
                resample = id ⟶ position ⟶ position ⟶ id
                */
                auto resample = 
                    procedural::map(
                        field::compose(nearest, field::Affinity(locals_to_globals[i])),
                        positions
                    );
                index(locals[i], resample, globals[i]);
            }
        }

        template<typename Raster>
        void localize(
            const std::vector<mat> global_to_locals,
            const Raster& global,
            std::vector<Raster>& locals
        ) const {
            for (std::size_t i = 0; i < global_to_locals.size(); ++i)
            {
                /*
                            grid      rotation    grid
                resample = id ⟶ position ⟶ position ⟶ id
                */
                auto resample = 
                    procedural::map(
                        field::compose(nearest, field::Affinity(global_to_locals[i])),
                        positions
                    );
                index(global, resample, locals[i]);
            }
        }

    };

    /*
    basic use case:
    summarization.summarize (plates,    locals, scratch)     // summarize each plate into (e.g.) a FormationSummary raster
    frames       .globalize (rotations, locals, globals)     // resample plate-specific rasters onto a global grid
    summarization.flatten   (globals,   master)              // flatten globalized rasters into (e.g.) a LithosphereSummary
    frames       .localize  (rotations, master, derivatives) // resample global raster to a plate-specific for each plate
    */

}

