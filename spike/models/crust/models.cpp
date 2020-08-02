#pragma once

#include <many/types.hpp>         // floats, etc.

#include <rasters/Grid.hpp>       // Grid

#include <models/rock/Strata.hpp> // Strata

namespace crust
{
    void phase_transition(
        const tmany<StrataValues<float>>& pressure, const tmany<StrataValues<float>>& temperature, 
        tmany<StrataStore>& crust,
        tmany<StrataStore>& solidification_deltas,
        std::array<StratumMassPool, Stratum::mass_pool_count> vaporization_delta&,
        std::array<StratumMassPool, Stratum::mass_pool_count> melting_delta&
    ){
        assert(pressure.size() == solidification_deltas.size());
        assert(temperature.size() == solidification_deltas.size());
        using MassPools = OxygenPlanetStratumMassPoolTypes;
        Strata strata;
        Strata solidification_delta;
        BasicPhase phase;

        for (int i = 0; i < crust.size(); ++i)
        {
            strata.decompress(crust[i]);
            solidification_delta.count = crust[i].count;
            for (int j = 0; j < crust[i].count; ++j)
            {
                // assume the properties of silica for the critical points of any mineral we don't know the properties of
                // assume the properties of olivine for any mafic mineral we don't know the properties of
                // properties for olivine from Nagahara (1994)

                // HEMATITE
                phase = get_basic_phase( pressure[i].values[j]  , temperature[i].values[j],
                //          p0        t0        pc        tc       pf      tf       L          M           a         c      
                            5.2     , 2163    , 1.7e8   , 5300   , 101e3 , 1811   , 543e3/0.141, 0.141   , 0       , 0       , FALSE ,  TRUE  ); // forsterite
                //          0.0063  , 1490    , 1.7e8   , 5300   , 101e3 , 1811   , 502e3/0.204, 0.204   , 0       , 0       , FALSE ,  TRUE  ); // fayalite
                solidification_delta.mass_pools[MassPools::hematite] += phase != BasicPhase::solid? -strata.mass_pools[MassPools::hematite] : 0.0;
                vaporization_delta             [MassPools::hematite] += phase == BasicPhase::vapor?  strata.mass_pools[MassPools::hematite] : 0.0;
                melting_delta                  [MassPools::hematite] +=(phase == BasicPhase::liquid || phase == BasicPhase::supercritical)? strata.mass_pools[MassPools::hematite] : 0.0;

                // PYRITE
                phase = get_basic_phase( pressure[i].values[j]  , temperature[i].values[j],
                //          p0        t0        pc        tc       pf      tf       L          M           a         c      
                            5.2     , 2163    , 1.7e8   , 5300   , 101e3 , 1461   , 543e3/0.141, 0.141   , 0       , 0       , FALSE ,  TRUE  ); // forsterite
                //          0.0063  , 1490    , 1.7e8   , 5300   , 101e3 , 1461   , 502e3/0.204, 0.204   , 0       , 0       , FALSE ,  TRUE  ); // fayalite
                solidification_delta.mass_pools[MassPools::pyrite] += phase != BasicPhase::solid? -strata.mass_pools[MassPools::pyrite] : 0.0;
                vaporization_delta             [MassPools::pyrite] += phase == BasicPhase::vapor?  strata.mass_pools[MassPools::pyrite] : 0.0;
                melting_delta                  [MassPools::pyrite] +=(phase == BasicPhase::liquid || phase == BasicPhase::supercritical)? strata.mass_pools[MassPools::pyrite] : 0.0;

                // OLIVINE
                phase = get_basic_phase( pressure[i].values[j]  , temperature[i].values[j],
                //          p0        t0        pc        tc       pf      tf       L          M           a         c      
                            5.2     , 2163    , 1.7e8   , 5300   , 101e3 , 2173   , 543e3/0.141, 0.141   , 0       , 0       , FALSE ,  TRUE  ); // forsterite
                //          0.0063  , 1490    , 1.7e8   , 5300   , 101e3 , 2073   , 502e3/0.204, 0.204   , 0       , 0       , FALSE ,  TRUE  ); // fayalite
                solidification_delta.mass_pools[MassPools::olivine] += phase != BasicPhase::solid? -strata.mass_pools[MassPools::olivine] : 0.0;
                vaporization_delta             [MassPools::olivine] += phase == BasicPhase::vapor?  strata.mass_pools[MassPools::olivine] : 0.0;
                melting_delta                  [MassPools::olivine] +=(phase == BasicPhase::liquid || phase == BasicPhase::supercritical)? strata.mass_pools[MassPools::olivine] : 0.0;

                // PYROXENE
                phase = get_basic_phase( pressure[i].values[j]  , temperature[i].values[j], 
                //          p0        t0        pc        tc       pf      tf       L          M           a         c         
                            5.2     , 2163    , 1.7e8   , 5300   , 101e3 , 1673   , 543e3/0.141, 0.141   , 0       , 0       , FALSE ,  TRUE  ); // forsterite
                //          0.0063  , 1490    , 1.7e8   , 5300   , 101e3 , 1673   , 502e3/0.204, 0.204   , 0       , 0       , FALSE ,  TRUE  ); // fayalite
                solidification_delta.mass_pools[MassPools::pyroxene] += phase != BasicPhase::solid? -strata.mass_pools[MassPools::pyroxene] : 0.0;
                vaporization_delta             [MassPools::pyroxene] += phase == BasicPhase::vapor?  strata.mass_pools[MassPools::pyroxene] : 0.0;
                melting_delta                  [MassPools::pyroxene] +=(phase == BasicPhase::liquid || phase == BasicPhase::supercritical)? strata.mass_pools[MassPools::pyroxene] : 0.0;

                // assume the properties of silica for any felsic/intermediate mineral we don't know the properties of
                // this applies for basically everything except melting point and molar mass

                // PLAGIOCLASE
                phase = get_basic_phase( pressure[i].values[j]  , temperature[i].values[j], 
                //          p0        t0        pc        tc       pf      tf       L          M           a         c         
                            0.0003  , 1983    , 1.7e8   , 5300   , 101e3 , 1943   , 11770e3  , 0.06008   , 0       , 0       , FALSE ,  TRUE  ); // quartz 
                solidification_delta.mass_pools[MassPools::plagioclase] += phase != BasicPhase::solid? -strata.mass_pools[MassPools::plagioclase] : 0.0;
                vaporization_delta             [MassPools::plagioclase] += phase == BasicPhase::vapor?  strata.mass_pools[MassPools::plagioclase] : 0.0;
                melting_delta                  [MassPools::plagioclase] +=(phase == BasicPhase::liquid || phase == BasicPhase::supercritical)? strata.mass_pools[MassPools::plagioclase] : 0.0;

                // QUARTZ
                phase = get_basic_phase( pressure[i].values[j]  , temperature[i].values[j], 
                //          p0        t0        pc        tc       pf      tf       L          M           a         c         
                            0.0003  , 1983    , 1.7e8   , 5300   , 101e3 , 1943   , 11770e3  , 0.06008   , 0       , 0       , FALSE ,  TRUE  ); // quartz 
                solidification_delta.mass_pools[MassPools::quartz] += phase != BasicPhase::solid? -strata.mass_pools[MassPools::quartz] : 0.0;
                vaporization_delta             [MassPools::quartz] += phase == BasicPhase::vapor?  strata.mass_pools[MassPools::quartz] : 0.0;
                melting_delta                  [MassPools::quartz] +=(phase == BasicPhase::liquid || phase == BasicPhase::supercritical)? strata.mass_pools[MassPools::quartz] : 0.0;

                // ORTHOCLASE
                phase = get_basic_phase( pressure[i].values[j]  , temperature[i].values[j], 
                //          p0        t0        pc        tc       pf      tf       L          M           a         c         
                            0.0003  , 1983    , 1.7e8   , 5300   , 101e3 , 1443   , 11770e3  , 0.06008   , 0       , 0       , FALSE ,  TRUE  ); // quartz 
                solidification_delta.mass_pools[MassPools::orthoclase] += phase != BasicPhase::solid? -strata.mass_pools[MassPools::orthoclase] : 0.0;
                vaporization_delta             [MassPools::orthoclase] += phase == BasicPhase::vapor?  strata.mass_pools[MassPools::orthoclase] : 0.0;
                melting_delta                  [MassPools::orthoclase] +=(phase == BasicPhase::liquid || phase == BasicPhase::supercritical)? strata.mass_pools[MassPools::orthoclase] : 0.0;

                // CALCITE
                phase = get_basic_phase( pressure[i].values[j]  , temperature[i].values[j], 
                //          p0        t0        pc        tc       pf      tf       L          M           a         c         
                            0.0003  , 1983    , 1.7e8   , 5300   , 101e3 , 1612   , 11770e3  , 0.06008   , 0       , 0       , FALSE ,  TRUE  ); // quartz 
                solidification_delta.mass_pools[MassPools::calcite] += phase != BasicPhase::solid? -strata.mass_pools[MassPools::calcite] : 0.0;
                vaporization_delta             [MassPools::calcite] += phase == BasicPhase::vapor?  strata.mass_pools[MassPools::calcite] : 0.0;
                melting_delta                  [MassPools::calcite] +=(phase == BasicPhase::liquid || phase == BasicPhase::supercritical)? strata.mass_pools[MassPools::calcite] : 0.0;




                WaterPhases water_phase = get_water_phase( pressure[i].values[j], temperature[i].values[j]);
                solidification_delta.mass_pools[MassPools::ice] += (water_phase == WaterPhase::vapor || water_phase == WaterPhase::liquid || water_phase == WaterPhase::supercritical)? -strata.mass_pools[MassPools::ice] : 0.0;
                vaporization_delta             [MassPools::ice] +=  water_phase == WaterPhase::vapor?  strata.mass_pools[MassPools::ice] : 0.0;
                melting_delta                  [MassPools::ice] += (water_phase == WaterPhase::liquid || water_phase == WaterPhase::supercritical)? strata.mass_pools[MassPools::ice] : 0.0;

                CarbonPhases carbon_phase = get_carbon_phase( pressure[i].values[j], temperature[i].values[j]);
                solidification_delta.mass_pools[MassPools::organics] += (carbon_phase == CarbonPhases::vapor || carbon_phase == CarbonPhases::liquid || carbon_phase == CarbonPhases::supercritical)? -strata.mass_pools[MassPools::organics] : 0.0;
                vaporization_delta             [MassPools::organics] +=  carbon_phase == CarbonPhases::vapor?  strata.mass_pools[MassPools::organics] : 0.0;
                melting_delta                  [MassPools::organics] += (carbon_phase == CarbonPhases::liquid || carbon_phase == CarbonPhases::supercritical)? strata.mass_pools[MassPools::organics] : 0.0;

                phase = get_basic_phase( pressure[i].values[j]  , temperature[i].values[j], 
                //          p0        t0        pc        tc       pf      tf       L          M           a         c         
                            12.6e3  , 63.1604 , 3390e3  , 126.2  , 101e3 , 63.15  , 199000   , 0.028     , 1607e5  , 1.7910  , TRUE  ,  TRUE  ); // Nitrogen
                solidification_delta.mass_pools[MassPools::nitrogen] += phase != BasicPhase::solid? -strata.mass_pools[MassPools::nitrogen] : 0.0;
                vaporization_delta             [MassPools::nitrogen] += phase == BasicPhase::vapor?  strata.mass_pools[MassPools::nitrogen] : 0.0;
                melting_delta                  [MassPools::nitrogen] +=(phase == BasicPhase::liquid || phase == BasicPhase::supercritical)? strata.mass_pools[MassPools::nitrogen] : 0.0;

                phase = get_basic_phase( pressure[i].values[j]  , temperature[i].values[j], 
                //          p0        t0        pc        tc       pf      tf       L          M           a         c         
                            0.152e3 , 54.383  , 5050e3  , 154.6  , 101e3 , 54.36  , 213000   , 0.032     , 2733e5  , 1.7425  , TRUE  ,  TRUE  ); // Oxygen 
                solidification_delta.mass_pools[MassPools::oxygen] += phase != BasicPhase::solid? -strata.mass_pools[MassPools::oxygen] : 0.0;
                vaporization_delta             [MassPools::oxygen] += phase == BasicPhase::vapor?  strata.mass_pools[MassPools::oxygen] : 0.0;
                melting_delta                  [MassPools::oxygen] +=(phase == BasicPhase::liquid || phase == BasicPhase::supercritical)? strata.mass_pools[MassPools::oxygen] : 0.0;

                phase = get_basic_phase( pressure[i].values[j]  , temperature[i].values[j], 
                //          p0        t0        pc        tc       pf      tf       L          M           a         c         
                            11.7e3  , 90.66   , 4640e3  , 190.8  , 101e3 , 90.68  , 511000   , 0.016     , 2080e5  , 1.698   , TRUE  ,  TRUE  ); // Methane           
                solidification_delta.mass_pools[MassPools::methane] += phase != BasicPhase::solid? -strata.mass_pools[MassPools::methane] : 0.0;
                vaporization_delta             [MassPools::methane] += phase == BasicPhase::vapor?  strata.mass_pools[MassPools::methane] : 0.0;
                melting_delta                  [MassPools::methane] +=(phase == BasicPhase::liquid || phase == BasicPhase::supercritical)? strata.mass_pools[MassPools::methane] : 0.0;

                phase = get_basic_phase( pressure[i].values[j]  , temperature[i].values[j], 
                //          p0        t0        pc        tc       pf      tf       L          M           a         c         
                            15.37e3 , 68.146  , 3500e3  , 132.85 , 101e3 , 68.15  , 216000   , 0.028     , 0       , 0       , FALSE ,  TRUE  ); // Carbon Monoxide   
                solidification_delta.mass_pools[MassPools::carbon_monoxide] += phase != BasicPhase::solid? -strata.mass_pools[MassPools::carbon_monoxide] : 0.0;
                vaporization_delta             [MassPools::carbon_monoxide] += phase == BasicPhase::vapor?  strata.mass_pools[MassPools::carbon_monoxide] : 0.0;
                melting_delta                  [MassPools::carbon_monoxide] +=(phase == BasicPhase::liquid || phase == BasicPhase::supercritical)? strata.mass_pools[MassPools::carbon_monoxide] : 0.0;

                phase = get_basic_phase( pressure[i].values[j]  , temperature[i].values[j], 
                //          p0        t0        pc        tc       pf      tf       L          M           a         c         
                            517e3   , 216.56  , 7380e3  , 304.19 , 101e3 , 216.6  , 205000   , 0.044     , 4000e5  , 2.60    , TRUE  ,  TRUE  ); // Carbon Dioxide    
                solidification_delta.mass_pools[MassPools::carbon_dioxide] += phase != BasicPhase::solid? -strata.mass_pools[MassPools::carbon_dioxide] : 0.0;
                vaporization_delta             [MassPools::carbon_dioxide] += phase == BasicPhase::vapor?  strata.mass_pools[MassPools::carbon_dioxide] : 0.0;
                melting_delta                  [MassPools::carbon_dioxide] +=(phase == BasicPhase::liquid || phase == BasicPhase::supercritical)? strata.mass_pools[MassPools::carbon_dioxide] : 0.0;
            }
            solidification_deltas[i].compress(solidification_delta);
        }
    }


