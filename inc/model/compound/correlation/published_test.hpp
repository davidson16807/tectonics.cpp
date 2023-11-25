
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/test_tools.cpp"

#include "published.hpp"

TEST_CASE( "Klincewicz method purity", "[correlation]" ) {
    si::UnitAdapter<double> adapter(1e-4);

	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	si::temperature<double> Tb (329.20*si::kelvin);
	int A (10);
    SECTION("Calling a function twice with the same arguments must produce the same results"){
    	CHECK(adapter.equal( compound::correlation::estimate_critical_temperature_from_klincewicz(M, Tb), 
    		                      compound::correlation::estimate_critical_temperature_from_klincewicz(M, Tb)));
    	CHECK(adapter.equal( compound::correlation::estimate_critical_pressure_from_klincewicz(M, A), 
    		                      compound::correlation::estimate_critical_pressure_from_klincewicz(M, A)));
    	CHECK(adapter.equal( compound::correlation::estimate_critical_molar_volume_from_klincewicz(M, A), 
    		                      compound::correlation::estimate_critical_molar_volume_from_klincewicz(M, A)));
    }
}
TEST_CASE( "Klincewicz method accuracy", "[correlation]" ) {
    si::UnitAdapter<double> adapter(0.3);

	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	si::temperature<double> Tb (329.20*si::kelvin);
	int A (10);
	si::pressure<double> pc (47.0*si::bar);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::molar_volume<double> Vc (209.0 * si::centimeter3/si::mole);

    SECTION("Klincewicz method must predict properties of acetone to within 30%"){
    	CHECK(adapter.equal(compound::correlation::estimate_critical_temperature_from_klincewicz(M, Tb), Tc));
    	CHECK(adapter.equal(compound::correlation::estimate_critical_pressure_from_klincewicz(M, A), pc));
    	CHECK(adapter.equal(compound::correlation::estimate_critical_molar_volume_from_klincewicz(M, A), Vc));
    }
}
TEST_CASE( "Ihmels method purity", "[correlation]" ) {
    si::UnitAdapter<double> adapter(1e-4);

	// properties of acetone
	si::pressure<double> pc (47.0*si::bar);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::molar_volume<double> Vc (209.0 * si::centimeter3/si::mole);

    SECTION("Calling a function twice with the same arguments must produce the same results"){
    	CHECK(adapter.equal(compound::correlation::estimate_critical_temperature_from_ihmels(pc, Vc), compound::correlation::estimate_critical_temperature_from_ihmels(pc, Vc)));
    	CHECK(adapter.equal(compound::correlation::estimate_critical_pressure_from_ihmels(Tc, Vc), compound::correlation::estimate_critical_pressure_from_ihmels(Tc, Vc)));
    	CHECK(adapter.equal(compound::correlation::estimate_critical_molar_volume_from_ihmels(Tc, pc), compound::correlation::estimate_critical_molar_volume_from_ihmels(Tc, pc)));
    }
}
TEST_CASE( "Ihmels method accuracy", "[correlation]" ) {
    si::UnitAdapter<double> adapter(0.3);

	// properties of acetone
	si::pressure<double> pc (47.0*si::bar);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::molar_volume<double> Vc (209.0 * si::centimeter3/si::mole);

    SECTION("Ihmels method must predict properties of acetone to within 30%"){
    	CHECK(adapter.equal(compound::correlation::estimate_critical_temperature_from_ihmels(pc, Vc), Tc));
    	CHECK(adapter.equal(compound::correlation::estimate_critical_pressure_from_ihmels(Tc, Vc), pc));
    	CHECK(adapter.equal(compound::correlation::estimate_critical_molar_volume_from_ihmels(Tc, pc), Vc));
    }
}

TEST_CASE( "Klincewicz-Ihmels consistency", "[correlation]" ) {
    si::UnitAdapter<double> adapter(0.3);

	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	si::temperature<double> Tb (329.20*si::kelvin);
	int A (10);
	si::pressure<double> pc (47.0*si::bar);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::molar_volume<double> Vc (209.0 * si::centimeter3/si::mole);

    SECTION("Klincewicz method and Ihmels method must agree on predictions to within 30%"){
    	CHECK(adapter.equal(compound::correlation::estimate_critical_temperature_from_ihmels(pc, Vc), compound::correlation::estimate_critical_temperature_from_klincewicz(M, Tb)));
    	CHECK(adapter.equal(compound::correlation::estimate_critical_pressure_from_ihmels(Tc, Vc), compound::correlation::estimate_critical_pressure_from_klincewicz(M,A)));
    	CHECK(adapter.equal(compound::correlation::estimate_critical_molar_volume_from_ihmels(Tc,pc), compound::correlation::estimate_critical_molar_volume_from_klincewicz(M,A)));
    }
}

