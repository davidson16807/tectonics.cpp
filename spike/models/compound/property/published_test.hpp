
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "published.hpp"


TEST_CASE( "Klincewicz method purity", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	si::temperature Tb (329.20*si::kelvin);
	int A (10);
    SECTION("Calling a function twice with the same arguments must produce the same results"){
    	CHECK(si::is_within_fraction( compound::property::estimate_critical_temperature_from_klincewicz(M, Tb), 
    		                      compound::property::estimate_critical_temperature_from_klincewicz(M, Tb), 1e-4));
    	CHECK(si::is_within_fraction( compound::property::estimate_critical_pressure_from_klincewicz(M, A), 
    		                      compound::property::estimate_critical_pressure_from_klincewicz(M, A), 1e-4));
    	CHECK(si::is_within_fraction( compound::property::estimate_critical_molar_volume_from_klincewicz(M, A), 
    		                      compound::property::estimate_critical_molar_volume_from_klincewicz(M, A), 1e-4));
    }
}
TEST_CASE( "Klincewicz method accuracy", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	si::temperature Tb (329.20*si::kelvin);
	int A (10);
	si::pressure pc (47.0*si::bar);
	si::temperature Tc (508.1*si::kelvin);
	si::molar_volume Vc (209.0 * si::centimeter3/si::mole);

    SECTION("Klincewicz method must predict properties of acetone to within 30%"){
    	CHECK(si::is_within_fraction(compound::property::estimate_critical_temperature_from_klincewicz(M, Tb), Tc, 0.3));
    	CHECK(si::is_within_fraction(compound::property::estimate_critical_pressure_from_klincewicz(M, A), pc, 0.3));
    	CHECK(si::is_within_fraction(compound::property::estimate_critical_molar_volume_from_klincewicz(M, A), Vc, 0.3));
    }
}
TEST_CASE( "Ihmels method purity", "[properties]" ) {
	// properties of acetone
	si::pressure pc (47.0*si::bar);
	si::temperature Tc (508.1*si::kelvin);
	si::molar_volume Vc (209.0 * si::centimeter3/si::mole);

    SECTION("Calling a function twice with the same arguments must produce the same results"){
    	CHECK(si::is_within_fraction(compound::property::estimate_critical_temperature_from_ihmels(pc, Vc), compound::property::estimate_critical_temperature_from_ihmels(pc, Vc), 1e-4));
    	CHECK(si::is_within_fraction(compound::property::estimate_critical_pressure_from_ihmels(Tc, Vc), compound::property::estimate_critical_pressure_from_ihmels(Tc, Vc), 1e-4));
    	CHECK(si::is_within_fraction(compound::property::estimate_critical_molar_volume_from_ihmels(Tc, pc), compound::property::estimate_critical_molar_volume_from_ihmels(Tc, pc), 1e-4));
    }
}
TEST_CASE( "Ihmels method accuracy", "[properties]" ) {
	// properties of acetone
	si::pressure pc (47.0*si::bar);
	si::temperature Tc (508.1*si::kelvin);
	si::molar_volume Vc (209.0 * si::centimeter3/si::mole);

    SECTION("Ihmels method must predict properties of acetone to within 30%"){
    	CHECK(si::is_within_fraction(compound::property::estimate_critical_temperature_from_ihmels(pc, Vc), Tc, 0.3));
    	CHECK(si::is_within_fraction(compound::property::estimate_critical_pressure_from_ihmels(Tc, Vc), pc, 0.3));
    	CHECK(si::is_within_fraction(compound::property::estimate_critical_molar_volume_from_ihmels(Tc, pc), Vc, 0.3));
    }
}

