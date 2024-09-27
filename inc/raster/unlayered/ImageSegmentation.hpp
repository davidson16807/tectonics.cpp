#pragma once

#include <deque> // std::deque

#include <index/iterated/Nary.hpp>
#include <index/iterated/Metric.hpp>

namespace unlayered
{

    /*
    properties used:
        arrow_target_vertex_id
    */

    template<typename id, typename scalar, typename FloodFilling, typename ElementMetric>
    struct ImageSegmentation
    {
        const FloodFilling fill;
        const iterated::Metric<ElementMetric> metric;
        const iterated::Ternary ternary;
        const iterated::Identity copy;

        /* VectorFloodFill(
            [](auto U, auto V){return glm::similarity (U,V) > Math.cos(Math.PI * 60/180)}) */
        ImageSegmentation(const FloodFilling fill, const ElementMetric submetric):
            fill(fill),
            metric(submetric),
            ternary(),
            copy()
        {}

        template<typename Grid, typename Raster, typename ScratchScalars, typename ScratchMask, typename Out>
        void operator()(const Grid& grid, const Raster& raster, 
            const int segment_count, const int min_segment_vertex_count, 
            Out& out, ScratchScalars& scratch4, ScratchMask& scratch1, ScratchMask& scratch2, ScratchMask& scratch3) const {

          const int max_iterations = 2 * segment_count;

          ScratchScalars& magnitude = scratch4;
          metric.length(raster, magnitude);
          Out& segments = out;
          copy(series::uniform(0), segments);
          ScratchMask& occupied = scratch2;
          copy(series::uniform(1), occupied);
          ScratchMask& segment = scratch1;

          // step 1: run flood fill algorithm several times
          for (int i=1, j=0; i<segment_count && j<max_iterations; j++) {

            // const Grid& grid, const Raster& raster, const Mask& mask, const id start_id, 
            //        Out& out, ScratchMask& scratch
            // raster, max_id(magnitude), occupied, segment,         scratch_ui8_3
            // vector_raster, start_id, mask, result, scratch_ui8

            fill(grid, raster, occupied, whole::max_id(magnitude), segment, scratch3);
            ternary(segment, series::uniform(0), magnitude, magnitude);
            ternary(segment, series::uniform(0), occupied, occupied);
            if (whole::sum<int>(segment) > min_segment_vertex_count) {
                ternary(segment, series::uniform(i), segments, segments);
                i++;
            }

          }

        }

    };

    template<typename id, typename scalar, typename FloodFilling, typename ElementMetric>
    auto image_segmentation(const FloodFilling fill, const ElementMetric submetric) {
        return ImageSegmentation<id,scalar,FloodFilling,ElementMetric>(fill, submetric);
    }

}