TEST_CASE( "Klincewicz/Ihmels accuracy", "[correlation]" ) {
    si::UnitAdapter<double> adapter(0.3);

	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	int A (10);
	si::temperature<double> Tc (508.1*si::kelvin);
	double Zc (0.233f);
    SECTION("Combining Klincewicz and Ihmels must predict properties of acetone to within 30%"){
		si::pressure<double> pc2 (compound::correlation::estimate_critical_pressure_from_klincewicz(M, A));
		si::molar_volume<double> Vc2 (compound::correlation::estimate_critical_molar_volume_from_klincewicz(M, A));
		si::temperature<double> Tc2 (compound::correlation::estimate_critical_temperature_from_ihmels(pc2, Vc2));
		double Zc2 (compound::correlation::get_critical_compressibility(pc2, Tc2, Vc2));
    	CHECK(adapter.equal(Tc2, Tc));
    	CHECK(adapter.equal(Zc2, Zc));
    }
}

TEST_CASE( "Sheffy-Johnson method purity", "[correlation]" ) {
	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	si::temperature<double> Tm (178.5 * si::kelvin);
	si::temperature<double> T (si::standard_temperature);

    SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(M,T,Tm) == compound::correlation::estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(M,T,Tm));
    }
}

TEST_CASE( "Sheffy-Johnson method accuracy", "[correlation]" ) {
    si::UnitAdapter<double> adapter(0.3);

	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	si::temperature<double> Tm (178.5 * si::kelvin);
	si::temperature<double> T (si::standard_temperature);
	si::thermal_conductivity<double> kL (0.161 * si::watt/(si::meter*si::kelvin));

    SECTION("Sheffy-Johnson method must predict properties of acetone to within 30%"){
    	CHECK(adapter.equal(compound::correlation::estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(M,T,Tm), kL));
    }
}

TEST_CASE( "Sato-Riedel method purity", "[correlation]" ) {
	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::temperature<double> Tb (329.20*si::kelvin);
	si::temperature<double> T (si::standard_temperature);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_thermal_conductivity_as_liquid_from_sato_riedel(M,T,Tb,Tc) == compound::correlation::estimate_thermal_conductivity_as_liquid_from_sato_riedel(M,T,Tb,Tc));
    }
}

TEST_CASE( "Sato-Riedel method accuracy", "[correlation]" ) {
    si::UnitAdapter<double> adapter(0.3);

	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::temperature<double> Tb (329.20*si::kelvin);
	si::temperature<double> T (si::standard_temperature);
	si::thermal_conductivity<double> kL (0.161 * si::watt/(si::meter*si::kelvin));

    SECTION("Sato-Riedel method must predict properties of acetone to within 30%"){
    	CHECK(adapter.equal(compound::correlation::estimate_thermal_conductivity_as_liquid_from_sato_riedel(M,T,Tb,Tc), kL));
    }
}

TEST_CASE( "Sheffy-Johnson/Sato-Riedel consistency", "[correlation]" ) {
    si::UnitAdapter<double> adapter(0.3);

	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::temperature<double> Tb (329.20*si::kelvin);
	si::temperature<double> Tm (178.5* si::kelvin);
	si::temperature<double> T (si::standard_temperature);

    SECTION("Sheffy-Johnson method and Sato-Riedel method must agree on predictions to within 30%"){
    	CHECK(adapter.equal(compound::correlation::estimate_thermal_conductivity_as_liquid_from_sato_riedel(M,T,Tb,Tc), compound::correlation::estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(M,T,Tm)));
    }
}


