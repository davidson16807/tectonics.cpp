
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "PartlyKnownCompound_to_string.hpp"
#include "PartlyKnownCompound_test_utils.hpp"
#include "PartlyKnownCompound_library.hpp"

TEST_CASE( "PartlyKnownCompound to_string() purity", "[field]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
		CHECK(compound::to_string(compound::unknown_hydrogen)
		   == compound::to_string(compound::unknown_hydrogen));
		CHECK(compound::to_string(compound::test_water)
		   == compound::to_string(compound::test_water));
		CHECK(compound::to_string(compound::nitrogen)
		   == compound::to_string(compound::nitrogen));
		CHECK(compound::to_string(compound::quartz)
		   == compound::to_string(compound::quartz));
		CHECK(compound::to_string(compound::gold)
		   == compound::to_string(compound::gold));
    }
}

TEST_CASE( "PartlyKnownCompound to_string() appreciable difference preservation", "[field]" ) {
	compound::PartlyKnownCompound nudge_molar_mass                         = compound::test_water; nudge_molar_mass                         .molar_mass                         = 1.01 * nudge_molar_mass                         .molar_mass                    ;
	compound::PartlyKnownCompound nudge_atoms_per_molecule                 = compound::test_water; nudge_atoms_per_molecule                 .atoms_per_molecule                 = 2    * nudge_atoms_per_molecule                 .atoms_per_molecule            ;
	compound::PartlyKnownCompound nudge_molecular_diameter                 = compound::test_water; nudge_molecular_diameter                 .molecular_diameter                 = 1.01 * nudge_molecular_diameter                 .molecular_diameter            ;
	compound::PartlyKnownCompound nudge_molecular_degrees_of_freedom       = compound::test_water; nudge_molecular_degrees_of_freedom       .molecular_degrees_of_freedom       = std::monostate();
	compound::PartlyKnownCompound nudge_acentric_factor                    = compound::test_water; nudge_acentric_factor                    .acentric_factor                    = std::monostate();
	compound::PartlyKnownCompound nudge_critical_point_pressure            = compound::test_water; nudge_critical_point_pressure            .critical_point_pressure            = 1.01 * nudge_critical_point_pressure            .critical_point_pressure       ;
	compound::PartlyKnownCompound nudge_critical_point_volume              = compound::test_water; nudge_critical_point_volume              .critical_point_volume              = 1.01 * nudge_critical_point_volume              .critical_point_volume         ;
	compound::PartlyKnownCompound nudge_critical_point_temperature         = compound::test_water; nudge_critical_point_temperature         .critical_point_temperature         = 1.01 * nudge_critical_point_temperature         .critical_point_temperature    ;
	compound::PartlyKnownCompound nudge_critical_point_compressibility     = compound::test_water; nudge_critical_point_compressibility     .critical_point_compressibility     = 1.01 * nudge_critical_point_compressibility     .critical_point_compressibility;
	compound::PartlyKnownCompound nudge_latent_heat_of_vaporization        = compound::test_water; nudge_latent_heat_of_vaporization        .latent_heat_of_vaporization        = std::monostate();
	compound::PartlyKnownCompound nudge_latent_heat_of_fusion              = compound::test_water; nudge_latent_heat_of_fusion              .latent_heat_of_fusion              = std::monostate();
	compound::PartlyKnownCompound nudge_triple_point_pressure              = compound::test_water; nudge_triple_point_pressure              .triple_point_pressure              = std::monostate();
	compound::PartlyKnownCompound nudge_triple_point_temperature           = compound::test_water; nudge_triple_point_temperature           .triple_point_temperature           = std::monostate();
	compound::PartlyKnownCompound nudge_freezing_point_sample_pressure     = compound::test_water; nudge_freezing_point_sample_pressure     .freezing_point_sample_pressure     = std::monostate();
	compound::PartlyKnownCompound nudge_freezing_point_sample_temperature  = compound::test_water; nudge_freezing_point_sample_temperature  .freezing_point_sample_temperature  = std::monostate();
	compound::PartlyKnownCompound nudge_boiling_point_sample_pressure      = compound::test_water; nudge_boiling_point_sample_pressure      .boiling_point_sample_pressure      = std::monostate();
	compound::PartlyKnownCompound nudge_boiling_point_sample_temperature   = compound::test_water; nudge_boiling_point_sample_temperature   .boiling_point_sample_temperature   = std::monostate();
	compound::PartlyKnownCompound nudge_simon_glatzel_slope                = compound::test_water; nudge_simon_glatzel_slope                .simon_glatzel_slope                = std::monostate();
	compound::PartlyKnownCompound nudge_simon_glatzel_exponent             = compound::test_water; nudge_simon_glatzel_exponent             .simon_glatzel_exponent             = std::monostate();
	compound::PartlyKnownCompound nudge_molecular_absorption_cross_section = compound::test_water; nudge_molecular_absorption_cross_section .molecular_absorption_cross_section = std::monostate();
	compound::PartlyKnownCompound nudge_solid_first                        = compound::test_water; nudge_solid_first.solids[0]              .density                            = std::monostate();
	compound::PartlyKnownCompound nudge_solid_last                         = compound::test_water; nudge_solid_last.solids[nudge_solid_last.solids.size()-1].density            = std::monostate();
	compound::PartlyKnownCompound nudge_liquid                             = compound::test_water; nudge_liquid .liquid                     .density                            = std::monostate();
	compound::PartlyKnownCompound nudge_gas                                = compound::test_water; nudge_gas    .gas                        .density                            = std::monostate();

	SECTION("Appreciable differences in input are preserved within output")
	{
		CHECK( compound::to_string(nudge_molar_mass                        ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_atoms_per_molecule                ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_molecular_diameter                ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_molecular_degrees_of_freedom      ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_acentric_factor                   ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_critical_point_pressure           ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_critical_point_volume             ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_critical_point_temperature        ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_critical_point_compressibility    ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_latent_heat_of_vaporization       ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_latent_heat_of_fusion             ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_triple_point_pressure             ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_triple_point_temperature          ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_freezing_point_sample_pressure    ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_freezing_point_sample_temperature ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_boiling_point_sample_pressure     ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_boiling_point_sample_temperature  ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_simon_glatzel_slope               ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_simon_glatzel_exponent            ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_molecular_absorption_cross_section) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_solid_first                       ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_solid_last                        ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_liquid                            ) != compound::to_string(compound::test_water) ); 
		CHECK( compound::to_string(nudge_gas                               ) != compound::to_string(compound::test_water) ); 
    }
}