TEST_CASE( "Klincewicz-Ihmels consistency", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	si::temperature Tb (329.20*si::kelvin);
	int A (10);
	si::pressure pc (47.0*si::bar);
	si::temperature Tc (508.1*si::kelvin);
	si::molar_volume Vc (209.0 * si::centimeter3/si::mole);

    SECTION("Klincewicz method and Ihmels method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(compound::property::estimate_critical_temperature_from_ihmels(pc, Vc), compound::property::estimate_critical_temperature_from_klincewicz(M, Tb), 0.3));
    	CHECK(si::is_within_fraction(compound::property::estimate_critical_pressure_from_ihmels(Tc, Vc), compound::property::estimate_critical_pressure_from_klincewicz(M,A), 0.3));
    	CHECK(si::is_within_fraction(compound::property::estimate_critical_molar_volume_from_ihmels(Tc,pc), compound::property::estimate_critical_molar_volume_from_klincewicz(M,A), 0.3));
    }
}

TEST_CASE( "Klincewicz/Ihmels accuracy", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	int A (10);
	si::temperature Tc (508.1*si::kelvin);
	double Zc (0.233f);
    SECTION("Combining Klincewicz and Ihmels must predict properties of acetone to within 30%"){
		si::pressure pc2 (compound::property::estimate_critical_pressure_from_klincewicz(M, A));
		si::molar_volume Vc2 (compound::property::estimate_critical_molar_volume_from_klincewicz(M, A));
		si::temperature Tc2 (compound::property::estimate_critical_temperature_from_ihmels(pc2, Vc2));
		double Zc2 (compound::property::get_critical_compressibility(pc2, Tc2, Vc2));
    	CHECK(si::is_within_fraction(Tc2, Tc, 0.3));
    	CHECK(si::is_within_fraction(Zc2, Zc, 0.3));
    }
}

TEST_CASE( "Sheffy-Johnson method purity", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	si::temperature Tm (178.5 * si::kelvin);
	si::temperature T (si::standard_temperature);

    SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(M,T,Tm) == compound::property::estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(M,T,Tm));
    }
}

TEST_CASE( "Sheffy-Johnson method accuracy", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	si::temperature Tm (178.5 * si::kelvin);
	si::temperature T (si::standard_temperature);
	si::thermal_conductivity kL (0.161 * si::watt/(si::meter*si::kelvin));

    SECTION("Sheffy-Johnson method must predict properties of acetone to within 30%"){
    	CHECK(si::is_within_fraction(compound::property::estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(M,T,Tm), kL, 0.3));
    }
}

TEST_CASE( "Sato-Riedel method purity", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	si::temperature Tc (508.1*si::kelvin);
	si::temperature Tb (329.20*si::kelvin);
	si::temperature T (si::standard_temperature);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_thermal_conductivity_as_liquid_from_sato_riedel(M,T,Tb,Tc) == compound::property::estimate_thermal_conductivity_as_liquid_from_sato_riedel(M,T,Tb,Tc));
    }
}

TEST_CASE( "Sato-Riedel method accuracy", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	si::temperature Tc (508.1*si::kelvin);
	si::temperature Tb (329.20*si::kelvin);
	si::temperature T (si::standard_temperature);
	si::thermal_conductivity kL (0.161 * si::watt/(si::meter*si::kelvin));

    SECTION("Sato-Riedel method must predict properties of acetone to within 30%"){
    	CHECK(si::is_within_fraction(compound::property::estimate_thermal_conductivity_as_liquid_from_sato_riedel(M,T,Tb,Tc), kL, 0.3));
    }
}

TEST_CASE( "Sheffy-Johnson/Sato-Riedel consistency", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	si::temperature Tc (508.1*si::kelvin);
	si::temperature Tb (329.20*si::kelvin);
	si::temperature Tm (178.5* si::kelvin);
	si::temperature T (si::standard_temperature);

    SECTION("Sheffy-Johnson method and Sato-Riedel method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(compound::property::estimate_thermal_conductivity_as_liquid_from_sato_riedel(M,T,Tb,Tc), compound::property::estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(M,T,Tm), 0.3));
    }
}