TEST_CASE( "Rowlinson-Poling method purity", "[correlation]" ) {
	// properties of acetone
	si::temperature<double> Tc (508.1*si::kelvin);
	si::temperature<double> T (si::standard_temperature);
	si::molar_heat_capacity<double> C_L (125.0 * si::joule/(si::mole*si::kelvin));
	si::molar_heat_capacity<double> C_G (75.0 * si::joule/(si::mole*si::kelvin));
	double omega(0.304);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_isobaric_heat_capacity_as_liquid_from_rowlinson_poling(Tc,T,omega,C_G) == compound::correlation::estimate_isobaric_heat_capacity_as_liquid_from_rowlinson_poling(Tc,T,omega,C_G));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_isobaric_heat_capacity_as_gas_from_rowlinson_poling(Tc,T,omega,C_L) == compound::correlation::estimate_isobaric_heat_capacity_as_gas_from_rowlinson_poling(Tc,T,omega,C_L));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_acentric_factor_from_rowlinson_poling(Tc,T,C_L,C_G) == compound::correlation::estimate_acentric_factor_from_rowlinson_poling(Tc,T,C_L,C_G));
    }
}

TEST_CASE( "Rowlinson-Poling method accuracy", "[correlation]" ) {
    si::UnitAdapter<double> adapter(0.3);

	// properties of acetone
	si::temperature<double> Tc (508.1*si::kelvin);
	si::temperature<double> T (si::standard_temperature);
	si::molar_heat_capacity<double> C_L (125.0 * si::joule/(si::mole*si::kelvin));
	si::molar_heat_capacity<double> C_G (75.0 * si::joule/(si::mole*si::kelvin));
	double omega(0.304);

	SECTION("Rowlinson-Poling method must agree on predictions to within 30%"){
    	CHECK(adapter.equal(C_L, compound::correlation::estimate_isobaric_heat_capacity_as_liquid_from_rowlinson_poling(Tc,T,omega,C_G)));
    }

	SECTION("Rowlinson-Poling method must agree on predictions to within 30%"){
    	CHECK(adapter.equal(C_G, compound::correlation::estimate_isobaric_heat_capacity_as_gas_from_rowlinson_poling(Tc,T,omega,C_L)));
    }

	SECTION("Rowlinson-Poling method must agree on predictions to within 30%"){
    	CHECK(adapter.equal(omega, compound::correlation::estimate_acentric_factor_from_rowlinson_poling(Tc,T,C_L,C_G)));
    }
}

TEST_CASE( "Rowlinson-Poling consistency", "[correlation]" ) {
    si::UnitAdapter<double> adapter(1e-4);

	// properties of acetone
	si::temperature<double> Tc (508.1*si::kelvin);
	si::temperature<double> T (si::standard_temperature);
	si::molar_heat_capacity<double> C_L (125.0 * si::joule/(si::mole*si::kelvin));
	si::molar_heat_capacity<double> C_G (75.0 * si::joule/(si::mole*si::kelvin));
	double omega(0.304);

    SECTION("Rowlinson-Poling method must be invertible"){
    	CHECK(adapter.equal(C_L, compound::correlation::estimate_isobaric_heat_capacity_as_liquid_from_rowlinson_poling(Tc,T,compound::correlation::estimate_acentric_factor_from_rowlinson_poling(Tc,T,C_L,C_G),C_G)));
    	CHECK(adapter.equal(C_G, compound::correlation::estimate_isobaric_heat_capacity_as_gas_from_rowlinson_poling(Tc,T,compound::correlation::estimate_acentric_factor_from_rowlinson_poling(Tc,T,C_L,C_G),C_L)));
    	CHECK(adapter.equal(omega, compound::correlation::estimate_acentric_factor_from_rowlinson_poling(Tc,T,compound::correlation::estimate_isobaric_heat_capacity_as_liquid_from_rowlinson_poling(Tc,T,omega,C_G),C_G)));
    	CHECK(adapter.equal(omega, compound::correlation::estimate_acentric_factor_from_rowlinson_poling(Tc,T,C_L,compound::correlation::estimate_isobaric_heat_capacity_as_gas_from_rowlinson_poling(Tc,T,omega,C_L))));
    }
}


TEST_CASE( "Pitzer method purity", "[correlation]" ) {
	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::temperature<double> T (si::standard_temperature);
	si::specific_energy<double> H_v = 31.3*si::kilojoule/si::mole / M;
	double omega(0.304);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_latent_heat_of_vaporization_from_pitzer(omega, M, T, Tc) == compound::correlation::estimate_latent_heat_of_vaporization_from_pitzer(omega, M, T, Tc));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_acentric_factor_from_pitzer(H_v, M, T, Tc) == compound::correlation::estimate_acentric_factor_from_pitzer(H_v, M, T, Tc));
    }
}

