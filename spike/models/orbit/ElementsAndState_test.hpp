
// std libraries
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include <units/si.hpp>

#include "ElementsAndState.hpp"

#include "Elements_test_utils.hpp"

namespace orbit {

	TEST_CASE( "get_elements_from_state()/get_state_from_elements() invertibility", "[orbit]" ) {
		property::Properties<double> properties(glm::dvec3(1,0,0), glm::dvec3(0,0,1), si::gravitational_constant * si::solar_mass / (si::meter3/si::second2));
		ElementsAndState<double> conversion(properties);
	    SECTION("For a given function there exists another function that negates its effect"){
	    	const double max_samples = 3.0;
	    	const double pi = 3.1415926;
	    	for (double ai = 0.0; ai < max_samples; ++ai) {
	    	for (double ei = 0.0; ei < max_samples; ++ei) {
	    	for (double ii = 0.0; ii < max_samples; ++ii) {
	    	for (double Oi = 0.0; Oi < max_samples; ++Oi) {
	    	for (double wi = 0.0; wi < max_samples; ++wi) {
	    	for (double Mi = 0.0; Mi < max_samples; ++Mi) {
	    		double a = math::mix(si::solar_radius/si::meter, si::astronomical_unit/si::meter, ai/max_samples); // `a` must be nonzero to be physically meaningful
	    		double e = math::mix(0.1, 1.2, ei/max_samples); // `e` must be nonzero for tests or else w is not defined
	    		double i = math::mix(0.1, 0.9*pi, ii/max_samples); // `i` must be positive and nonzero for tests or else O is not defined, and must be less than pi to allow reconstruction
	    		double O = math::mix(0.1, 0.9*pi, Oi/max_samples); // `O` must be less than pi to allow reconstruction
	    		double w = math::mix(0.1, 0.9*pi, wi/max_samples); // `w` must be less than pi to allow reconstruction
	    		double M = math::mix(0.1, 0.9*pi, Mi/max_samples); // `M` must be less than pi to allow reconstruction
	    		Elements<double> elements(a,e,i,O,w,M);
	    		Elements<double> reproduced = conversion.get_elements_from_state(conversion.get_state_from_elements(elements));
				ORBIT_EQUAL(reproduced, elements);
	    	}}}}}}
		}
	}

}