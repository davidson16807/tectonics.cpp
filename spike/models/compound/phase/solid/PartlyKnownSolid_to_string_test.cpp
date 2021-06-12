
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "PartlyKnownSolid_to_string.hpp"
#include "PartlyKnownSolid_test_utils.hpp"

TEST_CASE( "OptionalConstantField to_string() purity", "[field]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
		CHECK(compound::phase::to_string(unknown_solid)
		   == compound::phase::to_string(unknown_solid));
		CHECK(compound::phase::to_string(ice)
		   == compound::phase::to_string(ice));
		CHECK(compound::phase::to_string(quartz)
		   == compound::phase::to_string(quartz));
		CHECK(compound::phase::to_string(copper)
		   == compound::phase::to_string(copper));
		CHECK(compound::phase::to_string(dummy_solid)
		   == compound::phase::to_string(dummy_solid));
    }
}

TEST_CASE( "OptionalConstantField to_string() appreciable difference preservation", "[field]" ) {
	compound::phase::PartlyKnownSolid nudge_specific_heat_capacity           = dummy_solid; nudge_specific_heat_capacity           .specific_heat_capacity           = std::monostate();
	compound::phase::PartlyKnownSolid nudge_thermal_conductivity             = dummy_solid; nudge_thermal_conductivity             .thermal_conductivity             = std::monostate();
	compound::phase::PartlyKnownSolid nudge_dynamic_viscosity                = dummy_solid; nudge_dynamic_viscosity                .dynamic_viscosity                = std::monostate();
	compound::phase::PartlyKnownSolid nudge_density                          = dummy_solid; nudge_density                          .density                          = std::monostate();
	compound::phase::PartlyKnownSolid nudge_vapor_pressure                   = dummy_solid; nudge_vapor_pressure                   .vapor_pressure                   = std::monostate();
	compound::phase::PartlyKnownSolid nudge_refractive_index                 = dummy_solid; nudge_refractive_index                 .refractive_index                 = std::monostate();
	compound::phase::PartlyKnownSolid nudge_spectral_reflectance             = dummy_solid; nudge_spectral_reflectance             .spectral_reflectance             = std::monostate();
	compound::phase::PartlyKnownSolid nudge_bulk_modulus                     = dummy_solid; nudge_bulk_modulus                     .bulk_modulus                     = std::monostate();
	compound::phase::PartlyKnownSolid nudge_tensile_modulus                  = dummy_solid; nudge_tensile_modulus                  .tensile_modulus                  = std::monostate();
	compound::phase::PartlyKnownSolid nudge_shear_modulus                    = dummy_solid; nudge_shear_modulus                    .shear_modulus                    = std::monostate();
	compound::phase::PartlyKnownSolid nudge_pwave_modulus                    = dummy_solid; nudge_pwave_modulus                    .pwave_modulus                    = std::monostate();
	compound::phase::PartlyKnownSolid nudge_lame_parameter                   = dummy_solid; nudge_lame_parameter                   .lame_parameter                   = std::monostate();
	compound::phase::PartlyKnownSolid nudge_poisson_ratio                    = dummy_solid; nudge_poisson_ratio                    .poisson_ratio                    = std::monostate();
	compound::phase::PartlyKnownSolid nudge_compressive_fracture_strength    = dummy_solid; nudge_compressive_fracture_strength    .compressive_fracture_strength    = std::monostate();
	compound::phase::PartlyKnownSolid nudge_tensile_fracture_strength        = dummy_solid; nudge_tensile_fracture_strength        .tensile_fracture_strength        = std::monostate();
	compound::phase::PartlyKnownSolid nudge_shear_fracture_strength          = dummy_solid; nudge_shear_fracture_strength          .shear_fracture_strength          = std::monostate();
	compound::phase::PartlyKnownSolid nudge_compressive_yield_strength       = dummy_solid; nudge_compressive_yield_strength       .compressive_yield_strength       = std::monostate();
	compound::phase::PartlyKnownSolid nudge_tensile_yield_strength           = dummy_solid; nudge_tensile_yield_strength           .tensile_yield_strength           = std::monostate();
	compound::phase::PartlyKnownSolid nudge_shear_yield_strength             = dummy_solid; nudge_shear_yield_strength             .shear_yield_strength             = std::monostate();
	// compound::phase::PartlyKnownSolid nudge_chemical_susceptibility_estimate = dummy_solid; nudge_chemical_susceptibility_estimate .chemical_susceptibility_estimate = std::monostate();
	
	SECTION("Appreciable differences in input are preserved within output")
	{
		CHECK(compound::phase::to_string(nudge_specific_heat_capacity          ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_thermal_conductivity            ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_dynamic_viscosity               ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_density                         ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_vapor_pressure                  ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_refractive_index                ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_spectral_reflectance            ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_bulk_modulus                    ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_tensile_modulus                 ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_shear_modulus                   ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_pwave_modulus                   ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_lame_parameter                  ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_poisson_ratio                   ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_compressive_fracture_strength   ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_tensile_fracture_strength       ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_shear_fracture_strength         ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_compressive_yield_strength      ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_tensile_yield_strength          ) != compound::phase::to_string(dummy_solid));
		CHECK(compound::phase::to_string(nudge_shear_yield_strength            ) != compound::phase::to_string(dummy_solid));
		// CHECK(compound::phase::to_string(nudge_chemical_susceptibility_estimate) != compound::phase::to_string(dummy_solid));
    }
}
