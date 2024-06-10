#pragma once

#include <deque> // std::deque

namespace unlayered
{

    /*
    properties used:
        arrow_target_vertex_id
    */

    template<typename id, typename scalar, typename FloodFilling>
    struct ImageSegmentation
    {
        FloodFilling filling;

        /* VectorFloodFill(
            [](auto U, auto V){return glm::similarity (U,V) > Math.cos(Math.PI * 60/180)}) */
        ImageSegmentation(const FloodFilling filling):
            filling(filling)
        {}

        template<typename Grid, typename Raster, typename ScratchScalars, typename ScratchMask, typename Out>
        void segment(const Grid& grid, const Raster& raster, 
            const int segment_count, const int min_segment_vertex_count, 
            Out& out, ScratchScalars& scratch4, ScratchMask& scratch1, ScratchMask& scratch2, ScratchMask& scratch3) {

          const int max_iterations = 2 * segment_count;

          ScratchScalars& magnitude = scratch4;
          each::length(raster, magnitude);
          Out& segments = out;
          each::copy(series::uniform(0), segments);
          ScratchMask& occupied = scratch2;
          each::copy(series::uniform(1), occupied);
          ScratchMask& segment = scratch1;

          // step 1: run flood fill algorithm several times
          for (int i=1, j=0; i<segment_count && j<max_iterations; j++) {

            // const Grid& grid, const Raster& raster, const Mask& mask, const id start_id, 
            //        Out& out, ScratchMask& scratch
            // raster, max_id(magnitude), occupied, segment,         scratch_ui8_3
            // vector_raster, start_id, mask, result, scratch_ui8

            filling.fill(grid, raster, occupied, whole::max_id(magnitude), segment, scratch3);
            each::copy_if(magnitude, series::uniform(0), segment, magnitude);
            each::copy_if(occupied,  series::uniform(0), segment, occupied);
            if (whole::sum<int>(segment) > min_segment_vertex_count) {
                each::copy_if(segments, series::uniform(i), segment, segments);
                i++;
            }

          }

        }

    };

    template<typename id, typename scalar, typename FloodFilling>
    auto image_segmentation(const FloodFilling filling) {
        return ImageSegmentation<id,scalar,FloodFilling>(filling);
    }

}

