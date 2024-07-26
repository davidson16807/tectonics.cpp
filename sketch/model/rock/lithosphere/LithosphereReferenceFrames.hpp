#pragma once

#include <.hpp>

#include <index/each.hpp>           // each::get
#include <index/series/Map.hpp>     // series::map

#include <field/Compose.hpp>        // field::compose
#include <field/Affinity.hpp>       // field::affinity

#include <grid/dymaxion/series.hpp> // dymaxion::NearestVertexIds, dymaxion::VertexPositions

// in house libraries

namespace rock{

    /*
    `LithosphereReferenceFrames` handles a common use case where 
    the rasters of multiple plates must be resampled onto a single global grid system
    so that they can interact with one another.
    */
    class LithosphereReferenceFrames
    {

        const dymaxion::Grid grid;
        const dymaxion::NearestVertexId nearest;
        const dymaxion::VertexPositions positions;

    public:

        LithosphereReferenceFrames(const dymaxion::Grid& grid): 
            nearest(grid),
            positions(grid),
        {}

        template<typename Raster>
        void globalize(
            const std::vector<glm::dmat3>& locals_to_globals,
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
                    series::map(
                        field::compose(nearest, field::Affinity(locals_to_globals[i])),
                        positions
                    );
                each::get(locals, resample, globals[i]);
            }
        }

        template<typename Raster>
        void localize(
            const std::vector<glm::dmat3> global_to_locals,
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
                    series::map(
                        field::compose(nearest, field::Affinity(global_to_locals[i])),
                        positions
                    );
                each::get(global, resample, locals[i]);
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

