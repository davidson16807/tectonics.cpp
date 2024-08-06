#pragma once

#include <series/types.hpp>         // floats, etc.

#include <model/rock/Strata.hpp> // Strata

namespace crust
{

    /*

    `CrustPhaseChange` (together with `FormationSimulation`)
    is basically *the most important part of the geological model*.
    They contain everything that's needed to simulate the geology of a planet.

    This may include the following:
    * weathering     the conversion of rock to sediment
    * erosion        the horizontal transport of sediment
    * astroblemes    the removal of mass as a function of distance from a prescribed geocoordinate

    */
    void get_arrow_slope(
        const SpheroidGrid&   grid,
        const series::Series<float>&   vertex_height,
        series::Series<float>&         arrow_height
    ){
        assert( vertex_height.size()  == grid.vertex_count );
        assert( arrow_height   .size()  == grid.arrow_count  );
        unsigned int from, to;
        for (int i = 0; i < grid.arrow_count; ++i)
        {
            from = grid.arrow_vertex_ids[i].x;
            to   = grid.arrow_vertex_ids[i].y;
            arrow_height[i] = (vertex_height[to] - vertex_height[from]) / grid.arrow_lengths[i];
        }
    }

    void get_arrow_discharge(
        const SpheroidGrid&   grid,
        const series::Series<float>&   vertex_precipitation,
        const series::Series<float>&   arrow_slope,
        series::Series<float>&         arrow_discharge,
        series::Series<float>&         scratch,
    ){
        assert( vertex_precipitation.size()  == grid.vertex_count );
        assert( arrow_slope         .size()  == grid.arrow_count  );
        assert( arrow_discharge     .size()  == grid.arrow_count  );
        unsigned int from;
        const int iteration_count = 3;
        fill(vertex_slope_sum, 0.0f);
        for (int i = 0; i < grid.arrow_count; ++i)
        {
            from = grid.arrow_vertex_id_from[i];
            vertex_slope_sum[from] += max(arrow_slope[i], 0.0f);
        }
        fill(arrow_fraction_discharged, 0.0f);
        for (int i = 0; i < grid.arrow_count; ++i)
        {
            from = grid.arrow_vertex_id_from[i];
            arrow_fraction_discharged[i] = max(arrow_slope[i], 0.0f) / vertex_slope_sum[from];
        }
        copy(vertex_incoming, vertex_precipitation);
        for (int j = 0; j < iteration_count; ++j)
        {
            for (int i = 0; i < grid.arrow_count; ++i)
            {
                from = grid.arrow_vertex_ids[i].x;
                to   = grid.arrow_vertex_ids[i].y;
                vertex_incoming_updated[from] -= vertex_incoming[from] * arrow_fraction_discharged[i];
                vertex_incoming_updated[to]   += vertex_incoming[from] * arrow_fraction_discharged[i];
                arrow_discharge[i] += vertex_incoming[from] * arrow_fraction_discharged[i];
            }
            copy(vertex_incoming, vertex_incoming_updated);
        }
    }

    // model based on Simoes (2010)
    void erosion(
        const SpheroidGrid&   grid,
        const series::Series<Stratum>& sediment, 
        const series::Series<float>&   arrow_slope, 
        const series::Series<float>&   arrow_discharge, 
        const float           seconds, 
        series::Series<Stratum>&       outgoing, 
        series::Series<Stratum>&       incoming,
        series::Series<float>&         vertex_capacity, // scratch
        series::Series<float>&         vertex_outgoing, // scratch
        series::Series<float>&         arrow_capacity   // scratch
    ) {
        const float transport_coefficient(0.0036); // fraction, from Simoes (2010)

        assert( sediment  .size()  == grid.vertex_count );
        assert( outgoing  .size()  == grid.vertex_count );
        assert( incoming  .size()  == grid.vertex_count );
        assert( vertex_capacity .size() == grid.vertex_count );
        assert( vertex_outgoing .size() == grid.vertex_count );
        assert( arrow_slope     .size()  == grid.arrow_count  );
        assert( arrow_discharge .size()  == grid.arrow_count  );
        assert( arrow_capacity  .size() == grid.arrow_count  );

        // Find sediment capacity between a pair of cells.
        // This is the maximum possible volume of sediment that can be transported across a pair of cells by flowing water.
        // It is a fraction of the volume of the water flowing between the cells (the "discharge")
        fill(arrow_capacity, 0.0f);
        for (int i = 0; i < grid.arrow_count; ++i)
        {
            arrow_capacity[i] = min(-transport_coefficient * arrow_discharge[i] * arrow_slope[i], 0.0f);
        }

        // Find sediment capacity for the vertex: the maximum volume of sediment that can leave the cell by flowing water.
        fill(vertex_capacity, 0.0f);
        for (int i = 0; i < grid.arrow_count; ++i)
        {
            from = grid.arrow_vertex_id_from[i];
            vertex_capacity[from] += arrow_capacity[i];
        }

        // Find the actual volume of sediment transported: outgoing amount cannot exceed the amount available in the cell
        fill(vertex_outgoing, 0.0f);
        for (int i = 0; i < grid.vertex_count; ++i)
        {
            vertex_outgoing[i] = min(vertex_capacity[i], sediment[i].volume()); 
        }

        // Find actual volume of sediment transported between cells
        // by scaling capacity by the fraction of capacity for the vertex that is used
        fill(arrow_outgoing_volume, 0.0f);
        for (int i = 0; i < grid.arrow_count; ++i)
        {
            from = grid.arrow_vertex_id_from[i];
            arrow_outgoing_volume[i] = arrow_capacity[i] * vertex_outgoing[from] / vertex_capacity[from];
        }

        // Scale mass pools by the amount transported
        fill(outgoing, 0.0f);
        fill(incoming, 0.0f);
        Stratum transported;
        for (int i = 0; i < grid.arrow_count; ++i)
        {
            from = grid.arrow_vertex_ids[i].x;
            to   = grid.arrow_vertex_ids[i].y;
            Stratum::scale  (sediment[from], arrow_outgoing_volume[i] / sediment[i].volume(), transported);
            Stratum::combine(outgoing[from], transported, outgoing[from]);
            Stratum::combine(incoming[to],   transported, incoming[to]  );
        }
    }


    /*
    
    */
    void weathering(
        const series::Series<StrataStore>& crust, 
        const series::Series<float>& max_height_difference, 
        const series::Series<float>& precipitation, 
        const float seconds, 
        series::Series<StrataStore>& weathering_delta, 
        series::Series<Stratum>& deposited
    ){
        const float erosive_factor(1.8e-7);
        Strata strata;
        for (int i = 0; i < grid.vertex_count; ++i)
        {
            float eroded_thickness = max_height_difference[i] * erosive_factor * precipitation[i];
            crust[i].unpack(strata);
            float running_thickness(strata.strata[0].thickness());
            for (int j = 0; j < max_stratum_count && eroded_thickness < running_thickness; ++j)
            {
                
            }
            if (eroded_thickness > running_thickness)
            {
                /* do something with remaining layer */
            }
        }
    }
}

