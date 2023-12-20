
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <rasters/entities/SpheroidGrid/SpheroidGrid.hpp>
#include <rasters/entities/SpheroidGrid/SpheroidGrid_test_utils.hpp>
#include <rasters/entities/LayeredSpheroidGrid/LayeredSpheroidGrid.hpp>
#include <rasters/entities/LayeredSpheroidGrid/LayeredSpheroidGrid_test_utils.hpp>

#include "Crust_to_CrustValues.hpp"
#include "Crust_to_DensityTower.hpp"
#include "DensityTower_to_LiquidMixtures.hpp"
#include "DensityTower_to_StrataBoundaries.hpp"
#include "CrustValues_StrataBoundaries_to_LayeredRaster.hpp"

#include "CrustValues_test_utils.cpp"

using namespace crust;

TEST_CASE( "get_LayeredRaster_from_CrustValues/get_CrustValues_from_LayeredRaster invertibility", "[crust]" ) {
  	std::mt19937 generator(2);
  	const std::size_t L = 16;
  	const std::size_t M = 15;

	auto crust = get_random_crust<L,M>(icosahedron_grid, generator);

	auto stratum_pressures1 = make_CrustValues<float,L>(icosahedron_grid);
	get_max_pressures_received(crust, stratum_pressures1);

	auto stratum_temperatures = make_CrustValues<float,L>(icosahedron_grid);
	get_max_temperatures_received(crust, stratum_temperatures);

	constexpr std::array<float, M> mineral_densities {
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
		   1524.0f, //oxygen      
		   1562.0f, //co2         
		// 1616.0f, //argon
		   0920.0f, //ice         
		   0789.0f, //co          
		   0850.0f, //nitrogen    
		   0522.0f, //methane     
		// 0545.0f, //ethane      
		// 1130.0f  //tholins     Horst (2013)
		// 0187.0f, //helium      
		// 0086.0f, //hydrogen    
	};
	// values are mostly from wolfram alpha
	constexpr std::array<float, M> liquid_densities {
		// 5000.0f, //magnetite   
		//          //chalcophile 
		   5300.0f, //hematite    
		   5010.0f, //pyrite      
		// 3800.0f  //goethite    
		   2800.0f, //olivine     
		   2800.0f, //pyroxene    
		   2180.0f, //plagioclase 
		   2180.0f, //quartz      
		   2180.0f, //orthoclase  // from Murase and McBirney (1973), for rhyolitic magma
		   2180.0f, //calcite     
		   1370.0f, //organics    // Haaland (1976)
		// 1370.0f, //graphite    
		// 3180.0f, //apatite     
		// 4020.0f, //corundum    
		   1141.0f, //oxygen      
		   1101.0f, //co2         
		// 1401.0f, //argon
		   1022.0f, //ice         // NOTE: we use the density of seawater, because do not model the effect of solute concentration on density
		   0755.4f, //co          
		   0690.8f, //nitrogen    
		   0359.6f, //methane     
		// 0545.0f, //ethane      
		// 1130.0f  //tholins     Horst (2013)
		// 0125.0f, //helium      
		// 0072.0f, //hydrogen    
    };

	constexpr std::array<float, M> liquid_mass_pools {
		// 1e4f, //magnetite   
		//          //chalcophile 
		   1e4f, //hematite    
		   1e4f, //pyrite      
		// 3800.0f  //goethite    
		   1e4f, //olivine     
		   1e4f, //pyroxene    
		   1e4f, //plagioclase 
		   1e4f, //quartz      
		   1e4f, //orthoclase  // from Murase and McBirney (1973), for rhyolitic magma
		   1e4f, //calcite     
		   1e4f, //organics    // Haaland (1976)
		// 1e4f, //graphite    
		// 1e4f, //apatite     
		// 1e4f, //corundum    
		   1e4f, //oxygen      
		   1e4f, //co2         
		// 1e4f, //argon
		   1e4f, //ice         // NOTE: we use the density of seawater, because do not model the effect of solute concentration on density
		   1e4f, //co          
		   1e4f, //nitrogen    
		   1e4f, //methane     
		// 1e4f, //ethane      
		// 1130.0f  //tholins     Horst (2013)
		// 1e4f, //helium      
		// 1e4f, //hydrogen    
    };

	auto tower = make_DensityTower<L>(icosahedron_grid);
	get_DensityTower(
        crust, 
        mineral_densities,
        liquid_densities,
        liquid_mass_pools,
        units::earth_radius,
        units::earth_age,
        tower
    );

    auto mixtures = get_LiquidMixtures<L,M>(tower);

    auto boundaries = make_StrataBoundaries<L>(icosahedron_grid);
    get_StrataBoundaries(
        tower, 
        mixtures,
        liquid_mass_pools,
        liquid_densities,
        units::earth_radius,
        boundaries,
        rasters::make_Raster<float>(icosahedron_grid),
        rasters::make_Raster<float>(icosahedron_grid)
    );

    auto pressure_raster = rasters::make_LayeredRaster<float>(layered_icosahedron_grid);
    get_LayeredRaster_from_CrustValues(stratum_pressures1, boundaries, units::earth_radius, pressure_raster);

	auto stratum_pressures2 = make_CrustValues<float,L>(icosahedron_grid);
	// get_max_pressures_received(crust, stratum_pressures2);

    get_CrustValues_from_LayeredRaster(pressure_raster, boundaries, units::earth_radius, stratum_pressures2);

	SECTION("Converting crust values to and from LayeredRaster must produce the original object to within acceptable tolerances"){
	    CRUST_VALUES_LAYER_INTERPOLATION_EQUAL(stratum_pressures1, stratum_pressures2)
	}

	// SECTION("updating a Strata object attribute to its original value must produce the original object to within acceptable tolerances"){
	// 	CRUST_EQUAL(original, modified);
	// }

	// SECTION("updating a Strata object attribute to its original value must produce the original object to within acceptable tolerances"){
	// 	CRUST_EQUAL(original, modified);
	// }
}