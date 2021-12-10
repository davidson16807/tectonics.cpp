
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "CompletedCompound_to_string.hpp"
#include "CompletedCompound_test_utils.hpp"
#include "library.hpp"

TEST_CASE( "CompletedCompound to_string() purity", "[field]" ) {
	SECTION("Calling a function twice with the same arguments must produce the same results")
	{
		CHECK(compound::to_string(compound::fallback::water)
		   == compound::to_string(compound::fallback::water));
    }
}

TEST_CASE( "CompletedCompound to_string() appreciable difference preservation", "[field]" ) {
	compound::CompletedCompound nudge_molar_mass                         = compound::fallback::water; nudge_molar_mass                         .molar_mass                         = 1.01 * nudge_molar_mass                         .molar_mass                         ;
	compound::CompletedCompound nudge_atoms_per_molecule                 = compound::fallback::water; nudge_atoms_per_molecule                 .atoms_per_molecule                 = 2    * nudge_atoms_per_molecule                 .atoms_per_molecule                 ;
	compound::CompletedCompound nudge_molecular_diameter                 = compound::fallback::water; nudge_molecular_diameter                 .molecular_diameter                 = 1.01 * nudge_molecular_diameter                 .molecular_diameter                 ;
	compound::CompletedCompound nudge_molecular_degrees_of_freedom       = compound::fallback::water; nudge_molecular_degrees_of_freedom       .molecular_degrees_of_freedom       = 1.01 * nudge_molecular_degrees_of_freedom       .molecular_degrees_of_freedom       ;
	compound::CompletedCompound nudge_acentric_factor                    = compound::fallback::water; nudge_acentric_factor                    .acentric_factor                    = 1.01 * nudge_acentric_factor                    .acentric_factor                    ;
	compound::CompletedCompound nudge_critical_point_pressure            = compound::fallback::water; nudge_critical_point_pressure            .critical_point_pressure            = 1.01 * nudge_critical_point_pressure            .critical_point_pressure            ;
	compound::CompletedCompound nudge_critical_point_volume              = compound::fallback::water; nudge_critical_point_volume              .critical_point_volume              = 1.01 * nudge_critical_point_volume              .critical_point_volume              ;
	compound::CompletedCompound nudge_critical_point_temperature         = compound::fallback::water; nudge_critical_point_temperature         .critical_point_temperature         = 1.01 * nudge_critical_point_temperature         .critical_point_temperature         ;
	compound::CompletedCompound nudge_critical_point_compressibility     = compound::fallback::water; nudge_critical_point_compressibility     .critical_point_compressibility     = 1.01 * nudge_critical_point_compressibility     .critical_point_compressibility     ;
	compound::CompletedCompound nudge_latent_heat_of_vaporization        = compound::fallback::water; nudge_latent_heat_of_vaporization        .latent_heat_of_vaporization        = 1.01 * nudge_latent_heat_of_vaporization        .latent_heat_of_vaporization        ;
	compound::CompletedCompound nudge_latent_heat_of_fusion              = compound::fallback::water; nudge_latent_heat_of_fusion              .latent_heat_of_fusion              = 1.01 * nudge_latent_heat_of_fusion              .latent_heat_of_fusion              ;
	compound::CompletedCompound nudge_triple_point_pressure              = compound::fallback::water; nudge_triple_point_pressure              .triple_point_pressure              = 1.01 * nudge_triple_point_pressure              .triple_point_pressure              ;
	compound::CompletedCompound nudge_triple_point_temperature           = compound::fallback::water; nudge_triple_point_temperature           .triple_point_temperature           = 1.01 * nudge_triple_point_temperature           .triple_point_temperature           ;
	compound::CompletedCompound nudge_freezing_point_sample_pressure     = compound::fallback::water; nudge_freezing_point_sample_pressure     .freezing_point_sample_pressure     = 1.01 * nudge_freezing_point_sample_pressure     .freezing_point_sample_pressure     ;
	compound::CompletedCompound nudge_freezing_point_sample_temperature  = compound::fallback::water; nudge_freezing_point_sample_temperature  .freezing_point_sample_temperature  = 1.01 * nudge_freezing_point_sample_temperature  .freezing_point_sample_temperature  ;
	compound::CompletedCompound nudge_boiling_point_sample_pressure      = compound::fallback::water; nudge_boiling_point_sample_pressure      .boiling_point_sample_pressure      = 1.01 * nudge_boiling_point_sample_pressure      .boiling_point_sample_pressure      ;
	compound::CompletedCompound nudge_boiling_point_sample_temperature   = compound::fallback::water; nudge_boiling_point_sample_temperature   .boiling_point_sample_temperature   = 1.01 * nudge_boiling_point_sample_temperature   .boiling_point_sample_temperature   ;
	compound::CompletedCompound nudge_phase                              = compound::fallback::water; nudge_phase                              .phase                              = 1    + nudge_phase                              .phase                              (compound::field::StateParameters());
	compound::CompletedCompound nudge_molecular_absorption_cross_section = compound::fallback::water; nudge_molecular_absorption_cross_section .molecular_absorption_cross_section = 1e-20 * si::meter2;
	compound::CompletedCompound nudge_solid_first                        = compound::fallback::water; nudge_solid_first.solids[0]              .density                            = 1.01 * nudge_solid_first.solids[0]              .density                            (compound::field::StateParameters());
	compound::CompletedCompound nudge_solid_last                         = compound::fallback::water; nudge_solid_last.solids[nudge_solid_last.solids.size()-1].density            = 1.01 * nudge_solid_last.solids[nudge_solid_last.solids.size()-1].density            (compound::field::StateParameters());
	compound::CompletedCompound nudge_liquid                             = compound::fallback::water; nudge_liquid .liquid                     .density                            = 1.01 * nudge_liquid .liquid                     .density                            (compound::field::StateParameters());
	compound::CompletedCompound nudge_gas                                = compound::fallback::water; nudge_gas    .gas                        .density                            = 1.01 * nudge_gas    .gas                        .density                            (compound::field::StateParameters());

	SECTION("Appreciable differences in input are preserved within output")
	{
		CHECK( compound::to_string(nudge_molar_mass                        ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_atoms_per_molecule                ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_molecular_diameter                ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_molecular_degrees_of_freedom      ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_acentric_factor                   ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_critical_point_pressure           ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_critical_point_volume             ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_critical_point_temperature        ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_critical_point_compressibility    ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_latent_heat_of_vaporization       ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_latent_heat_of_fusion             ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_triple_point_pressure             ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_triple_point_temperature          ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_freezing_point_sample_pressure    ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_freezing_point_sample_temperature ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_boiling_point_sample_pressure     ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_boiling_point_sample_temperature  ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_phase                             ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_molecular_absorption_cross_section) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_solid_first                       ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_solid_last                        ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_liquid                            ) != compound::to_string(compound::fallback::water) ); 
		CHECK( compound::to_string(nudge_gas                               ) != compound::to_string(compound::fallback::water) ); 
    }
}