TEST_CASE( "Pitzer method accuracy", "[correlation]" ) {
    si::UnitAdapter<double> broad (0.4);
    si::UnitAdapter<double> narrow(0.3);

	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::temperature<double> T (si::standard_temperature);
	si::specific_energy<double> H_v = 518.0*si::kilojoule/si::kilogram;
	double omega(0.304);

	// std::cout << compound::correlation::estimate_latent_heat_of_vaporization_from_pitzer(omega, M, T, Tc).to_string() << "  " << H_v.to_string() << std::endl;
	// std::cout << compound::correlation::estimate_acentric_factor_from_pitzer(H_v, M, T, Tc) << "  " << omega << std::endl;

	SECTION("Pitzer method must agree on predictions to within 30%"){
    	CHECK(narrow.equal(H_v, compound::correlation::estimate_latent_heat_of_vaporization_from_pitzer(omega, M, T, Tc)));
    }

	SECTION("Pitzer method must agree on predictions to within 40%"){
    	CHECK(broad.equal(omega, compound::correlation::estimate_acentric_factor_from_pitzer(H_v, M, T, Tc)));
    }
}

TEST_CASE( "Pitzer consistency", "[correlation]" ) {
    si::UnitAdapter<double> adapter(1e-4);

	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::temperature<double> T (si::standard_temperature);
	si::specific_energy<double> H_v = 518.0*si::kilojoule/si::kilogram;
	double omega(0.304);

    SECTION("Pitzer method must be invertible"){
    	CHECK(adapter.equal(H_v, compound::correlation::estimate_latent_heat_of_vaporization_from_pitzer(compound::correlation::estimate_acentric_factor_from_pitzer(H_v, M, T, Tc), M, T, Tc)));
    	CHECK(adapter.equal(omega, compound::correlation::estimate_acentric_factor_from_pitzer(compound::correlation::estimate_latent_heat_of_vaporization_from_pitzer(omega, M, T, Tc), M, T, Tc)));
    }
}


TEST_CASE( "Letsou-Stiel method purity", "[correlation]" ) {
	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	si::temperature<double> T (si::standard_temperature);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::pressure<double> pc (47.0*si::bar);
	si::dynamic_viscosity<double> eta_L = 0.316*si::centipoise;
	double omega(0.304);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_viscosity_as_liquid_from_letsou_stiel(omega, M, T, Tc, pc) == compound::correlation::estimate_viscosity_as_liquid_from_letsou_stiel(omega, M, T, Tc, pc));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_acentric_factor_from_letsou_stiel(eta_L, M, T, Tc, pc) == compound::correlation::estimate_acentric_factor_from_letsou_stiel(eta_L, M, T, Tc, pc));
    }
}

TEST_CASE( "Letsou-Stiel method accuracy", "[correlation]" ) {
    si::UnitAdapter<double> adapter(0.3);

	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	si::temperature<double> T (si::standard_temperature);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::pressure<double> pc (47.0*si::bar);
	si::dynamic_viscosity<double> eta_L = 0.316*si::centipoise;
	double omega(0.304);

	// std::cout << compound::correlation::estimate_viscosity_as_liquid_from_letsou_stiel(omega, M, T, Tc, pc).to_string() << "  " << eta_L.to_string() << std::endl;
	// std::cout << compound::correlation::estimate_acentric_factor_from_letsou_stiel(eta_L, M, T, Tc, pc) << "  " << omega << std::endl;

	SECTION("Letsou-Stiel method must agree on predictions to within 30%"){
    	CHECK(adapter.equal(eta_L, compound::correlation::estimate_viscosity_as_liquid_from_letsou_stiel(omega, M, T, Tc, pc)));
    }

	SECTION("Letsou-Stiel method must agree on predictions to within 30%"){
    	CHECK(adapter.equal(omega, compound::correlation::estimate_acentric_factor_from_letsou_stiel(eta_L, M, T, Tc, pc)));
    }
}