TEST_CASE( "Rowlinson-Poling method purity", "[properties]" ) {
	// properties of acetone
	si::temperature Tc (508.1*si::kelvin);
	si::temperature T (si::standard_temperature);
	si::molar_heat_capacity C_L (125.0 * si::joule/(si::mole*si::kelvin));
	si::molar_heat_capacity C_G (75.0 * si::joule/(si::mole*si::kelvin));
	double omega(0.304);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_constant_pressure_heat_capacity_as_liquid_from_rowlinson_poling(Tc,T,omega,C_G) == compound::property::estimate_constant_pressure_heat_capacity_as_liquid_from_rowlinson_poling(Tc,T,omega,C_G));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_constant_pressure_heat_capacity_as_gas_from_rowlinson_poling(Tc,T,omega,C_L) == compound::property::estimate_constant_pressure_heat_capacity_as_gas_from_rowlinson_poling(Tc,T,omega,C_L));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_acentric_factor_from_rowlinson_poling(Tc,T,C_L,C_G) == compound::property::estimate_acentric_factor_from_rowlinson_poling(Tc,T,C_L,C_G));
    }
}

TEST_CASE( "Rowlinson-Poling method accuracy", "[properties]" ) {
	// properties of acetone
	si::temperature Tc (508.1*si::kelvin);
	si::temperature T (si::standard_temperature);
	si::molar_heat_capacity C_L (125.0 * si::joule/(si::mole*si::kelvin));
	si::molar_heat_capacity C_G (75.0 * si::joule/(si::mole*si::kelvin));
	double omega(0.304);

	SECTION("Rowlinson-Poling method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(C_L, compound::property::estimate_constant_pressure_heat_capacity_as_liquid_from_rowlinson_poling(Tc,T,omega,C_G), 0.3));
    }

	SECTION("Rowlinson-Poling method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(C_G, compound::property::estimate_constant_pressure_heat_capacity_as_gas_from_rowlinson_poling(Tc,T,omega,C_L), 0.3));
    }

	SECTION("Rowlinson-Poling method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(omega, compound::property::estimate_acentric_factor_from_rowlinson_poling(Tc,T,C_L,C_G), 0.3));
    }
}

TEST_CASE( "Rowlinson-Poling consistency", "[properties]" ) {
	// properties of acetone
	si::temperature Tc (508.1*si::kelvin);
	si::temperature T (si::standard_temperature);
	si::molar_heat_capacity C_L (125.0 * si::joule/(si::mole*si::kelvin));
	si::molar_heat_capacity C_G (75.0 * si::joule/(si::mole*si::kelvin));
	double omega(0.304);

    SECTION("Rowlinson-Poling method must be invertible"){
    	CHECK(si::is_within_fraction(C_L, compound::property::estimate_constant_pressure_heat_capacity_as_liquid_from_rowlinson_poling(Tc,T,compound::property::estimate_acentric_factor_from_rowlinson_poling(Tc,T,C_L,C_G),C_G), 1e-4));
    	CHECK(si::is_within_fraction(C_G, compound::property::estimate_constant_pressure_heat_capacity_as_gas_from_rowlinson_poling(Tc,T,compound::property::estimate_acentric_factor_from_rowlinson_poling(Tc,T,C_L,C_G),C_L), 1e-4));
    	CHECK(si::is_within_fraction(omega, compound::property::estimate_acentric_factor_from_rowlinson_poling(Tc,T,compound::property::estimate_constant_pressure_heat_capacity_as_liquid_from_rowlinson_poling(Tc,T,omega,C_G),C_G), 1e-4));
    	CHECK(si::is_within_fraction(omega, compound::property::estimate_acentric_factor_from_rowlinson_poling(Tc,T,C_L,compound::property::estimate_constant_pressure_heat_capacity_as_gas_from_rowlinson_poling(Tc,T,omega,C_L)), 1e-4));
    }
}


TEST_CASE( "Pitzer method purity", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	si::temperature Tc (508.1*si::kelvin);
	si::temperature T (si::standard_temperature);
	si::specific_energy H_v = 31.3*si::kilojoule/si::mole / M;
	double omega(0.304);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_latent_heat_of_vaporization_from_pitzer(omega, M, T, Tc) == compound::property::estimate_latent_heat_of_vaporization_from_pitzer(omega, M, T, Tc));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_accentric_factor_from_pitzer(H_v, M, T, Tc) == compound::property::estimate_accentric_factor_from_pitzer(H_v, M, T, Tc));
    }
}

