
// std libraries
#include <iostream>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include <unit/si.hpp>

#include "ElementsAndState.hpp"

#include "Elements_test_tools.hpp"
#include "State_test_tools.hpp"

namespace orbit {

	TEST_CASE( "get_elements_from_state()/get_state_from_elements() invertibility", "[orbit]" ) {
		property::Properties<double> properties(glm::dvec3(1,0,0), glm::dvec3(0,0,1), si::gravitational_constant * si::solar_mass / (si::meter3/si::second2));
		ElementsAndState<double> conversion(properties);
	    SECTION("For a given function there exists another function that negates its effect") {
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
		/*
	    SECTION("Same as above, but starting from the other direction") {
	    	const double max_samples = 4.0;
	    	for (double uxi = 0.0; uxi < max_samples; ++uxi) {
	    	for (double uyi = 0.0; uyi < max_samples; ++uyi) {
	    	for (double uzi = 0.0; uzi < max_samples; ++uzi) {
	    	for (double vxi = 0.0; vxi < max_samples; ++vxi) {
	    	for (double vyi = 0.0; vyi < max_samples; ++vyi) {
	    	for (double vzi = 0.0; vzi < max_samples; ++vzi) {
	    		double ux = math::mix(-si::astronomical_unit/si::meter, si::astronomical_unit/si::meter, uxi/max_samples);
	    		double uy = math::mix(-si::astronomical_unit/si::meter, si::astronomical_unit/si::meter, uyi/max_samples);
	    		double uz = math::mix(-si::astronomical_unit/si::meter, si::astronomical_unit/si::meter, uzi/max_samples);
	    		double vx = math::mix(-10000.0, 10000.0, vxi/max_samples);
	    		double vy = math::mix(-10000.0, 10000.0, vyi/max_samples);
	    		double vz = math::mix(-10000.0, 10000.0, vzi/max_samples);
	    		State<double> state(glm::vec3(ux,uy,uz), glm::vec3(vx,vy,vz));
	    		State<double> reproduced = conversion.get_state_from_elements(conversion.get_elements_from_state(state));
				STATE_EQUAL(reproduced, state);
	    	}}}}}}
		}
		*/
	}
	TEST_CASE( "get_state_from_elements() inclination congruence", "[orbit]" ) {
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
	    		State<double> baseline = conversion.get_state_from_elements(Elements(a,e,i,O,w,M));
				STATE_EQUAL(baseline, conversion.get_state_from_elements(Elements(a,e,i+2.0*pi,O,w,M)));
				STATE_EQUAL(baseline, conversion.get_state_from_elements(Elements(a,e,i,O+2.0*pi,w,M)));
				STATE_EQUAL(baseline, conversion.get_state_from_elements(Elements(a,e,i,O,w+2.0*pi,M)));
				STATE_EQUAL(baseline, conversion.get_state_from_elements(Elements(a,e,i,O,w,M+2.0*pi)));
	    	}}}}}}
		}
	}

}

