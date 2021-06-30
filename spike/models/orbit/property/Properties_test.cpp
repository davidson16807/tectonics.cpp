
// std libraries
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include <math/lerp.hpp>
#include <units/si.hpp>

#include "Properties.hpp"

/*
TEST_CASE( "get_mean_anomaly_from_eccentric_anomaly() purity", "[orbit]" ) {
	Properties properties(glm::vec3(1,0,0), glm::vec3(0,0,1), si::gravitational_constant * si::earth_mass);
    SECTION("Calling a function twice with the same arguments must produce the same results"){
		CHECK( properties.get_mean_anomaly_from_eccentric_anomaly() );
	}
}
TEST_CASE( "solve_eccentric_anomaly_from_mean_anomaly() purity", "[orbit]" ) {
	Properties properties(glm::vec3(1,0,0), glm::vec3(0,0,1), si::gravitational_constant * si::earth_mass);
    SECTION("Calling a function twice with the same arguments must produce the same results"){
		// CHECK(sizeof(Properties) <= 8 );
	}
}
*/
TEST_CASE( "get_mean_anomaly_from_eccentric_anomaly()/solve_eccentric_anomaly_from_mean_anomaly() invertibility", "[orbit]" ) {
	Properties properties(glm::vec3(1,0,0), glm::vec3(0,0,1), si::gravitational_constant * si::earth_mass / (si::meter3/si::second2));
    SECTION("For every function there exists another function that negates its effect"){
    	const double max_i = 20.0;
    	const double max_j = 20.0;
    	const double pi = 3.1415926535;
    	for (double i = 0.0; i < max_i; ++i)
    	{
	    	for (double j = 0.0; j < max_j; ++j)
	    	{
	    		double M = math::mix(-2.0*pi, 2.0*pi, i/max_i);
	    		double eccentricity = math::mix(0.0, 1.1, j/max_j);
				CHECK( 
					properties.get_mean_anomaly_from_eccentric_anomaly(
						properties.solve_eccentric_anomaly_from_mean_anomaly(M, eccentricity), eccentricity
					) == Approx(M).margin(0.01)
				);
	    	}
	    }
	}
}
TEST_CASE( "get_true_anomaly_from_eccentric_anomaly()/solve_eccentric_anomaly_from_true_anomaly() invertibility", "[orbit]" ) {
	Properties properties(glm::vec3(1,0,0), glm::vec3(0,0,1), si::gravitational_constant * si::earth_mass / (si::meter3/si::second2));
    SECTION("For every function there exists another function that negates its effect"){
    	const double max_i = 10.0;
    	const double max_j = 10.0;
    	const double pi = 3.14159;
    	for (double i = 0.0; i < max_i; ++i)
    	{
	    	for (double j = 0.0; j < max_j; ++j)
	    	{
	    		double nu = math::mix(0.0, pi, i/max_i);
	    		double eccentricity = math::mix(0.0, 0.99, j/max_j);
				CHECK( 
					properties.get_true_anomaly_from_eccentric_anomaly(
						properties.solve_eccentric_anomaly_from_true_anomaly(nu, eccentricity), eccentricity
					) == Approx(nu).margin(0.01)
				);
			}
    	}
	}
}
/*
*/