TEST_CASE( "Pitzer method accuracy", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	si::temperature Tc (508.1*si::kelvin);
	si::temperature T (si::standard_temperature);
	si::specific_energy H_v = 518.0*si::kilojoule/si::kilogram;
	double omega(0.304);

	// std::cout << compound::property::estimate_latent_heat_of_vaporization_from_pitzer(omega, M, T, Tc).to_string() << "  " << H_v.to_string() << std::endl;
	// std::cout << compound::property::estimate_accentric_factor_from_pitzer(H_v, M, T, Tc) << "  " << omega << std::endl;

	SECTION("Pitzer method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(H_v, compound::property::estimate_latent_heat_of_vaporization_from_pitzer(omega, M, T, Tc), 0.3));
    }

	SECTION("Pitzer method must agree on predictions to within 40%"){
    	CHECK(si::is_within_fraction(omega, compound::property::estimate_accentric_factor_from_pitzer(H_v, M, T, Tc), 0.4));
    }
}

TEST_CASE( "Pitzer consistency", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	si::temperature Tc (508.1*si::kelvin);
	si::temperature T (si::standard_temperature);
	si::specific_energy H_v = 518.0*si::kilojoule/si::kilogram;
	double omega(0.304);

    SECTION("Pitzer method must be invertible"){
    	CHECK(si::is_within_fraction(H_v, compound::property::estimate_latent_heat_of_vaporization_from_pitzer(compound::property::estimate_accentric_factor_from_pitzer(H_v, M, T, Tc), M, T, Tc), 1e-4));
    	CHECK(si::is_within_fraction(omega, compound::property::estimate_accentric_factor_from_pitzer(compound::property::estimate_latent_heat_of_vaporization_from_pitzer(omega, M, T, Tc), M, T, Tc), 1e-4));
    }
}


TEST_CASE( "Letsou-Stiel method purity", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	si::temperature T (si::standard_temperature);
	si::temperature Tc (508.1*si::kelvin);
	si::pressure pc (47.0*si::bar);
	si::dynamic_viscosity eta_L = 0.316*si::centipoise;
	double omega(0.304);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_viscosity_as_liquid_from_letsou_stiel(omega, M, T, Tc, pc) == compound::property::estimate_viscosity_as_liquid_from_letsou_stiel(omega, M, T, Tc, pc));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_acentric_factor_from_letsou_stiel(eta_L, M, T, Tc, pc) == compound::property::estimate_acentric_factor_from_letsou_stiel(eta_L, M, T, Tc, pc));
    }
}

TEST_CASE( "Letsou-Stiel method accuracy", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	si::temperature T (si::standard_temperature);
	si::temperature Tc (508.1*si::kelvin);
	si::pressure pc (47.0*si::bar);
	si::dynamic_viscosity eta_L = 0.316*si::centipoise;
	double omega(0.304);

	// std::cout << compound::property::estimate_viscosity_as_liquid_from_letsou_stiel(omega, M, T, Tc, pc).to_string() << "  " << eta_L.to_string() << std::endl;
	// std::cout << compound::property::estimate_acentric_factor_from_letsou_stiel(eta_L, M, T, Tc, pc) << "  " << omega << std::endl;

	SECTION("Letsou-Stiel method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(eta_L, compound::property::estimate_viscosity_as_liquid_from_letsou_stiel(omega, M, T, Tc, pc), 0.3));
    }

	SECTION("Letsou-Stiel method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(omega, compound::property::estimate_acentric_factor_from_letsou_stiel(eta_L, M, T, Tc, pc), 0.3));
    }
}

