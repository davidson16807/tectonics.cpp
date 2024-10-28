#pragma once

// standard libraries
#include <queue> // std::priority_queue

// in-house libraries
#include <index/iterated/Nary.hpp>

#include "FloodFillCandidateComparison.hpp"

namespace unlayered
{

    template<typename id, typename scalar>
    struct SeedBasedFloodFillState
    {
        using Candidate = std::pair<id,scalar>;
        std::priority_queue<Candidate, std::vector<Candidate>, FloodFillCandidateComparison<id,scalar>> candidates;
        std::vector<bool> is_included;
        const id seed_id;
        SeedBasedFloodFillState(const id seed_id_, const id vertex_count):
            candidates(FloodFillCandidateComparison<id,scalar>{}),
            is_included(vertex_count, false),
            seed_id(seed_id_)
        {
            candidates.emplace(seed_id_, scalar(0));
            is_included[seed_id_] = true;
        }
        SeedBasedFloodFillState(const SeedBasedFloodFillState& state):
            candidates(state.candidates),
            is_included(state.is_included),
            seed_id(state.seed_id)
        {}
    };

    /*
    grid attributes used:
        arrow_target_vertex_id
    */

    /*
    properties to satisfy:
    * contiguous (requires floodfill)
    * symmetric with respect to plate seeds
    * cells with largest vectors must occur at boundaries 
    * lowest complexity allowable
    * in-order traversal if possible

    ideas:
    * priority queue for candidates, sorting on seed proximity or vector magnitude

    weights:

    */

    template<typename id, typename scalar, typename IsSimilar, typename Priority>
    struct SeedBasedFloodFilling
    {
        const IsSimilar is_similar;
        const Priority priority;
        const iterated::Identity copy;

        /* VectorFloodFill(
            [](auto U, auto V){return glm::similarity (U,V) > Math.cos(Math.PI * 60/180)}) */
        SeedBasedFloodFilling(
            const IsSimilar is_similar, 
            const Priority& priority
        ):
            is_similar(is_similar),
            priority(priority),
            copy()
        {}

        template<typename Grid, typename Raster, typename Mask>
        void advance(
            const Grid& grid, 
            const Raster& raster, 
            Mask& is_considered, 
            SeedBasedFloodFillState<id,scalar>& io
        ) const {

            // float average_separation(0.08);
            // auto similarity = [average_separation](auto A, auto U, auto O, auto V) { 
            //     /* 
            //     We return true if fracture does not occur.
            //     We start with the assumption that microfractures are sufficiently common 
            //     such that plates are uniformly weak according to a single 
            //     empirically derived value for stress (θ) beyond which fracture occurs.
            //     So fracture does not occur if stress falls below θ.
            //     Stress (forcer per unit area, F/A) is proportionate to virtual displacement (ΔL),
            //     which can be calculated for two cells as follows:

            //     A+U   L+ΔL  B+V
            //        ∘-------∘
            //       U ↖     ↗ V
            //          ∘---∘
            //         A  L  B

            //     where A and B are velocities, U and V are velocities, and L is vertex distance

            //     The specific equation that relates stress to virtual displacement is as follows: 

            //     F/A = E ΔL/L 

            //     where E is young's modulus, which in this context can only be derived empirically.

            //     `vertex_gradient` has magnitudes somewhere between 0 and 9000,
            //     and we say that tangibly sized vectors (judging by our 3d render) are generally above 3000.
            //     Fracture should absolutely occur between two tangible vectors of opposite sign,
            //     but fracture should generally not occur between a tangible vector and a zero vector.
            //     This places our displacement threshold somewhere around 6000<θ<9000 when L=1.
            //     However grid radius is 2 and there are many vertices, given by `grid.vertices_per_meridian`.
            //     By our calculations cell separation is typically 0.001 so θ must be 1000 times larger 
            //     than the estimate above.

            //     To allow the use of the existing floodfill algorithm, 
            //     we further assume that neighboring cells are of constant distance apart 
            //     and are offset by a vector that is aligned with the offset from the seed cell
            //     to the current cell being considered at any iteration
            //     */
            //     // count += 1.0f;
            //     // sum += glm::distance(A+U,B+V) - glm::distance(A,B);
            //     return math::similarity(U,V) <= 0.5;
            //     // return true;
            //     // auto B = A + average_separation*glm::normalize(O-A);
            //     // if (glm::distance(B,A) > 0.0001)
            //     // {
            //     // }
            //     // auto displacement = (std::abs(glm::distance(A+U,B+V)-average_separation) / average_separation);
            //     // return displacement;
            // };


            int cell_id(0);
            int neighbor_id(0);
            glm::vec3 neighbor_position;

            const int N = grid.arrows_per_vertex;
            if(io.candidates.size() > 0){
                cell_id = io.candidates.top().first;
                io.candidates.pop();

                if (is_similar(
                        grid.vertex_position(cell_id), raster[cell_id], 
                        grid.vertex_position(io.seed_id), raster[io.seed_id])) {
                    io.is_included[cell_id] = true;

                    for (int j = 0; j < N; ++j)
                    {
                        neighbor_id = grid.arrow_target_id(cell_id,j);
                        neighbor_position = grid.vertex_position(neighbor_id);
                        if (is_considered[neighbor_id]) {
                            io.candidates.emplace(neighbor_id, 
                                priority(
                                    grid.vertex_position(cell_id), raster[cell_id], 
                                    grid.vertex_position(io.seed_id), raster[io.seed_id])
                            );
                            is_considered[neighbor_id] = 0;
                        }
                    }

                }

            }

        }

    };

    template<typename id, typename scalar, typename IsSimilar, typename Priority>
    auto seed_based_flood_filling(const IsSimilar is_similar, const Priority priority) {
        return SeedBasedFloodFilling<id,scalar,IsSimilar,Priority>(is_similar, priority);
    }

}