TEST_CASE( "Letsou-Stiel consistency", "[correlation]" ) {
    si::UnitAdapter<double> adapter(1e-4);

	// properties of acetone
	si::molar_mass<double> M (58.080*si::gram/si::mole);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::temperature<double> T (si::standard_temperature);
	si::pressure<double> pc (47.0*si::bar);
	si::dynamic_viscosity<double> eta_L = 0.316*si::centipoise;
	double omega(0.304);

    SECTION("Letsou-Stiel method must be invertible"){
    	CHECK(adapter.equal(eta_L, compound::correlation::estimate_viscosity_as_liquid_from_letsou_stiel(compound::correlation::estimate_acentric_factor_from_letsou_stiel(eta_L, M, T, Tc, pc), M, T, Tc, pc)));
    	CHECK(adapter.equal(omega, compound::correlation::estimate_acentric_factor_from_letsou_stiel(compound::correlation::estimate_viscosity_as_liquid_from_letsou_stiel(omega, M, T, Tc, pc), M, T, Tc, pc)));
    }
}




TEST_CASE( "Tee-Gotoh-Steward method purity", "[correlation]" ) {
	// properties of acetone
	// si::length<double> sigma = 0.469*si::nanometer;
	si::temperature<double> Tc (508.1*si::kelvin);
	si::pressure<double> pc (47.0*si::bar);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_molecular_diameter_from_tee_gotoh_steward(Tc, pc) == compound::correlation::estimate_molecular_diameter_from_tee_gotoh_steward(Tc, pc));
    }

 // SECTION("Calling a function twiced with the same arguments must produce the same results"){
 //     CHECK(compound::correlation::estimate_acentric_factor_from_tee_gotoh_steward(sigma, Tc, pc) == compound::correlation::estimate_acentric_factor_from_tee_gotoh_steward(sigma, Tc, pc));
 // }
}

TEST_CASE( "Tee-Gotoh-Steward method accuracy", "[correlation]" ) {
    si::UnitAdapter<double> adapter(0.3);

	// properties of acetone
	si::length<double> sigma = 0.469*si::nanometer;
	si::temperature<double> Tc (508.1*si::kelvin);
	si::pressure<double> pc (47.0*si::bar);
	// double omega(0.304);

	// std::cout << compound::correlation::estimate_molecular_diameter_from_tee_gotoh_steward(Tc, pc).to_string() << "  " << sigma.to_string() << std::endl;
	// std::cout << compound::correlation::estimate_acentric_factor_from_tee_gotoh_steward(sigma, Tc, pc) << "  " << omega << std::endl;

	SECTION("Tee-Gotoh-Steward method must agree on predictions to within 30%"){
    	CHECK(adapter.equal(sigma, compound::correlation::estimate_molecular_diameter_from_tee_gotoh_steward(Tc, pc)));
    }

 // SECTION("Tee-Gotoh-Steward method must agree on predictions to within 30%"){
 //    	CHECK(adapter.equal(omega, compound::correlation::estimate_acentric_factor_from_tee_gotoh_steward(sigma, Tc, pc)));
 // }
}


TEST_CASE( "Lee-Kesler method purity", "[correlation]" ) {
	// properties of acetone
	si::temperature<double> T (si::standard_temperature);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::pressure<double> pc (47.0*si::bar);
	// si::pressure<double> pvL = 9.639 * si::kilopascal;
	double omega(0.304);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_vapor_pressure_as_liquid_from_lee_kesler(omega, T, Tc, pc) == compound::correlation::estimate_vapor_pressure_as_liquid_from_lee_kesler(omega, T, Tc, pc));
    }

	// SECTION("Calling a function twiced with the same arguments must produce the same results"){
    // 	CHECK(compound::correlation::estimate_acentric_factor_from_lee_kesler(pvL, T, Tc, pc) == compound::correlation::estimate_acentric_factor_from_lee_kesler(pvL, T, Tc, pc));
    // }
}

TEST_CASE( "Lee-Kesler method accuracy", "[correlation]" ) {
    si::UnitAdapter<double> adapter(0.3);

	// properties of acetone
	si::temperature<double> T (si::standard_temperature);
	si::temperature<double> Tc (508.1*si::kelvin);
	si::pressure<double> pc (47.0*si::bar);
	si::pressure<double> pvL = 9.639 * si::kilopascal;
	double omega(0.304);

	// std::cout << compound::correlation::estimate_vapor_pressure_as_liquid_from_lee_kesler(omega, T, Tc, pc).to_string() << "  " << pvL.to_string() << std::endl;
	// std::cout << compound::correlation::estimate_acentric_factor_from_lee_kesler(pvL, T, Tc, pc) << "  " << omega << std::endl;

	SECTION("Lee-Kesler method must agree on predictions to within 30%"){
    	CHECK(adapter.equal(pvL, compound::correlation::estimate_vapor_pressure_as_liquid_from_lee_kesler(omega, T, Tc, pc)));
    }

	// SECTION("Lee-Kesler method must agree on predictions to within 30%"){
    // 	CHECK(adapter.equal(omega, compound::correlation::estimate_acentric_factor_from_lee_kesler(pvL, T, Tc, pc)));
    // }
}