TEST_CASE( "Letsou-Stiel consistency", "[properties]" ) {
	// properties of acetone
	si::molar_mass M (58.080*si::gram/si::mole);
	si::temperature Tc (508.1*si::kelvin);
	si::temperature T (si::standard_temperature);
	si::pressure pc (47.0*si::bar);
	si::dynamic_viscosity eta_L = 0.316*si::centipoise;
	double omega(0.304);

    SECTION("Letsou-Stiel method must be invertible"){
    	CHECK(si::is_within_fraction(eta_L, compound::property::estimate_viscosity_as_liquid_from_letsou_stiel(compound::property::estimate_acentric_factor_from_letsou_stiel(eta_L, M, T, Tc, pc), M, T, Tc, pc), 1e-4));
    	CHECK(si::is_within_fraction(omega, compound::property::estimate_acentric_factor_from_letsou_stiel(compound::property::estimate_viscosity_as_liquid_from_letsou_stiel(omega, M, T, Tc, pc), M, T, Tc, pc), 1e-4));
    }
}




TEST_CASE( "Tee-Gotoh-Steward method purity", "[properties]" ) {
	// properties of acetone
	// si::length sigma = 0.469*si::nanometer;
	si::temperature Tc (508.1*si::kelvin);
	si::pressure pc (47.0*si::bar);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_molecular_diameter_from_tee_gotoh_steward(Tc, pc) == compound::property::estimate_molecular_diameter_from_tee_gotoh_steward(Tc, pc));
    }

 // SECTION("Calling a function twiced with the same arguments must produce the same results"){
 //     CHECK(compound::property::estimate_acentric_factor_from_tee_gotoh_steward(sigma, Tc, pc) == compound::property::estimate_acentric_factor_from_tee_gotoh_steward(sigma, Tc, pc));
 // }
}

TEST_CASE( "Tee-Gotoh-Steward method accuracy", "[properties]" ) {
	// properties of acetone
	si::length sigma = 0.469*si::nanometer;
	si::temperature Tc (508.1*si::kelvin);
	si::pressure pc (47.0*si::bar);
	// double omega(0.304);

	// std::cout << compound::property::estimate_molecular_diameter_from_tee_gotoh_steward(Tc, pc).to_string() << "  " << sigma.to_string() << std::endl;
	// std::cout << compound::property::estimate_acentric_factor_from_tee_gotoh_steward(sigma, Tc, pc) << "  " << omega << std::endl;

	SECTION("Tee-Gotoh-Steward method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(sigma, compound::property::estimate_molecular_diameter_from_tee_gotoh_steward(Tc, pc), 0.3));
    }

 // SECTION("Tee-Gotoh-Steward method must agree on predictions to within 30%"){
 //    	CHECK(si::is_within_fraction(omega, compound::property::estimate_acentric_factor_from_tee_gotoh_steward(sigma, Tc, pc), 0.3));
 // }
}


TEST_CASE( "Lee-Kesler method purity", "[properties]" ) {
	// properties of acetone
	si::temperature T (si::standard_temperature);
	si::temperature Tc (508.1*si::kelvin);
	si::pressure pc (47.0*si::bar);
	si::pressure pvL = 9.639 * si::kilopascal;
	double omega(0.304);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_vapor_pressure_as_liquid_from_lee_kesler(omega, T, Tc, pc) == compound::property::estimate_vapor_pressure_as_liquid_from_lee_kesler(omega, T, Tc, pc));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_accentric_factor_from_lee_kesler(pvL, T, Tc, pc) == compound::property::estimate_accentric_factor_from_lee_kesler(pvL, T, Tc, pc));
    }
}

