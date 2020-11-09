
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "lerp.hpp"

const float a = 1.6180;
const float b = 3.1415;
const float c = 1.6807;
const float f = 0.25;

TEST_CASE( "mix purity", "[lerp]" ) {
    SECTION("calling mix() multiple times returns the same result"){
		CHECK(lerp::mix(a,b,f) == lerp::mix(a,b,f));
	}
}

TEST_CASE( "mix identity", "[lerp]" ) {
    SECTION("calling mix(a,b,0) produces results equivalent to input, a"){
		CHECK(lerp::mix(a,b,0.0f) == a);
	}
}

TEST_CASE( "mix distributivity over multiplication", "[lerp]" ) {
    SECTION("calling c*mix(a,b,f) is equivalent to mix(c*a,c*b,f)"){
		CHECK(c*lerp::mix(a,b,f) == Approx(lerp::mix(c*a,c*b,f)).margin(0.01));
	}
}



TEST_CASE( "linearstep purity", "[lerp]" ) {
    SECTION("calling linearstep() multiple times returns the same result"){
		CHECK(lerp::linearstep(a,b,c) == lerp::linearstep(a,b,c));
	}
}

TEST_CASE( "linearstep spatial invariance", "[lerp]" ) {
    SECTION("calling linearstep(a,b,c) is equivalent to linearstep(f+a,f+b,f+c)"){
		CHECK(lerp::linearstep(a,b,c) == lerp::linearstep(f+a,f+b,f+c));
	}
}

TEST_CASE( "mix/linearstep invertibility", "[lerp]" ) {
    SECTION("calling mix(a,b,linearstep(a,b,c)) will reproduce the value for c for any value a<c<b"){
		CHECK(lerp::mix(a,b,lerp::linearstep(a,b,c)) == Approx(c).margin(0.01));
	}
    SECTION("calling linearstep(a,b,mix(a,b,f)) will reproduce the value for f for any value 0<f<1"){
		CHECK(lerp::linearstep(a,b,lerp::mix(a,b,f)) == Approx(f).margin(0.01));
	}
}



TEST_CASE( "lerp purity", "[lerp]" ) {
  	const auto xs = std::array<float,2>{0.0f,1.0f};
  	const auto ys = std::array<float,2>{a,b};
    SECTION("calling lerp multiple times returns the same result"){
		CHECK(lerp::lerp(xs,ys,f) == lerp::lerp(xs,ys,f));
	}
}

TEST_CASE( "lerp degeneracy to mix", "[lerp]" ) {
  	const auto xs = std::array<float,2>{0.0f,1.0f};
  	const auto ys = std::array<float,2>{a,b};
    SECTION("calling lerp({0,1}, {a,b}, f) is equivalent to mix(a,b,f)"){
		CHECK(lerp::lerp(xs,ys,f) == Approx(lerp::mix(a,b,f)).margin(0.01));
	}
}

TEST_CASE( "integral_of_lerp purity", "[lerp]" ) {
  	std::mt19937 generator(2);
  	std::uniform_real_distribution<float> uniform(0.0f, 2.0f);
  	const auto xs = std::array<float,4>{ -0.5f, 0.75f, 1.0f, 1.5f };
  	const auto ys = std::array<float,4>{ uniform(generator), uniform(generator), uniform(generator), uniform(generator) };
    SECTION("calling lerp multiple times returns the same result"){
		CHECK(lerp::integral_of_lerp(xs,ys,0.0f,1.6807f) == lerp::integral_of_lerp(xs,ys,0.0f,1.6807f));
	}
}

TEST_CASE( "integral_of_lerp equivalence to numerical integration", "[lerp]" ) {
  	std::mt19937 generator(2);
  	std::uniform_real_distribution<float> uniform(0.0f, 2.0f);
  	const auto xs = std::array<float,4>{ -0.5f, 0.75f, 1.0f, 1.5f };
  	const auto ys = std::array<float,4>{ uniform(generator), uniform(generator), uniform(generator), uniform(generator) };
	const float dx = 0.003f;
	for (float x0 = -0.6; x0 <= 1.6f; x0+=0.2)
	{
		for (float x1 = x0; x1 <= 1.6f; x1+=0.2)
		{
			float I1 = 0.0f;
			for (float xi = x0; xi < x1; xi+=dx)
			{
				I1 += dx * lerp::lerp(xs,ys,xi);
			}
			float I2 = lerp::integral_of_lerp(xs,ys,x0,x1);
			// std::cout << std::abs(I1 - I2) << " " << x0 << " " << x1 << " " << I1 << " " << I2 << std::endl;
    		SECTION("calling integral_of_lerp is equivalent to result from numerical integration"){
				CHECK(I1 == Approx(I2).margin(0.01));
			}	
		}	
	}
}