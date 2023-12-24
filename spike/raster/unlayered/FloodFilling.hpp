#pragma once

#include <deque> // std::deque

namespace unlayered
{

    /*
    properties used:
        arrow_target_vertex_id
    */

    template<typename id, typename scalar, typename IsSimilar>
    struct FloodFilling
    {
        IsSimilar is_similar;

        /* VectorFloodFill(
            [](auto U, auto V){return glm::similarity (U,V) > Math.cos(Math.PI * 60/180)}) */
        FloodFilling(const IsSimilar is_similar):
            is_similar(is_similar)
        {}

        template<typename Grid, typename Raster, typename Mask, typename Scratch, typename Out>
        void fill(const Grid& grid, const Raster& raster, const Mask& mask, const id start_id, 
                   Out& out, Scratch& scratch) const {

            std::deque<id> searching { start_id };
            Scratch& searched = scratch;
            each::copy(series::uniform(false), searched);
            each::copy(series::uniform(0), out);

            searched[start_id] = true;

            id cell_id(0);
            id neighbor_id(0);

            const id N = grid.arrows_per_vertex;
            while(searching.size() > 0){
                cell_id = searching.front();
                searching.pop_front();

                if (is_similar(raster[cell_id], raster[start_id])) {
                    out[cell_id] = true;

                    for (int j = 0; j < N; ++j)
                    {
                        neighbor_id = grid.arrow_target_memory_id(cell_id,j);
                        if (searched[neighbor_id] == 0 && mask[cell_id] != 0) {
                            searching.push_back(neighbor_id);
                            searched[neighbor_id] = 1;
                        }
                    }

                }

            }

        }

    };

    template<typename id, typename scalar, typename IsSimilar>
    auto flood_filling(const IsSimilar is_similar) {
        return FloodFilling<id,scalar,IsSimilar>(is_similar);
    }

}