TEST_CASE( "Lee-Kesler method accuracy", "[properties]" ) {
	// properties of acetone
	si::temperature T (si::standard_temperature);
	si::temperature Tc (508.1*si::kelvin);
	si::pressure pc (47.0*si::bar);
	si::pressure pvL = 9.639 * si::kilopascal;
	double omega(0.304);

	// std::cout << compound::property::estimate_vapor_pressure_as_liquid_from_lee_kesler(omega, T, Tc, pc).to_string() << "  " << pvL.to_string() << std::endl;
	// std::cout << compound::property::estimate_accentric_factor_from_lee_kesler(pvL, T, Tc, pc) << "  " << omega << std::endl;

	SECTION("Lee-Kesler method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(pvL, compound::property::estimate_vapor_pressure_as_liquid_from_lee_kesler(omega, T, Tc, pc), 0.3));
    }

	SECTION("Lee-Kesler method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(omega, compound::property::estimate_accentric_factor_from_lee_kesler(pvL, T, Tc, pc), 0.3));
    }
}

TEST_CASE( "Lee-Kesler consistency", "[properties]" ) {
	// properties of acetone
	si::temperature T (si::standard_temperature);
	si::temperature Tc (508.1*si::kelvin);
	si::pressure pc (47.0*si::bar);
	si::pressure pvL = 9.639 * si::kilopascal;
	double omega(0.304);

    SECTION("Letsou-Stiel method must be invertible"){
    	CHECK(si::is_within_fraction(pvL, compound::property::estimate_vapor_pressure_as_liquid_from_lee_kesler(compound::property::estimate_accentric_factor_from_lee_kesler(pvL, T, Tc, pc), T, Tc, pc), 1e-4));
    	CHECK(si::is_within_fraction(omega, compound::property::estimate_accentric_factor_from_lee_kesler(compound::property::estimate_vapor_pressure_as_liquid_from_lee_kesler(omega, T, Tc, pc), T, Tc, pc), 1e-4));
    }
}




TEST_CASE( "Bird-Steward-Lightfoot method purity", "[properties]" ) {
	// properties of acetone
	si::molar_volume Vc (209.0 * si::centimeter3/si::mole);
	si::molar_volume VL (0.0000740 * si::meter3/si::mole);
	si::length sigma = 0.469*si::nanometer;

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_critical_molar_volume_from_bird_steward_lightfoot_1(sigma) == compound::property::estimate_critical_molar_volume_from_bird_steward_lightfoot_1(sigma));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_molecular_diameter_from_bird_steward_lightfoot_1(Vc) == compound::property::estimate_molecular_diameter_from_bird_steward_lightfoot_1(Vc));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_molecular_diameter_from_bird_steward_lightfoot_2(VL) == compound::property::estimate_molecular_diameter_from_bird_steward_lightfoot_2(VL));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma) == compound::property::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma));
    }
}

TEST_CASE( "Bird-Steward-Lightfoot method accuracy", "[properties]" ) {
	// properties of acetone
	si::molar_volume Vc (209.0 * si::centimeter3/si::mole);
	si::molar_volume VL (0.0000740 * si::meter3/si::mole);
	si::length sigma = 0.469*si::nanometer;

	// std::cout << compound::property::estimate_critical_molar_volume_from_bird_steward_lightfoot_1(sigma).to_string() << "  " << Vc.to_string() << std::endl;
	// std::cout << compound::property::estimate_molecular_diameter_from_bird_steward_lightfoot_1(Vc).to_string() << "  " << sigma.to_string() << std::endl;

	// std::cout << compound::property::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma).to_string() << "  " << VL.to_string() << std::endl;
	// std::cout << compound::property::estimate_molecular_diameter_from_bird_steward_lightfoot_2(VL).to_string() << "  " << sigma.to_string() << std::endl;

	SECTION("Bird-Steward-Lightfoot method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(Vc, compound::property::estimate_critical_molar_volume_from_bird_steward_lightfoot_1(sigma), 0.3));
    }

	SECTION("Bird-Steward-Lightfoot method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(sigma, compound::property::estimate_molecular_diameter_from_bird_steward_lightfoot_1(Vc), 0.3));
    }

	SECTION("Bird-Steward-Lightfoot method must agree on predictions to within 40%"){
    	CHECK(si::is_within_fraction(VL, compound::property::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma), 0.4));
    }

	SECTION("Bird-Steward-Lightfoot method must agree on predictions to within 40%"){
    	CHECK(si::is_within_fraction(sigma, compound::property::estimate_molecular_diameter_from_bird_steward_lightfoot_2(VL), 0.4));
    }
}

