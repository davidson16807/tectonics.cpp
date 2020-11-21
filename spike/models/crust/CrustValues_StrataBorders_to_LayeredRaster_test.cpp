
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <rasters/entities/Grid/Grid.hpp>
#include <rasters/entities/Grid/Grid_test_utils.hpp>
#include <rasters/entities/LayeredGrid/LayeredGrid.hpp>
#include <rasters/entities/LayeredGrid/LayeredGrid_test_utils.hpp>

// #include "Crust_to_CrustValues.hpp"
// #include "Crust_to_DensityTower.hpp"
// #include "DensityTower_to_LiquidMixtures.hpp"
// #include "DensityTower_to_StrataBorders.hpp"
#include "CrustValues_StrataBorders_to_LayeredRaster.hpp"

#include "CrustValues_test_utils.cpp"

using namespace crust;

// auto crust = make_Crust<L,M>(grid, {
// 	Strata<L,M>({ Stratum<M>({ Mineral(1.0f) }) }),
// 	Strata<L,M>({ Stratum<M>({ Mineral(1.0f) }) }),
// 	Strata<L,M>({ Stratum<M>({ Mineral(1.0f) }) })
// });

TEST_CASE( "get_LayeredRaster_from_CrustValues regularity", "[crust]" ) {
  	const std::size_t L = 2;

    rasters::Grid grid(
		series::vec3s({
            glm::vec3(0,0,0),
            glm::vec3(1,0,0),
            glm::vec3(2,0,0),
            glm::vec3(3,0,0),
        }), 
        series::uvec3s({})
    );

	auto original = make_CrustValues<float,L>(grid, {
		strata::StrataValues<float,L>({ 1.0f, 2.0f }),
		strata::StrataValues<float,L>({ 3.0f, 4.0f }),
		strata::StrataValues<float,L>({ 5.0f, 6.0f }),
		strata::StrataValues<float,L>({ 7.0f, 8.0f })
	});

	auto borders = make_StrataBorders<L>(grid, {
		strata::StrataValues<StratumBorders,L>({ StratumBorders(0.0f, 1.0f), StratumBorders(1.0f, 3.0f) }), // shared border
		strata::StrataValues<StratumBorders,L>({ StratumBorders(0.0f, 1.0f), StratumBorders(2.0f, 3.0f) }), // open space between
		strata::StrataValues<StratumBorders,L>({ StratumBorders(0.0f, 1.5f), StratumBorders(1.5f, 3.0f) }), // layer split between two strata
		strata::StrataValues<StratumBorders,L>({ StratumBorders(0.0f, 1.0f), StratumBorders(1.0f, 2.0f) })  // open space above
	});

    rasters::LayeredGrid layered_grid(grid, 3.0f, 0.0f, 3u);
    auto raster1 = rasters::make_LayeredRaster<float>(layered_grid);
    auto raster2 = rasters::make_LayeredRaster<float>(layered_grid);

    get_LayeredRaster_from_CrustValues(original, borders, raster1);
    get_LayeredRaster_from_CrustValues(original, borders, raster2);

	SECTION("Converting crust values to and from LayeredRaster multiple times must produce the same result"){
	    CHECK(standard_deviation(raster1, raster2) < 0.001f);
	}
}

TEST_CASE( "get_CrustValues_from_LayeredRaster regularity", "[crust]" ) {
  	const std::size_t L = 2;

    rasters::Grid grid(
		series::vec3s({
            glm::vec3(0,0,0),
            glm::vec3(1,0,0),
            glm::vec3(2,0,0),
            glm::vec3(3,0,0),
        }), 
        series::uvec3s({})
    );

	auto original = make_CrustValues<float,L>(grid, {
		strata::StrataValues<float,L>({ 1.0f, 2.0f }),
		strata::StrataValues<float,L>({ 3.0f, 4.0f }),
		strata::StrataValues<float,L>({ 5.0f, 6.0f }),
		strata::StrataValues<float,L>({ 7.0f, 8.0f })
	});

	auto borders = make_StrataBorders<L>(grid, {
		strata::StrataValues<StratumBorders,L>({ StratumBorders(0.0f, 1.0f), StratumBorders(1.0f, 3.0f) }), // shared border
		strata::StrataValues<StratumBorders,L>({ StratumBorders(0.0f, 1.0f), StratumBorders(2.0f, 3.0f) }), // open space between
		strata::StrataValues<StratumBorders,L>({ StratumBorders(0.0f, 1.5f), StratumBorders(1.5f, 3.0f) }), // layer split between two strata
		strata::StrataValues<StratumBorders,L>({ StratumBorders(0.0f, 1.0f), StratumBorders(1.0f, 2.0f) })  // open space above
	});

    rasters::LayeredGrid layered_grid(grid, 3.0f, 0.0f, 3u);
    auto raster1 = rasters::make_LayeredRaster<float>(layered_grid);

    get_LayeredRaster_from_CrustValues(original, borders, raster1);

	auto reproduced1 = make_CrustValues<float,L>(grid);
	auto reproduced2 = make_CrustValues<float,L>(grid);

    get_CrustValues_from_LayeredRaster(raster1, borders, reproduced1);
    get_CrustValues_from_LayeredRaster(raster1, borders, reproduced2);

	SECTION("Converting crust values to and from LayeredRaster multiple times must produce the same result"){
	    CRUST_VALUES_EQUAL(reproduced1, reproduced2)
	}
}

