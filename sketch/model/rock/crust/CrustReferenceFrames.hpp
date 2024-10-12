#pragma once

#include <.hpp>

#include <index/procedural/Map.hpp>     // procedural::map

#include <field/Compose.hpp>        // field::compose
#include <field/Affinity.hpp>       // field::affinity

#include <grid/dymaxion/series.hpp> // dymaxion::NearestVertexIds, dymaxion::VertexPositions

// in house libraries

namespace rock{

    /*
    `CrustReferenceFrames` handles a common use case where 
    the rasters of multiple plates must be resampled onto a single global grid system
    so that they can interact with one another.
    */
    class CrustReferenceFrames
    {

        const dymaxion::Grid grid;
        const dymaxion::NearestVertexId nearest;
        const dymaxion::VertexPositions positions;

    public:

        CrustReferenceFrames(const dymaxion::Grid& grid): 
            nearest(grid),
            positions(grid)
        {}

        template<typename In1, typename Out>
        void resample(
            const glm::dmat3& transform,
            const In1& input,
            Out& output
        ) const {
            /*
                        grid      rotation    grid
            resample = id ⟶ position ⟶ position ⟶ id
            */
            auto resample = 
                procedural::map(
                    field::compose(nearest, field::Affinity(transform[i])),
                    positions
                );
            iterated::Index{}(input, resample, output[i]);
        }

    };

}