TEST_CASE( "Bird-Steward-Lightfoot consistency", "[properties]" ) {
	// properties of acetone
	si::molar_volume Vc (209.0 * si::centimeter3/si::mole);
	si::molar_volume VL (0.0000740 * si::meter3/si::mole);
	si::length sigma = 0.469*si::nanometer;

    SECTION("Bird-Steward-Lightfoot method must be invertible"){
    	CHECK(si::is_within_fraction(Vc, compound::property::estimate_critical_molar_volume_from_bird_steward_lightfoot_1(compound::property::estimate_molecular_diameter_from_bird_steward_lightfoot_1(Vc)), 1e-4));
    	CHECK(si::is_within_fraction(sigma, compound::property::estimate_molecular_diameter_from_bird_steward_lightfoot_1(compound::property::estimate_critical_molar_volume_from_bird_steward_lightfoot_1(sigma)), 1e-4));
    	CHECK(si::is_within_fraction(VL, compound::property::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(compound::property::estimate_molecular_diameter_from_bird_steward_lightfoot_2(VL)), 1e-4));
    	CHECK(si::is_within_fraction(sigma, compound::property::estimate_molecular_diameter_from_bird_steward_lightfoot_2(compound::property::estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(sigma)), 1e-4));
    }
}





TEST_CASE( "Goodman method purity", "[properties]" ) {
	// properties of acetone
	si::molar_volume VL (0.0000740 * si::meter3/si::mole);
	si::temperature T (si::standard_temperature);
	si::temperature T0 (178.5*si::kelvin);
	si::molar_volume VS = compound::property::estimate_molar_volume_as_solid_from_goodman(VL,T,T0);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_molar_volume_as_solid_from_goodman(VL,T,T0) == compound::property::estimate_molar_volume_as_solid_from_goodman(VL,T,T0));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_molar_volume_as_liquid_from_goodman(VS,T,T0) == compound::property::estimate_molar_volume_as_liquid_from_goodman(VS,T,T0));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_triple_point_temperature_from_goodman(VL,VS,T) == compound::property::estimate_triple_point_temperature_from_goodman(VL,VS,T));
    }
}

TEST_CASE( "Goodman method accuracy", "[properties]" ) {
	// properties of acetone
	si::molar_volume VL (0.0000740 * si::meter3/si::mole);
	si::temperature T (si::standard_temperature);
	si::temperature T0 (178.5*si::kelvin);
	si::molar_volume VS = compound::property::estimate_molar_volume_as_solid_from_goodman(VL,T,T0);

	// std::cout << compound::property::estimate_molar_volume_as_solid_from_goodman(VL,T,T0).to_string() << "  " << VS.to_string() << std::endl;
	// std::cout << compound::property::estimate_molar_volume_as_liquid_from_goodman(VS,T,T0).to_string() << "  " << VL.to_string() << std::endl;
	// std::cout << compound::property::estimate_triple_point_temperature_from_goodman(VL,T,T0).to_string() << "  " << T0.to_string() << std::endl;

	SECTION("Goodman method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(VS, compound::property::estimate_molar_volume_as_solid_from_goodman(VL,T,T0), 0.3));
    }

	SECTION("Goodman method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(VL, compound::property::estimate_molar_volume_as_liquid_from_goodman(VS,T,T0), 0.3));
    }

	SECTION("Goodman method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(T0, compound::property::estimate_triple_point_temperature_from_goodman(VL,VS,T), 0.3));
    }
}

