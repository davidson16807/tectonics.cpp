#pragma once

// C libraries
#include <cmath>
#include <cstdint>
#include <cstddef>

// std libraries
#include <algorithm>
#include <array>

// in-house libraries
#include "StratumMassPool.hpp"

namespace strata
{
    // NOTE: M = 24 requires three cache lines of storage, 
    // but we need to store a few small variables within Stratum like particle_size, 
    // so we have to set M just shy of that to avoid wasting a cache line.
    const int oxygen_planet_mass_pool_count = 15;

    enum struct CarbonPlanetStratumMassPoolTypes
    {
        // NOTE: see https://www.ptable.com/#Compound/C for ideas
     // chalcophile, //                    
        iron_carbide,//           Fe     C 
        pyrite,      //           Fe     S 
        silicon_carbide,//     SiC 
        calcium_carbide,//   (Ca)C
        aluminum_carbide,//     (Al,Ca)C 
        metal_cyanides, // (Na,K,H)CN
        metal_carbonates, // (Mg)CO3
        organics,    // CNOHPSCaK...       
        graphite,    // C                  
        ice,         // H2O                
        carbon_dioxide,//CO2               
        co,          // CO                 
        oxygen,      // O2                 
        carbon_monoxide,//CO               
        methane,     // CH4                
     // ethane,      // C2H6               
     // tholins,     // CNOHS              
     // helium,      // He                 
     // hydrogen,    // H2, metallic       
    };

    enum struct OxygenPlanetStratumMassPoolTypes
    {
     // magnetite,   //           Fe     O for the surfaces of mars and maybe venus, representative of siderophile ores
     // chalcophile, //                    for the surfaces of maybe venus and mercury, representative of chalcophile ores
        hematite,    //           Fe     O for the early earth, representative of iron oxides and red soils in general, and siderophile ores
        pyrite,      //           Fe     S for the surfaces of maybe venus and mercury, representative of siderophile ores
     // goethite,    //           Fe     O for the surface of mars, representative of iron oxides and red soils in general, and siderophile ores
        olivine,     //       (Mg,Fe)  SiO for ultramafic rocks
        pyroxene,    // (Na,Ca,Mg,Fe)AlSiO for mafic rocks
        plagioclase, // (Na,Ca)      AlSiO for intermediate rocks
        quartz,      //                SiO for distinguishing noteworthy felsic rocks, namely sand, and biomineral deposits like diatomaceous earth
        orthoclase,  //  K           AlSiO for felsic rocks
        calcite,     // CaCO3              for biomineral deposits like limestone
        organics,    // CNOHPSCaK...       representative of peat, coal, oil shale, diamond, and tholins
     // graphite,    // C                  for diamonds, carbon planets, and theoretical biominerals deposits
     // apatite,     // XCa5(PO4)3         for theoretical biomineral deposits
     // corundum,    // Al2O3              representative of precious stones, excluding diamond
        carbon_dioxide,//CO2               for the ice caps of mars
        oxygen,      // O2                 for conserving mass if an earth like planet froze (such as in "a bucket of air")
        ice,         // H2O                for the ice caps of earth and the surfaces of europa and pluto
        carbon_monoxide,//CO               for the surface of pluto and possibly carbon planets
        nitrogen,    // N2                 for the surfaces of pluto or triton
        methane,     // CH4                for the surfaces of pluto and other kuiper belt objects
     // halite,      // NaCl               for salt bed flats and other cool things
     // ethane,      // C2H6               for conserving mass if, for instance, Titan's lakes froze
     // tholins,     // CNOHS              for the surfaces of pluto and kuiper belt objects, and representative of prebiotic chemistry
     // helium,      // He                 for completeness, and padding to fit on cache lines
     // hydrogen,    // H2, metallic       for completeness, and padding to fit on cache lines
    };
    constexpr std::array<float, oxygen_planet_mass_pool_count> oxygen_planet_mass_pool_densities {
        // 5000.0f, //magnetite   
        //          //chalcophile 
           5300.0f, //hematite    
           5010.0f, //pyrite      
        // 3800.0f  //goethite    
           3320.0f, //olivine     
           3225.0f, //pyroxene    
           2685.0f, //plagioclase 
           2633.0f, //quartz      
           2560.0f, //orthoclase  
           2710.0f, //calcite     
           1500.0f, //organics    // density of coal
        // 2260.0f, //graphite    
        // 3180.0f, //apatite     
        // 4020.0f, //corundum    
           1562.0f, //co2         
           1524.0f, //oxygen      
           0916.9f, //ice         
           0789.0f, //co          
           0850.0f, //nitrogen    
           0522.0f, //methane     
        // 0206.0f, //ethane      
        // 1130.0f  //tholins     Horst (2013)
        // 0187.0f, //helium      
        // 0086.0f, //hydrogen    
    };
    constexpr std::array<float, oxygen_planet_mass_pool_count> oxygen_planet_mass_pool_thermal_conductivities {
        // NOTE: values from Cermak (1988) unless stated otherwise
        //      , //magnetite   
        //        //chalcophile 
           11.3f, //hematite    
           19.2f, //pyrite      
        //        //goethite    
            3.5f, //olivine     
            4.3f, //pyroxene    
            2.0f, //plagioclase 
            7.7f, //quartz      
            2.3f, //orthoclase  
            3.6f, //calcite     
            0.3f, //organics    // representative of coal, Herrin (1996)
        //200.0f, //graphite    // wikipedia
        //  1.3f, //apatite     
        // 10.0f, //corundum    // order of magnitude, Manga (1997)
            1.0f, //co2         // order of magnitude, Sumarakov (2003)
            1.0f, //oxygen      // order of magnitude, Jezowski (1993)
          0.598f, //ice         // wikipedia
            3.0f, //co          // Stachowiak (1998)
            0.7f, //nitrogen    // wikipedia
            3.0f, //methane     // Jezowski (1997)
        //      , //ethane      
        //      , //tholins     
        //      , //helium      
        //      , //hydrogen    
    };
    /*
    Use of "oxygen_planet_mass_pool_chemical_susceptibility" is limited to determining 
    the particle size of chemically weathered sediment so only a crude approximation is needed
    */
    constexpr std::array<float, oxygen_planet_mass_pool_count> oxygen_planet_mass_pool_chemical_susceptibility {
        // 0.0f, //magnetite   
        // 0     //chalcophile 
           0.0f, //hematite    
           0.0f, //pyrite      
        // 0.0f  //goethite    
           0.0f, //olivine     
           0.0f, //pyroxene    
           0.0f, //plagioclase 
           0.0f, //quartz      
           0.0f, //orthoclase  
           1.0f, //calcite     
           1.0f, //organics    // density of coal
        // 0.0f, //graphite    
        // 0.0f, //apatite     
        // 0.0f, //corundum    
           0.0f, //co2         
           0.0f, //oxygen      
           0.9f, //ice         
           0.0f, //co          
           0.0f, //nitrogen    
           0.0f, //methane     
        // 0.0f, //ethane      
        // 1.0f  //tholins     Horst (2013)
        // 0.0f, //helium      
        // 0.0f, //hydrogen    
    };
    /*
    "Stratum" describes the composition and texuture of a single rock layer
    */
    template<int M>
    struct Stratum
    {
        /*
        "particle_size_bin_relative_volume" describes a set of bins.
        Each bin tracks the relative volume occupied by particles that fall within a certain range of diameters.
        Each bin corresponds to 3 gradations of the the Krumbein phi classification to a good approximation, 
        and describes anything from 1m boulders to colloids.
        Particle size is distinct from a mass pool's grain size.
        Grain size is primarily used to indicate distinctions between extrusive and intrusive rocks, like basalt and gabbro.
        Particle size is primarily used to indicate distinctions between things like boulders vs. pebbles vs sand vs. clay
        */
        std::array<StratumMassPool, M>  mass_pools;
        float max_temperature_received;
        float max_pressure_received;
        float age_of_world_when_deposited;

