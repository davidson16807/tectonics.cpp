#pragma once

#include <deque> // std::deque

#include <index/iterated/Nary.hpp>
#include <index/iterated/Metric.hpp>
#include <index/aggregated/Arithmetic.hpp>
#include <index/aggregated/Order.hpp>

namespace unlayered
{

    /*
    properties used:
        arrow_target_vertex_id
    */

    template<typename id, typename scalar, typename FloodFilling, 
             typename ElementMetric, typename ElementOrder, typename ElementArithmetic>
    struct ImageSegmentation
    {
        const FloodFilling fill;
        const iterated::Metric<ElementMetric> metric;
        const aggregated::Arithmetic<ElementArithmetic> arithmetic;
        const aggregated::Order<ElementOrder> order;
        const iterated::Ternary ternary;
        const iterated::Identity copy;

        /* VectorFloodFill(
            [](auto U, auto V){return glm::similarity (U,V) > Math.cos(Math.PI * 60/180)}) */
        ImageSegmentation(
                const FloodFilling fill, 
                const ElementMetric submetric, 
                const ElementArithmetic subarithmetic,
                const ElementOrder suborder
        ):
            fill(fill),
            metric(submetric),
            arithmetic(subarithmetic),
            order(suborder),
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
          copy(procedural::uniform(0), segments);
          ScratchMask& occupied = scratch2;
          copy(procedural::uniform(1), occupied);
          ScratchMask& segment = scratch1;

          // step 1: run flood fill algorithm several times
          for (int i=1, j=0; i<segment_count && j<max_iterations; j++) {
            fill(grid, raster, occupied, order.max_id(magnitude), segment, scratch3);
            ternary(segment, procedural::uniform(0), magnitude, magnitude);
            ternary(segment, procedural::uniform(0), occupied, occupied);
            if (arithmetic.sum(segment) > min_segment_vertex_count) {
                ternary(segment, procedural::uniform(i), segments, segments);
                i++;
            }

          }

        }

    };

    template<typename id, typename scalar, typename FloodFilling, 
             typename ElementMetric, typename ElementOrder, typename ElementArithmetic>
    auto image_segmentation(const FloodFilling fill, 
            const ElementMetric submetric, 
            const ElementArithmetic subarithmetic,
            const ElementOrder suborder
    ) {
        return ImageSegmentation<id,scalar,FloodFilling,ElementMetric, ElementOrder, ElementArithmetic>(fill, submetric, subarithmetic, suborder);
    }

}

