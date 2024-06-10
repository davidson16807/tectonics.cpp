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

    public:

        LithosphereReferenceFrames(const dymaxion::Grid& grid): grid(grid)
        {}

        template<typename Raster>
        void globalize(
            const std::vector<glm::dmat3>& locals_to_globals,
            const std::vector<Raster>& local,
            std::vector<Raster>& global
        ) const {
            for (std::size_t i = 0; i < plates.size(); ++i)
            {
                /*
                            grid      rotation    grid
                resample = id ⟶ position ⟶ position ⟶ id
                */
                auto resample = 
                    series::map(
                        field::compose(
                            dymaxion::NearestVertexId(grid), 
                            field::Affinity(locals_to_globals[i])),
                        dymaxion::VertexPositions(grid)
                    );
                each::get(raster, resample, global[i]);
            }
        }

        template<typename Raster>
        void localize(
            const std::vector<glm::dmat3> globals_to_locals,
            const Raster& global,
            std::vector<Raster>& local
        ) const {
            for (std::size_t i = 0; i < plates.size(); ++i)
            {
                /*
                            grid      rotation    grid
                resample = id ⟶ position ⟶ position ⟶ id
                */
                auto resample = 
                    series::map(
                        field::compose(
                            dymaxion::NearestVertexId(grid), 
                            field::Affinity(globals_to_locals[i])),
                        dymaxion::VertexPositions(grid)
                    );
                each::get(global, resample, local[i]);
            }
        }

    };

    /*
    basic use case:
    summarization.summarize (plates,    locals, scratch)     // summarize each plate into a (e.g.) FormationSummary raster
    frames       .globalize (rotations, locals, globals)     // resample plate-specific rasters onto a global grid
    summarization.condense  (globals,   master)              // condense globalized rasters into e.g. LithosphereSummary
    frames       .localize  (rotations, master, derivatives) // resample global raster to a plate-specific for each plate
    */

}