// TEST_CASE( "Lee-Kesler consistency", "[correlation]" ) {
// 	// properties of acetone
// 	si::temperature<double> T (si::standard_temperature);
// 	si::temperature<double> Tc (508.1*si::kelvin);
// 	si::pressure<double> pc (47.0*si::bar);
// 	si::pressure<double> pvL = 9.639 * si::kilopascal;
// 	double omega(0.304);

//     SECTION("Lee-Kesler method must be invertible"){
//     	CHECK(adapter.equal(pvL, compound::correlation::estimate_vapor_pressure_as_liquid_from_lee_kesler(compound::correlation::estimate_acentric_factor_from_lee_kesler(pvL, T, Tc, pc), T, Tc, pc)));
//     	CHECK(adapter.equal(omega, compound::correlation::estimate_acentric_factor_from_lee_kesler(compound::correlation::estimate_vapor_pressure_as_liquid_from_lee_kesler(omega, T, Tc, pc), T, Tc, pc)));
//     }
// }




TEST_CASE( "Bird-Steward-Lightfoot method purity", "[correlation]" ) {
	// properties of acetone
	si::molar_volume<double> Vc (209.0 * si::centimeter3/si::mole);
	si::molar_volume<double> VL (0.0000740 * si::meter3/si::mole);
	si::length<double> sigma = 0.469*si::nanometer;

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_critical_molar_volume_from_bird_steward_lightfoot_1(sigma) == compound::correlation::estimate_critical_molar_volume_from_bird_steward_lightfoot_1(sigma));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_molecular_diameter_from_bird_steward_lightfoot_1(Vc) == compound::correlation::estimate_molecular_diameter_from_bird_steward_lightfoot_1(Vc));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_molecular_diameter_from_bird_steward_lightfoot_2(VL) == compound::correlation::estimate_molecular_diameter_from_bird_steward_lightfoot_2(VL));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma) == compound::correlation::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma));
    }
}

TEST_CASE( "Bird-Steward-Lightfoot method accuracy", "[correlation]" ) {
    si::UnitAdapter<double> broad (0.4);
    si::UnitAdapter<double> narrow(0.3);

	// properties of acetone
	si::molar_volume<double> Vc (209.0 * si::centimeter3/si::mole);
	si::molar_volume<double> VL (0.0000740 * si::meter3/si::mole);
	si::length<double> sigma = 0.469*si::nanometer;

	// std::cout << compound::correlation::estimate_critical_molar_volume_from_bird_steward_lightfoot_1(sigma).to_string() << "  " << Vc.to_string() << std::endl;
	// std::cout << compound::correlation::estimate_molecular_diameter_from_bird_steward_lightfoot_1(Vc).to_string() << "  " << sigma.to_string() << std::endl;

	// std::cout << compound::correlation::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma).to_string() << "  " << VL.to_string() << std::endl;
	// std::cout << compound::correlation::estimate_molecular_diameter_from_bird_steward_lightfoot_2(VL).to_string() << "  " << sigma.to_string() << std::endl;

	SECTION("Bird-Steward-Lightfoot method must agree on predictions to within 30%"){
    	CHECK(narrow.equal(Vc, compound::correlation::estimate_critical_molar_volume_from_bird_steward_lightfoot_1(sigma)));
    }

	SECTION("Bird-Steward-Lightfoot method must agree on predictions to within 30%"){
    	CHECK(narrow.equal(sigma, compound::correlation::estimate_molecular_diameter_from_bird_steward_lightfoot_1(Vc)));
    }

	SECTION("Bird-Steward-Lightfoot method must agree on predictions to within 40%"){
    	CHECK(broad.equal(VL, compound::correlation::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma)));
    }

	SECTION("Bird-Steward-Lightfoot method must agree on predictions to within 40%"){
    	CHECK(broad.equal(sigma, compound::correlation::estimate_molecular_diameter_from_bird_steward_lightfoot_2(VL)));
    }
}

