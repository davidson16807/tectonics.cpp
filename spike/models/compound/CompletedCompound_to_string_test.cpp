
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "CompletedCompound_to_string.hpp"
#include "CompletedCompound_test_utils.hpp"
#include "CompletedCompound_library.hpp"

TEST_CASE( "CompletedCompound to_string() purity", "[field]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
		CHECK(compound::to_string(compound::water)
		   == compound::to_string(compound::water));
    }
}

TEST_CASE( "CompletedCompound to_string() appreciable difference preservation", "[field]" ) {
	compound::CompletedCompound nudge_molar_mass                         = compound::water; nudge_molar_mass                         .molar_mass                         = 1.01 * nudge_molar_mass                         .molar_mass                         ;
	compound::CompletedCompound nudge_atoms_per_molecule                 = compound::water; nudge_atoms_per_molecule                 .atoms_per_molecule                 = 2    * nudge_atoms_per_molecule                 .atoms_per_molecule                 ;
	compound::CompletedCompound nudge_molecular_diameter                 = compound::water; nudge_molecular_diameter                 .molecular_diameter                 = 1.01 * nudge_molecular_diameter                 .molecular_diameter                 ;
	compound::CompletedCompound nudge_molecular_degrees_of_freedom       = compound::water; nudge_molecular_degrees_of_freedom       .molecular_degrees_of_freedom       = 1.01 * nudge_molecular_degrees_of_freedom       .molecular_degrees_of_freedom       ;
	compound::CompletedCompound nudge_acentric_factor                    = compound::water; nudge_acentric_factor                    .acentric_factor                    = 1.01 * nudge_acentric_factor                    .acentric_factor                    ;
	compound::CompletedCompound nudge_critical_point_pressure            = compound::water; nudge_critical_point_pressure            .critical_point_pressure            = 1.01 * nudge_critical_point_pressure            .critical_point_pressure            ;
	compound::CompletedCompound nudge_critical_point_volume              = compound::water; nudge_critical_point_volume              .critical_point_volume              = 1.01 * nudge_critical_point_volume              .critical_point_volume              ;
	compound::CompletedCompound nudge_critical_point_temperature         = compound::water; nudge_critical_point_temperature         .critical_point_temperature         = 1.01 * nudge_critical_point_temperature         .critical_point_temperature         ;
	compound::CompletedCompound nudge_critical_point_compressibility     = compound::water; nudge_critical_point_compressibility     .critical_point_compressibility     = 1.01 * nudge_critical_point_compressibility     .critical_point_compressibility     ;
	compound::CompletedCompound nudge_latent_heat_of_vaporization        = compound::water; nudge_latent_heat_of_vaporization        .latent_heat_of_vaporization        = 1.01 * nudge_latent_heat_of_vaporization        .latent_heat_of_vaporization        ;
	compound::CompletedCompound nudge_latent_heat_of_fusion              = compound::water; nudge_latent_heat_of_fusion              .latent_heat_of_fusion              = 1.01 * nudge_latent_heat_of_fusion              .latent_heat_of_fusion              ;
	compound::CompletedCompound nudge_triple_point_pressure              = compound::water; nudge_triple_point_pressure              .triple_point_pressure              = 1.01 * nudge_triple_point_pressure              .triple_point_pressure              ;
	compound::CompletedCompound nudge_triple_point_temperature           = compound::water; nudge_triple_point_temperature           .triple_point_temperature           = 1.01 * nudge_triple_point_temperature           .triple_point_temperature           ;
	compound::CompletedCompound nudge_freezing_point_sample_pressure     = compound::water; nudge_freezing_point_sample_pressure     .freezing_point_sample_pressure     = 1.01 * nudge_freezing_point_sample_pressure     .freezing_point_sample_pressure     ;
	compound::CompletedCompound nudge_freezing_point_sample_temperature  = compound::water; nudge_freezing_point_sample_temperature  .freezing_point_sample_temperature  = 1.01 * nudge_freezing_point_sample_temperature  .freezing_point_sample_temperature  ;
	compound::CompletedCompound nudge_boiling_point_sample_pressure      = compound::water; nudge_boiling_point_sample_pressure      .boiling_point_sample_pressure      = 1.01 * nudge_boiling_point_sample_pressure      .boiling_point_sample_pressure      ;
	compound::CompletedCompound nudge_boiling_point_sample_temperature   = compound::water; nudge_boiling_point_sample_temperature   .boiling_point_sample_temperature   = 1.01 * nudge_boiling_point_sample_temperature   .boiling_point_sample_temperature   ;
	compound::CompletedCompound nudge_simon_glatzel_slope                = compound::water; nudge_simon_glatzel_slope                .simon_glatzel_slope                = 1.01 * nudge_simon_glatzel_slope                .simon_glatzel_slope                .value();
	compound::CompletedCompound nudge_simon_glatzel_exponent             = compound::water; nudge_simon_glatzel_exponent             .simon_glatzel_exponent             = 1.01 * nudge_simon_glatzel_exponent             .simon_glatzel_exponent             .value();
	compound::CompletedCompound nudge_molecular_absorption_cross_section = compound::water; nudge_molecular_absorption_cross_section .molecular_absorption_cross_section = 1e-20 * si::meter2;
	compound::CompletedCompound nudge_solid_first                        = compound::water; nudge_solid_first.solids[0]              .density                            = 1.01 * nudge_solid_first.solids[0]              .density                            (compound::field::StateParameters());
	compound::CompletedCompound nudge_solid_last                         = compound::water; nudge_solid_last.solids[nudge_solid_last.solids.size()-1].density            = 1.01 * nudge_solid_last.solids[nudge_solid_last.solids.size()-1].density            (compound::field::StateParameters());
	compound::CompletedCompound nudge_liquid                             = compound::water; nudge_liquid .liquid                     .density                            = 1.01 * nudge_liquid .liquid                     .density                            (compound::field::StateParameters());
	compound::CompletedCompound nudge_gas                                = compound::water; nudge_gas    .gas                        .density                            = 1.01 * nudge_gas    .gas                        .density                            (compound::field::StateParameters());

	SECTION("Appreciable differences in input are preserved within output")
	{
		CHECK( compound::to_string(nudge_molar_mass                        ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_atoms_per_molecule                ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_molecular_diameter                ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_molecular_degrees_of_freedom      ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_acentric_factor                   ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_critical_point_pressure           ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_critical_point_volume             ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_critical_point_temperature        ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_critical_point_compressibility    ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_latent_heat_of_vaporization       ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_latent_heat_of_fusion             ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_triple_point_pressure             ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_triple_point_temperature          ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_freezing_point_sample_pressure    ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_freezing_point_sample_temperature ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_boiling_point_sample_pressure     ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_boiling_point_sample_temperature  ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_simon_glatzel_slope               ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_simon_glatzel_exponent            ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_molecular_absorption_cross_section) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_solid_first                       ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_solid_last                        ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_liquid                            ) != compound::to_string(compound::water) ); 
		CHECK( compound::to_string(nudge_gas                               ) != compound::to_string(compound::water) ); 
    }
}
