
// 3rd-party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in-house libraries
#include "special.hpp"

const float a = 1.6180;
const float b = 3.1415;
const float c = 1.6807;
const float f = 0.25;
const float g = 0.75;

TEST_CASE( "mix purity", "[math]" ) {
    SECTION("calling mix() multiple times returns the same result"){
		CHECK(math::mix(a,b,f) == math::mix(a,b,f));
	}
}

TEST_CASE( "mix identity", "[math]" ) {
    SECTION("calling mix(a,b,0) produces results equivalent to input, a"){
		CHECK(math::mix(a,b,0.0f) == a);
	}
}

TEST_CASE( "mix distributivity over multiplication", "[math]" ) {
    SECTION("calling c*mix(a,b,f) is equivalent to mix(c*a,c*b,f)"){
		CHECK(c*math::mix(a,b,f) == Approx(math::mix(c*a,c*b,f)).margin(0.01));
	}
}


TEST_CASE( "linearstep purity", "[math]" ) {
    SECTION("calling linearstep() multiple times returns the same result"){
		CHECK(math::linearstep(a,b,c) == math::linearstep(a,b,c));
	}
}

TEST_CASE( "linearstep spatial invariance", "[math]" ) {
    SECTION("calling linearstep(a,b,c) is equivalent to linearstep(f+a,f+b,f+c)"){
		CHECK(math::linearstep(a,b,c) == math::linearstep(f+a,f+b,f+c));
	}
}

TEST_CASE( "mix/linearstep invertibility", "[math]" ) {
    SECTION("calling mix(a,b,linearstep(a,b,c)) will reproduce the value for c for any value a<c<b"){
		CHECK(math::mix(a,b,math::linearstep(a,b,c)) == Approx(c).margin(0.01));
	}
    SECTION("calling linearstep(a,b,mix(a,b,f)) will reproduce the value for f for any value 0<f<1"){
		CHECK(math::linearstep(a,b,math::mix(a,b,f)) == Approx(f).margin(0.01));
	}
}



TEST_CASE( "lerp purity", "[math]" ) {
  	const auto xs = std::array<float,2>{0.0f,1.0f};
  	const auto ys = std::array<float,2>{a,b};
    SECTION("calling lerp multiple times returns the same result"){
		CHECK(math::lerp(xs,ys,f) == math::lerp(xs,ys,f));
	}
}

TEST_CASE( "lerp degeneracy to mix", "[math]" ) {
  	const auto xs = std::array<float,2>{0.0f,1.0f};
  	const auto ys = std::array<float,2>{a,b};
    SECTION("calling lerp({0,1}, {a,b}, f) is equivalent to mix(a,b,f)"){
		CHECK(math::lerp(xs,ys,f) == Approx(math::mix(a,b,f)).margin(0.01));
	}
}

TEST_CASE( "lerp degeneracy to constant", "[math]" ) {
  	const auto xs = std::array<float,1>{0.0f};
  	const auto ys = std::array<float,1>{a};
    SECTION("calling lerp with a single control point is equivalent to a constant"){
		CHECK(math::lerp(xs,ys,f) == Approx(a).margin(0.01));
	}
}

TEST_CASE( "lerp commutativity", "[math]" ) {
  	std::mt19937 generator(2);
  	std::uniform_real_distribution<float> uniform(0.0f, 2.0f);
  	const auto xs = std::vector<float>{ -0.5f, 0.75f };
  	const auto xs2 = std::vector<float>{ xs[1], xs[0] };
  	const auto ys = std::vector<float>{ uniform(generator), uniform(generator) };
  	const auto ys2 = std::vector<float>{ ys[1], ys[0] };
    SECTION("calling lerp with interpolant points reversed returns the same result"){
		CHECK(math::lerp(xs,ys,f) == math::lerp(xs2,ys2,f));
	}
}