TEST_CASE( "Bird-Steward-Lightfoot consistency", "[correlation]" ) {
    si::UnitAdapter<double> adapter(1e-4);

	// properties of acetone
	si::molar_volume<double> Vc (209.0 * si::centimeter3/si::mole);
	si::molar_volume<double> VL (0.0000740 * si::meter3/si::mole);
	si::length<double> sigma = 0.469*si::nanometer;

    SECTION("Bird-Steward-Lightfoot method must be invertible"){
    	CHECK(adapter.equal(Vc, compound::correlation::estimate_critical_molar_volume_from_bird_steward_lightfoot_1(compound::correlation::estimate_molecular_diameter_from_bird_steward_lightfoot_1(Vc))));
    	CHECK(adapter.equal(sigma, compound::correlation::estimate_molecular_diameter_from_bird_steward_lightfoot_1(compound::correlation::estimate_critical_molar_volume_from_bird_steward_lightfoot_1(sigma))));
    	CHECK(adapter.equal(VL, compound::correlation::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(compound::correlation::estimate_molecular_diameter_from_bird_steward_lightfoot_2(VL))));
    	CHECK(adapter.equal(sigma, compound::correlation::estimate_molecular_diameter_from_bird_steward_lightfoot_2(compound::correlation::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma))));
    }
}





TEST_CASE( "Goodman method purity", "[correlation]" ) {
	// properties of acetone
	si::molar_volume<double> VL (0.0000740 * si::meter3/si::mole);
	si::temperature<double> T (si::standard_temperature);
	si::temperature<double> T0 (178.5*si::kelvin);
	si::molar_volume<double> VS = compound::correlation::estimate_molar_volume_as_solid_from_goodman(VL,T,T0);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_molar_volume_as_solid_from_goodman(VL,T,T0) == compound::correlation::estimate_molar_volume_as_solid_from_goodman(VL,T,T0));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_molar_volume_as_liquid_from_goodman(VS,T,T0) == compound::correlation::estimate_molar_volume_as_liquid_from_goodman(VS,T,T0));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_triple_point_temperature_from_goodman(VL,VS,T) == compound::correlation::estimate_triple_point_temperature_from_goodman(VL,VS,T));
    }
}

TEST_CASE( "Goodman method accuracy", "[correlation]" ) {
    si::UnitAdapter<double> adapter(0.3);

	// properties of acetone
	si::molar_volume<double> VL (0.0000740 * si::meter3/si::mole);
	si::temperature<double> T (si::standard_temperature);
	si::temperature<double> T0 (178.5*si::kelvin);
	si::molar_volume<double> VS = compound::correlation::estimate_molar_volume_as_solid_from_goodman(VL,T,T0);

	// std::cout << compound::correlation::estimate_molar_volume_as_solid_from_goodman(VL,T,T0).to_string() << "  " << VS.to_string() << std::endl;
	// std::cout << compound::correlation::estimate_molar_volume_as_liquid_from_goodman(VS,T,T0).to_string() << "  " << VL.to_string() << std::endl;
	// std::cout << compound::correlation::estimate_triple_point_temperature_from_goodman(VL,T,T0).to_string() << "  " << T0.to_string() << std::endl;

	SECTION("Goodman method must agree on predictions to within 30%"){
    	CHECK(adapter.equal(VS, compound::correlation::estimate_molar_volume_as_solid_from_goodman(VL,T,T0)));
    }

	SECTION("Goodman method must agree on predictions to within 30%"){
    	CHECK(adapter.equal(VL, compound::correlation::estimate_molar_volume_as_liquid_from_goodman(VS,T,T0)));
    }

	SECTION("Goodman method must agree on predictions to within 30%"){
    	CHECK(adapter.equal(T0, compound::correlation::estimate_triple_point_temperature_from_goodman(VL,VS,T)));
    }
}