TEST_CASE( "get_LayeredRaster_from_CrustValues/get_CrustValues_from_LayeredRaster invertibility", "[crust]" ) {
  	const std::size_t L = 2;

    rasters::Grid grid(
		series::vec3s({
            glm::vec3(0,0,0),
            glm::vec3(1,0,0),
            glm::vec3(2,0,0),
            glm::vec3(3,0,0),
        }), 
        series::uvec3s({})
    );

	auto original = make_CrustValues<float,L>(grid, {
		strata::StrataValues<float,L>({ 1.0f, 2.0f }),
		strata::StrataValues<float,L>({ 3.0f, 4.0f }),
		strata::StrataValues<float,L>({ 5.0f, 6.0f }),
		strata::StrataValues<float,L>({ 7.0f, 8.0f })
	});

	auto borders = make_StrataBorders<L>(grid, {
		strata::StrataValues<StratumBorders,L>({ StratumBorders(0.0f, 1.0f), StratumBorders(1.0f, 3.0f) }), // shared border
		strata::StrataValues<StratumBorders,L>({ StratumBorders(0.0f, 1.0f), StratumBorders(2.0f, 3.0f) }), // open space between
		strata::StrataValues<StratumBorders,L>({ StratumBorders(0.0f, 1.5f), StratumBorders(1.5f, 3.0f) }), // layer split between two strata
		strata::StrataValues<StratumBorders,L>({ StratumBorders(0.0f, 1.0f), StratumBorders(1.0f, 2.0f) })  // open space above
	});

    rasters::LayeredGrid layered_grid(grid, 3.0f, 0.0f, 3u);
    auto raster = rasters::make_LayeredRaster<float>(layered_grid);

    get_LayeredRaster_from_CrustValues(original, borders, raster);

	auto reproduced = make_CrustValues<float,L>(grid);

    get_CrustValues_from_LayeredRaster(raster, borders, reproduced);

	SECTION("Converting crust values to and from LayeredRaster must produce the original object under controlled conditions"){
	    CRUST_VALUES_EQUAL(original, reproduced)
	}
}

/*
TEST_CASE( "get_LayeredRaster_from_CrustValues/get_CrustValues_from_LayeredRaster integration test", "[crust]" ) {
  	const std::size_t L = 1;
  	const std::size_t M = 1;

    meshes::mesh mesh(series::vec3s({
	                glm::vec3(0,0,0),
	                glm::vec3(1,0,0),
	                glm::vec3(0,1,0),
	            }),
	        series::uvec3s({
	                glm::uvec3(0,1,2)
	            }));
    rasters::Grid grid(mesh.vertices, mesh.faces);

	auto crust = make_Crust<L,M>(grid);

	for (int i = 0; i < crust.size(); ++i)
	{

		crust[i]
	}

	auto crust_values = make_CrustValues<float,L>(grid);
	get_max_pressures_received(crust, crust_values);

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

	auto tower = make_DensityTower<L>(grid);
	get_DensityTower(
        crust, 
        mineral_densities,
        liquid_densities,
        liquid_mass_pools,
        units::earth_age,
        tower
    );

    auto mixtures = get_LiquidMixtures<L,M>(tower);

    auto mixture_densities = get_densities(mixtures, liquid_mass_pools, liquid_densities);

    auto borders = make_StrataBorders<L>(grid);
    get_StrataBorders(
        tower, 
        mixtures,
        mixture_densities,
        borders,
        rasters::make_Raster<float>(grid),
        rasters::make_Raster<float>(grid)
    );

    auto pressure_raster = rasters::make_LayeredRaster<float>(layered_grid);
    get_LayeredRaster_from_CrustValues(crust_values, borders, units::earth_radius, pressure_raster);

	auto stratum_pressures2 = make_CrustValues<float,L>(grid);
	// get_max_pressures_received(crust, stratum_pressures2);

    get_CrustValues_from_LayeredRaster(pressure_raster, borders, units::earth_radius, stratum_pressures2);

	// SECTION("Converting crust values to and from LayeredRaster must produce the original object to within acceptable tolerances"){
	    // CRUST_VALUES_LAYER_INTERPOLATION_EQUAL(crust_values, stratum_pressures2)
	// }

	// SECTION("updating a Strata object attribute to its original value must produce the original object to within acceptable tolerances"){
	// 	CRUST_EQUAL(original, modified);
	// }

	// SECTION("updating a Strata object attribute to its original value must produce the original object to within acceptable tolerances"){
	// 	CRUST_EQUAL(original, modified);
	// }
}
*/