        Stratum():
            max_temperature_received(0),
            max_pressure_received(0),
            age_of_world_when_deposited(0)
        {
            mass_pools.fill(StratumMassPool());
        }

        Stratum(
            const float max_temperature_received,
            const float max_pressure_received,
            const float age_of_world_when_deposited
        ):
            max_temperature_received(max_temperature_received),
            max_pressure_received(max_pressure_received),
            age_of_world_when_deposited(age_of_world_when_deposited)
        {
            mass_pools.fill(StratumMassPool());
        }

        // DERIVED ATTRIBUTES, regular functions of the form: Stratum -> T
        float mass() const {
            float total_mass(0.0);
            for (std::size_t i=0; i<M; i++)
            {
                total_mass += mass_pools[i].mass;
            }
            return total_mass;
        }
        float volume(const std::array<float, M>& mass_pool_densities) const {
            float total_volume(0.0);
            for (std::size_t i=0; i<M; i++)
            {
                total_volume += mass_pools[i].mass / mass_pool_densities[i];
            }
            return total_volume;
        }
        float density(const std::array<float, M>& mass_pool_densities, float age) const {
            return mass() / volume(mass_pool_densities);
        }
        float thermal_conductivity(
            const std::array<float, M>& mass_pool_densities,
            const std::array<float, M>& mass_pool_thermal_conductivities
        ) const {
            // geometric mean weighted by fractional volume, see work by Fuchs (2013)
            float logK(0.0);
            float total_volume(volume(mass_pool_densities));
            float fractional_volume(0);
            for (std::size_t i=0; i<M; i++)
            {
                fractional_volume = mass_pools[i].mass / (mass_pool_densities[i] * total_volume);
                logK += fractional_volume * mass_pool_thermal_conductivities[i];
            }
            return exp(logK);
        }
    };

}