    void get_arrow_slope(
        const SpheroidGrid&   grid,
        const tmany<float>&   vertex_height,
        tmany<float>&         arrow_height
    ){
        assert( vertex_height.size()  == grid.vertex_count );
        assert( arrow_height   .size()  == grid.arrow_count  );
        uint from, to;
        for (int i = 0; i < grid.arrow_count; ++i)
        {
            from = grid.arrow_vertex_ids[i].x;
            to   = grid.arrow_vertex_ids[i].y;
            arrow_height[i] = (vertex_height[to] - vertex_height[from]) / grid.arrow_lengths[i];
        }
    }

    void get_arrow_discharge(
        const SpheroidGrid&   grid,
        const tmany<float>&   vertex_precipitation,
        const tmany<float>&   arrow_slope,
        tmany<float>&         arrow_discharge,
        tmany<float>&         scratch,
    ){
        assert( vertex_precipitation.size()  == grid.vertex_count );
        assert( arrow_slope         .size()  == grid.arrow_count  );
        assert( arrow_discharge     .size()  == grid.arrow_count  );
        uint from;
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
        const tmany<Stratum>& sediment, 
        const tmany<float>&   arrow_slope, 
        const tmany<float>&   arrow_discharge, 
        const float           seconds, 
        tmany<Stratum>&       outgoing, 
        tmany<Stratum>&       incoming,
        tmany<float>&         vertex_capacity, // scratch
        tmany<float>&         vertex_outgoing, // scratch
        tmany<float>&         arrow_capacity   // scratch
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
        const tmany<StrataStore>& crust, 
        const tmany<float>& max_height_difference, 
        const tmany<float>& precipitation, 
        const float seconds, 
        tmany<StrataStore>& weathering_delta, 
        tmany<Stratum>& deposited
    ){
        const float erosive_factor(1.8e-7);
        Strata strata;
        for (int i = 0; i < grid.vertex_count; ++i)
        {
            float eroded_thickness = max_height_difference[i] * erosive_factor * precipitation[i];
            crust[i].decompress(strata);
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