TEST_CASE( "Goodman consistency", "[properties]" ) {
	// properties of acetone
	si::molar_volume VL (0.0000740 * si::meter3/si::mole);
	si::temperature T (si::standard_temperature);
	si::temperature T0 (178.5*si::kelvin);
	si::molar_volume VS = compound::property::estimate_molar_volume_as_solid_from_goodman(VL,T,T0);

    SECTION("Goodman method must be invertible"){
    	CHECK(si::is_within_fraction(VS, compound::property::estimate_molar_volume_as_solid_from_goodman(compound::property::estimate_molar_volume_as_liquid_from_goodman(VS,T,T0),T,T0), 1e-4));
    	CHECK(si::is_within_fraction(VL, compound::property::estimate_molar_volume_as_liquid_from_goodman(compound::property::estimate_molar_volume_as_solid_from_goodman(VL,T,T0),T,T0), 1e-4));
    	CHECK(si::is_within_fraction(T0, compound::property::estimate_triple_point_temperature_from_goodman(
    		compound::property::estimate_molar_volume_as_liquid_from_goodman(VS,T,T0),
    		compound::property::estimate_molar_volume_as_solid_from_goodman(VL,T,T0), T
    	), 1e-4));
    }
}





TEST_CASE( "Eucken method purity", "[properties]" ) {
	// properties of chloroform
	si::molar_mass M (119.377*si::gram/si::mole);
	si::dynamic_viscosity eta_G (1.01e-5*si::pascal* si::second);
	si::thermal_conductivity k_G (0.008499 * si::watt / (si::meter*si::kelvin));
	si::specific_heat_capacity C_G (65.33*si::joule/(si::mole*si::kelvin) / M);

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_thermal_conductivity_as_gas_from_eucken(eta_G,M,C_G) == compound::property::estimate_thermal_conductivity_as_gas_from_eucken(eta_G,M,C_G));
    }

	SECTION("Calling a function twiced with the same arguments must produce the same results"){
    	CHECK(compound::property::estimate_viscosity_as_gas_from_eucken(C_G,M,k_G) == compound::property::estimate_viscosity_as_gas_from_eucken(C_G,M,k_G));
    }
}

TEST_CASE( "Eucken method accuracy", "[properties]" ) {
	// properties of chloroform
	si::molar_mass M (119.377*si::gram/si::mole);
	si::dynamic_viscosity eta_G (1.01e-5*si::pascal* si::second);
	si::thermal_conductivity k_G (0.008499 * si::watt / (si::meter*si::kelvin));
	si::specific_heat_capacity C_G (65.33*si::joule/(si::mole*si::kelvin) / M);

	// std::cout << compound::property::estimate_thermal_conductivity_as_gas_from_eucken(eta_G,M,C_G).to_string() << "  " << k_G.to_string() << std::endl;
	// std::cout << compound::property::estimate_viscosity_as_gas_from_eucken(k_G,M,C_G).to_string() << "  " << eta_G.to_string() << std::endl;

	SECTION("Eucken method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(k_G, compound::property::estimate_thermal_conductivity_as_gas_from_eucken(eta_G,M,C_G), 0.3));
    }

	SECTION("Eucken method must agree on predictions to within 30%"){
    	CHECK(si::is_within_fraction(eta_G, compound::property::estimate_viscosity_as_gas_from_eucken(C_G,M,k_G), 0.3));
    }
}

TEST_CASE( "Eucken consistency", "[properties]" ) {
	// properties of chloroform
	si::molar_mass M (119.377*si::gram/si::mole);
	si::dynamic_viscosity eta_G (1.01e-5*si::pascal* si::second);
	si::thermal_conductivity k_G (0.008499 * si::watt / (si::meter*si::kelvin));
	si::specific_heat_capacity C_G (65.33*si::joule/(si::mole*si::kelvin) / M);

    SECTION("Eucken method must be invertible"){
    	CHECK(si::is_within_fraction(k_G, compound::property::estimate_thermal_conductivity_as_gas_from_eucken(compound::property::estimate_viscosity_as_gas_from_eucken(C_G,M,k_G), M, C_G), 1e-4));
    	CHECK(si::is_within_fraction(eta_G, compound::property::estimate_viscosity_as_gas_from_eucken(C_G, M, compound::property::estimate_thermal_conductivity_as_gas_from_eucken(eta_G,M,C_G)), 1e-4));
    }
}