TEST_CASE( "Goodman consistency", "[correlation]" ) {
    si::UnitAdapter<double> adapter(1e-4);

	// properties of acetone
	si::molar_volume<double> VL (0.0000740 * si::meter3/si::mole);
	si::temperature<double> T (si::standard_temperature);
	si::temperature<double> T0 (178.5*si::kelvin);
	si::molar_volume<double> VS = compound::correlation::estimate_molar_volume_as_solid_from_goodman(VL,T,T0);

    SECTION("Goodman method must be invertible"){
    	CHECK(adapter.equal(VS, compound::correlation::estimate_molar_volume_as_solid_from_goodman(compound::correlation::estimate_molar_volume_as_liquid_from_goodman(VS,T,T0),T,T0)));
    	CHECK(adapter.equal(VL, compound::correlation::estimate_molar_volume_as_liquid_from_goodman(compound::correlation::estimate_molar_volume_as_solid_from_goodman(VL,T,T0),T,T0)));
    	CHECK(adapter.equal(T0, compound::correlation::estimate_triple_point_temperature_from_goodman(
    		compound::correlation::estimate_molar_volume_as_liquid_from_goodman(VS,T,T0),
    		compound::correlation::estimate_molar_volume_as_solid_from_goodman(VL,T,T0), T
    	)));
    }
}





TEST_CASE( "Eucken method purity", "[correlation]" ) {
	// properties of chloroform
	si::molar_mass<double> M (119.377*si::gram/si::mole);
	si::dynamic_viscosity<double> eta_G (1.01e-5*si::pascal* si::second);
	si::thermal_conductivity<double> k_G (0.008499 * si::watt / (si::meter*si::kelvin));
	si::specific_heat_capacity<double> C_G (65.33*si::joule/(si::mole*si::kelvin) / M);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_thermal_conductivity_as_gas_from_eucken(eta_G,M,C_G) == compound::correlation::estimate_thermal_conductivity_as_gas_from_eucken(eta_G,M,C_G));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::correlation::estimate_viscosity_as_gas_from_eucken(C_G,M,k_G) == compound::correlation::estimate_viscosity_as_gas_from_eucken(C_G,M,k_G));
    }
}

TEST_CASE( "Eucken method accuracy", "[correlation]" ) {
    si::UnitAdapter<double> adapter(0.3);

	// properties of chloroform
	si::molar_mass<double> M (119.377*si::gram/si::mole);
	si::dynamic_viscosity<double> eta_G (1.01e-5*si::pascal* si::second);
	si::thermal_conductivity<double> k_G (0.008499 * si::watt / (si::meter*si::kelvin));
	si::specific_heat_capacity<double> C_G (65.33*si::joule/(si::mole*si::kelvin) / M);

	// std::cout << compound::correlation::estimate_thermal_conductivity_as_gas_from_eucken(eta_G,M,C_G).to_string() << "  " << k_G.to_string() << std::endl;
	// std::cout << compound::correlation::estimate_viscosity_as_gas_from_eucken(k_G,M,C_G).to_string() << "  " << eta_G.to_string() << std::endl;

	SECTION("Eucken method must agree on predictions to within 30%"){
    	CHECK(adapter.equal(k_G, compound::correlation::estimate_thermal_conductivity_as_gas_from_eucken(eta_G,M,C_G)));
    }

	SECTION("Eucken method must agree on predictions to within 30%"){
    	CHECK(adapter.equal(eta_G, compound::correlation::estimate_viscosity_as_gas_from_eucken(C_G,M,k_G)));
    }
}

TEST_CASE( "Eucken consistency", "[correlation]" ) {
    si::UnitAdapter<double> adapter(1e-4);

	// properties of chloroform
	si::molar_mass<double> M (119.377*si::gram/si::mole);
	si::dynamic_viscosity<double> eta_G (1.01e-5*si::pascal* si::second);
	si::thermal_conductivity<double> k_G (0.008499 * si::watt / (si::meter*si::kelvin));
	si::specific_heat_capacity<double> C_G (65.33*si::joule/(si::mole*si::kelvin) / M);

    SECTION("Eucken method must be invertible"){
    	CHECK(adapter.equal(k_G, compound::correlation::estimate_thermal_conductivity_as_gas_from_eucken(compound::correlation::estimate_viscosity_as_gas_from_eucken(C_G,M,k_G), M, C_G)));
    	CHECK(adapter.equal(eta_G, compound::correlation::estimate_viscosity_as_gas_from_eucken(C_G, M, compound::correlation::estimate_thermal_conductivity_as_gas_from_eucken(eta_G,M,C_G))));
    }
}
