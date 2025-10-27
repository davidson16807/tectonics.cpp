
// std libraries
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include <math/special.hpp>
#include <unit/si.hpp>

#include "Properties.hpp"

namespace orbit {

	TEST_CASE( "get_mean_anomaly_from_eccentric_anomaly()/solve_eccentric_anomaly_from_mean_anomaly() invertibility", "[orbit]" ) {
		Properties<double> properties(glm::dvec3(1,0,0), glm::dvec3(0,0,1), si::gravitational_constant * si::earth_mass / (si::meter3/si::second2));
	    SECTION("For every function there exists another function that negates its effect"){
	    	const double max_i = 20.0;
	    	const double max_j = 20.0;
	    	const double pi = 3.1415926535;
	    	for (double i = 0.0; i < max_i; ++i)
	    	{
		    	for (double j = 0.0; j < max_j; ++j)
		    	{
		    		double M = math::mix(-2.0*pi, 2.0*pi, i/max_i);
		    		double e = math::mix(0.0, 1.1, j/max_j);
					CHECK( 
						properties.get_mean_anomaly_from_eccentric_anomaly(
							properties.solve_eccentric_anomaly_from_mean_anomaly(M, e), e
						) == Approx(M).margin(0.01)
					);
		    	}
		    }
		}
	}
	TEST_CASE( "get_true_anomaly_from_eccentric_anomaly()/get_eccentric_anomaly_from_true_anomaly() invertibility", "[orbit]" ) {
		Properties<double> properties(glm::dvec3(1,0,0), glm::dvec3(0,0,1), si::gravitational_constant * si::earth_mass / (si::meter3/si::second2));
	    SECTION("For every function there exists another function that negates its effect"){
	    	const double max_i = 10.0;
	    	const double max_j = 10.0;
	    	const double pi = 3.14159;
	    	for (double i = 0.0; i < max_i; ++i)
	    	{
		    	for (double j = 0.0; j < max_j; ++j)
		    	{
		    		double nu = math::mix(0.0, pi, i/max_i);
		    		double e = math::mix(0.0, 0.99, j/max_j);
					CHECK( 
						properties.get_true_anomaly_from_eccentric_anomaly(
							properties.get_eccentric_anomaly_from_true_anomaly(nu, e), e
						) == Approx(nu).margin(0.01)
					);
				}
	    	}
		}
	}
	TEST_CASE( "get_semi_latus_rectum_from_semi_major_axis_and_eccentricity()/get_semi_major_axis_from_semi_latus_rectum_and_eccentricity() invertibility", "[orbit]" ) {
		Properties<double> properties(glm::dvec3(1,0,0), glm::dvec3(0,0,1), si::gravitational_constant * si::earth_mass / (si::meter3/si::second2));
	    SECTION("For every function there exists another function that negates its effect"){
	    	const double max_i = 10.0;
	    	const double max_j = 10.0;
	    	const double epsilon = 0.0001;
	    	for (double i = 0.0; i < max_i; ++i)
	    	{
		    	for (double j = 0.0; j < max_j; ++j)
		    	{
		    		double a = math::mix(epsilon, si::astronomical_unit/si::meter, i/max_i);
		    		double e = math::mix(0.0, 1.5, j/max_j);
					CHECK( 
						properties.get_semi_major_axis_from_semi_latus_rectum_and_eccentricity(
							properties.get_semi_latus_rectum_from_semi_major_axis_and_eccentricity(a, e), e
						) == Approx(a).margin(0.01)
					);
				}
	    	}
		}
	}
	TEST_CASE( "get_true_anomaly_from_eccentric_anomaly() congruence", "[orbit]" ) {
		Properties<double> properties(glm::dvec3(1,0,0), glm::dvec3(0,0,1), si::gravitational_constant * si::earth_mass / (si::meter3/si::second2));
	    SECTION("Results of a function repeat after a certain offset"){
	    	const double max_i = 10.0;
	    	const double max_j = 10.0;
	    	const double pi = 3.14159;
	    	const double epsilon = 0.0001;
	    	for (double i = 0.0; i < max_i; ++i)
	    	{
		    	for (double j = 0.0; j < max_j; ++j)
		    	{
		    		double E = math::mix(epsilon, si::astronomical_unit/si::meter, i/max_i);
		    		double e = math::mix(0.0, 1.1, j/max_j);
					CHECK(
						properties.get_true_anomaly_from_eccentric_anomaly(E,e) == 
							Approx(properties.get_true_anomaly_from_eccentric_anomaly(E+2.0*pi,e)).margin(0.01)
					);
				}
	    	}
		}
	}
	/*

	*/
}
