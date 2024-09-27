#pragma once

namespace unlayered
{

    /*
    properties used:
        arrow_target_vertex_id
    */

    template<typename ElementMetric>
    struct Voronoi
    {
        const ElementMetric metric;

        /* VectorFloodFill(
            [](auto U, auto V){return glm::similarity (U,V) > Math.cos(Math.PI * 60/180)}) */
        Voronoi(const ElementMetric& metric):
            metric(metric)
        {}

        template<typename Positions, typename Seeds, typename Ids>
        void operator()(const Positions& positions, const Seeds& seeds, Ids& seed_ids) const 
        {
            const auto N = positions.size();
            const auto M = seeds.size();
            for (std::size_t i = 0; i < N; ++i)
            {
                std::size_t min_seed_id = 0;
                auto min_distance = metric.distance(positions[i], seeds[0]);
                for (std::size_t j = 1; j < M; ++j)
                {
                    auto ij_distance = metric.distance(positions[i], seeds[j]);
                    if (ij_distance < min_distance)
                    {
                        min_seed_id = j;
                        min_distance = ij_distance;
                    }
                }
                seed_ids[i] = min_seed_id;
            }
        }

    };

    template<typename ElementMetric>
    auto voronoi(const ElementMetric metric) {
        return Voronoi<ElementMetric>(metric);
    }

}

