
// std libraries
#include <iostream>
#include <limits>

// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec2.hpp>     // *vec3
#include <glm/vec3.hpp>     // *vec3

// in-house libraries

#include <math/glm/special.hpp>
#include <math/special.hpp>

#include "whole_specialization.hpp"
#include "each.hpp"

#include <index/each.hpp>  
#include <index/whole.hpp>  
#include <index/series/Range.hpp>
#include <index/series/Get.hpp>
#include <index/series/glm/VectorInterleave.hpp>
#include <index/series/noise/UnitIntervalNoise.hpp>
#include <index/series/noise/glm/UnitVectorNoise.hpp>

#include <test/properties.hpp>  
#include <test/macros.hpp>  

#include "../each_test.cpp"



TEST_CASE( "vector series discernability", "[each]" ) {
    each::Adapter<double> adapter (1e-5);

    auto vectors = std::vector{
        series::unit_vector_noise<3>(10.0, 1e4),
        series::unit_vector_noise<3>(20.0, 2e4),
        series::unit_vector_noise<3>(30.0, 3e4)
    };

    REQUIRE(test::discernability(adapter, vectors));

}



TEST_CASE( "each::dot properties", "[each]" ) {
    each::Adapter<double> adapter (1e-5);

    auto vectors = std::vector{
        series::unit_vector_noise<3>(10.0, 1e4),
        series::unit_vector_noise<3>(20.0, 2e4),
        series::unit_vector_noise<3>(30.0, 3e4)
    };

    auto uniforms = std::vector{
        series::uniform(glm::dvec3(0.0,0.0,0.0)),
        series::uniform(glm::dvec3(2.0,2.0,2.0)),
        series::uniform(glm::dvec3(-2.0,-2.0,-2.0))
    };

    REQUIRE(test::determinism(adapter,
        "each::dot", EACH_TEST_BINARY_OUT_PARAMETER(double,30,each::dot),
        vectors,vectors
    ));

    REQUIRE(test::commutativity(adapter,
        "each::dot", EACH_TEST_BINARY_OUT_PARAMETER(double,30,each::dot),
        vectors,vectors
    ));

    // REQUIRE(test::binary_distributivity(adapter,
    //     "each::dot", EACH_TEST_BINARY_OUT_PARAMETER(double,30,each::dot),
    //     "each::add", EACH_TEST_BINARY_OUT_PARAMETER(double,30,each::add),
    //     vectors,vectors
    // ));

    // REQUIRE(test::binary_idempotence(adapter,
    //     "1",         series::uniform(1.0),
    //     "each::dot", EACH_TEST_BINARY_OUT_PARAMETER(double,30,each::dot),
    //     vectors,vectors
    // ));

    REQUIRE(test::determinism(adapter,
        "each::dot", EACH_TEST_BINARY_OUT_PARAMETER(double,30,each::dot),
        vectors,uniforms
    ));

    REQUIRE(test::commutativity(adapter,
        "each::dot", EACH_TEST_BINARY_OUT_PARAMETER(double,30,each::dot),
        vectors,uniforms
    ));

    // REQUIRE(test::binary_distributivity(adapter,
    //     "each::dot", EACH_TEST_BINARY_OUT_PARAMETER(double,30,each::dot),
    //     "each::add", EACH_TEST_BINARY_OUT_PARAMETER(double,30,each::add),
    //     vectors,uniforms
    // ));

    REQUIRE(test::determinism(adapter,
        "each::dot", EACH_TEST_BINARY_OUT_PARAMETER(double,30,each::dot),
        uniforms,vectors
    ));

    REQUIRE(test::commutativity(adapter,
        "each::dot", EACH_TEST_BINARY_OUT_PARAMETER(double,30,each::dot),
        uniforms,vectors
    ));

    // REQUIRE(test::binary_distributivity(adapter,
    //     "each::dot", EACH_TEST_BINARY_OUT_PARAMETER(double,30,each::dot),
    //     "each::add", EACH_TEST_BINARY_OUT_PARAMETER(double,30,each::add),
    //     uniforms,vectors
    // ));

}



TEST_CASE( "each::cross properties", "[each]" ) {
    each::Adapter<double> adapter (1e-5);

    auto vectors = std::vector{
        series::unit_vector_noise<3>(10.0, 1e4),
        series::unit_vector_noise<3>(20.0, 2e4),
        series::unit_vector_noise<3>(30.0, 3e4)
    };

    REQUIRE(test::determinism(adapter,
        "each::cross", EACH_TEST_BINARY_OUT_PARAMETER(glm::dvec3,30,each::cross),
        vectors,vectors
    ));

    REQUIRE(test::anticommutativity(adapter,
        "each::cross",  EACH_TEST_BINARY_OUT_PARAMETER(glm::dvec3,30,each::cross),
        "each::negate", EACH_TEST_UNARY_OUT_PARAMETER (glm::dvec3,30,each::negate),
        vectors,vectors
    ));

    // REQUIRE(test::nilpotence(adapter,
    //     "0",           series::uniform(0.0),
    //     "each::cross", EACH_TEST_BINARY_OUT_PARAMETER(glm::dvec3,30,each::cross),
    //     vectors,vectors
    // ));

}


TEST_CASE( "Series<T> cross() distributivity", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(1.0,1e4);
    auto b = series::unit_vector_noise<3>(2.0,2e4);
    auto c = series::unit_vector_noise<3>(3.0,3e4);
    auto ab = std::vector<glm::dvec3>(30);
    auto ac = std::vector<glm::dvec3>(30);
    auto bc = std::vector<glm::dvec3>(30);
    auto ab_c = std::vector<glm::dvec3>(30);
    auto ac_bc = std::vector<glm::dvec3>(30);

    SECTION("a+b must equal b+a"){
        each::add(a, b, ab);
        each::cross(ab, c, ab_c);
        each::cross(a, c, ac);
        each::cross(b, c, bc);
        each::add(ac, bc, ac_bc);
        CHECK(adapter.equal(ab_c, ac_bc));
    }
}




















TEST_CASE( "vec3s abs purity", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(1.0,1.0e4);
    auto out1 = std::vector<glm::dvec3>(30);
    auto out2 = std::vector<glm::dvec3>(30);
    SECTION("abs(a) must generate the same output when called repeatedly"){
        each::abs(a, out1);
        each::abs(a, out2);
        CHECK(adapter.equal(out1, out2));
    }
}
TEST_CASE( "vec3s sign purity", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(1.3,1.3e4);
    auto out1 = std::vector<glm::dvec3>(30);
    auto out2 = std::vector<glm::dvec3>(30);
    SECTION("sign(a) must generate the same output when called repeatedly"){
        each::sign(a, out1);
        each::sign(a, out2);
        CHECK(adapter.equal(out1, out2));
    }
}
TEST_CASE( "vec3s abs/sign consistency", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(1.6,1.6e4);
    auto abs_a = std::vector<glm::dvec3>(30);
    auto sign_a = std::vector<glm::dvec3>(30);
    auto sign_a_abs_a = std::vector<glm::dvec3>(30);
    SECTION("sign(a)*abs(a) must reconstruct a"){
        each::abs(a, abs_a);
        each::sign(a, sign_a);
        each::mult(sign_a, abs_a, sign_a_abs_a);
        CHECK(adapter.equal(sign_a_abs_a, a));
    }
}
TEST_CASE( "vec3s floor purity", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(2.0,2.0e4);
    auto out1 = std::vector<glm::dvec3>(30);
    auto out2 = std::vector<glm::dvec3>(30);
    SECTION("floor(a) must generate the same output when called repeatedly"){
        each::floor(a, out1);
        each::floor(a, out2);
        CHECK(adapter.equal(out1, out2));
    }
}
TEST_CASE( "vec3s fract purity", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(2.3,2.3e4);
    auto out1 = std::vector<glm::dvec3>(30);
    auto out2 = std::vector<glm::dvec3>(30);
    SECTION("fract(a) must generate the same output when called repeatedly"){
        each::fract(a, out1);
        each::fract(a, out2);
        CHECK(adapter.equal(out1, out2));
    }
}
TEST_CASE( "vec3s floor/fract consistency", "[each]" ) {
    each::Adapter<double> adapter (1e-7);
    auto a = series::unit_vector_noise<3>(2.6,2.6e4);
    auto floor_a = std::vector<glm::dvec3>(30);
    auto fract_a = std::vector<glm::dvec3>(30);
    auto floor_a_fract_a = std::vector<glm::dvec3>(30);
    SECTION("fract(a)+floor(a) must reconstruct a"){
        each::floor(a, floor_a);
        each::fract(a, fract_a);
        each::add(floor_a, fract_a, floor_a_fract_a);
        CHECK(adapter.equal(floor_a_fract_a, a));
    }
}

// TEST_CASE( "vec3s mod purity", "[each]" ) {
//     each::Adapter<double> adapter (1e-5);
//  auto a = series::unit_vector_noise<3>(3.0,3.0e4);
//  auto b = series::unit_vector_noise<3>(3.1,3.1e4);
//  auto out1 = std::vector<glm::dvec3>(30);
//  auto out2 = std::vector<glm::dvec3>(303.3,3.3e4);
//     SECTION("each::mod(a,b) must generate the same output when called repeatedly"){
//         each::mod(a,b, out1);
//         each::mod(a,b, out2);
//         CHECK(adapter.equal(out1, out2));
//     }
// }
// TEST_CASE( "vec3s mod/fract consistency", "[each]" ) {
//     each::Adapter<double> adapter (1e-5);
//  auto a = series::unit_vector_noise<3>(3.4,3.4e4);
//  auto ones = series::unit_vector_noise<3>(3.5,3.5e4);
//  auto out1 = std::vector<glm::dvec3>(30);
//  auto out2 = std::vector<glm::dvec3>(303.7,3.7e4);
//     SECTION("each::mod(a,1) must generate the same output as fract(a) for positive numbers"){
//         each::mod(a,1, out1);
//         each::fract(a, out2);
//         CHECK(adapter.equal(out1, out2));
//     }
//     SECTION("each::mod(a,ones) must generate the same output as fract(a) for positive numbers"){
//         each::mod(a,ones, out1);
//         each::fract(a, out2);
//         CHECK(adapter.equal(out1, out2));
//     }
// }
TEST_CASE( "vec3s min purity", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(3.8,3.8e4);
    auto b = series::unit_vector_noise<3>(3.9,3.9e4);
    auto out1 = std::vector<glm::dvec3>(30);
    auto out2 = std::vector<glm::dvec3>(30);
    SECTION("each::min(a,b) must generate the same output when called repeatedly"){
        each::min(a,b, out1);
        each::min(a,b, out2);
        CHECK(adapter.equal(out1, out2));
    }
}
// TEST_CASE( "vec3s min decreasing", "[each]" ) {
//     each::Adapter<double> adapter (1e-5);
//  auto a = series::unit_vector_noise<3>(4.2,4.2e4);
//  auto b = series::unit_vector_noise<3>(4.3,4.3e4);
//  auto c = series::unit_vector_noise<3>(4.4,4.4e4);
//  auto ab = series::unit_vector_noise<3>(4.5,4.5e4);
//  auto abc = series::unit_vector_noise<3>(4.6,4.6e4);
//     SECTION("sum(each::min(a,b)) must always return a value less than or equal to the starting value"){
//         each::min(a,b, ab);
//         min(ab,c, abc);
//         CHECK(sum(ab) >= sum(abc));
//     }
// }
TEST_CASE( "vec3s min idempotence", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(4.7,4.7e4);
    auto b = series::unit_vector_noise<3>(4.8,4.8e4);
    auto ab = std::vector<glm::dvec3>(30);
    auto abb = std::vector<glm::dvec3>(30);
    SECTION("subsequent calls to min(*,b) must return the same value"){
        each::min(a,b, ab);
        each::min(ab,b, abb);
        CHECK(adapter.equal(ab, abb));
    }
}

TEST_CASE( "vec3s max purity", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(5.1,5.1e4);
    auto b = series::unit_vector_noise<3>(5.2,5.2e4);
    auto out1 = std::vector<glm::dvec3>(30);
    auto out2 = std::vector<glm::dvec3>(30);
    SECTION("each::max(a,b) must generate the same output when called repeatedly"){
        each::max(a,b, out1);
        each::max(a,b, out2);
        CHECK(adapter.equal(out1, out2));
    }
}
// TEST_CASE( "vec3s max decreasing", "[each]" ) {
//     each::Adapter<double> adapter (1e-5);
//  auto a = series::unit_vector_noise<3>(5.5,5.5e4);
//  auto b = series::unit_vector_noise<3>(5.6,5.6e4);
//  auto c = series::unit_vector_noise<3>(5.7,5.7e4);
//  auto ab = series::unit_vector_noise<3>(5.8,5.8e4);
//  auto abc = series::unit_vector_noise<3>(5.9,5.9e4);
//     SECTION("sum(each::max(a,b)) must always return a value greater than or equal to the starting value"){
//         each::max(a,b, ab);
//         each::max(ab,c, abc);
//         CHECK(sum(ab) <= sum(abc));
//     }
// }
TEST_CASE( "vec3s max idempotence", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(6.0,6.0e4);
    auto b = series::unit_vector_noise<3>(6.1,6.1e4);
    auto ab = std::vector<glm::dvec3>(30);
    auto abb = std::vector<glm::dvec3>(30);
    SECTION("subsequent calls to max(*,b) must return the same value"){
        each::max(a,b, ab);
        each::max(ab,b, abb);
        CHECK(adapter.equal(ab, abb));
    }
}

// TEST_CASE( "vec3s min/max/clamp consistency", "[each]" ) {
//     each::Adapter<double> adapter (1e-5);
//  auto a = series::unit_vector_noise<3>(6.4,6.4e4);
//  auto lo = series::unit_vector_noise<3>(6.5,6.5e4);
//  auto hi = series::unit_vector_noise<3>(6.6,6.6e4);
//  auto alo = series::unit_vector_noise<3>(6.7,6.7e4);
//  auto alohi1 = series::unit_vector_noise<3>(6.8,6.8e4);
//  auto alohi2 = series::unit_vector_noise<3>(6.9,6.9e4);
//     SECTION("min(each::max(a,lo),hi) must return the same value as each::clamp(a,lo,hi)"){
//         each::max(a,lo, alo);
//         min(alo,hi, alohi1);
//         each::clamp(a,lo,hi, alohi2);
//         CHECK(alohi1 == alohi2);
//     }
// }










TEST_CASE( "vector series sqrt purity", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::vector_interleave<3>(series::unit_interval_noise(10.0,1.0e4));
    auto c1 = std::vector<glm::dvec3>(30);
    auto c2 = std::vector<glm::dvec3>(30);
    SECTION("sqrt(a) must be called repeatedly without changing the output"){
        each::sqrt(a,c1);
        each::sqrt(a,c2);
        CHECK(adapter.equal(c1,c2));
    }
}

// TEST_CASE( "vector series sqrt consistency", "[each]" ) {
//     each::Adapter<double> adapter (1e-5);
//  auto a     = series::unit_vector_noise<3>(13.0,1.3e4);
//  auto b     = series::unit_vector_noise<3>(14.0,1.4e4);
//  auto sqrt1 = std::vector<glm::dvec3>(30);
//  auto sqrt2 = std::vector<glm::dvec3>(30);
//  auto log1_ = std::vector<glm::dvec3>(30);
//  auto log2_ = std::vector<glm::dvec3>(30);

//     SECTION("sqrt(a) must equal pow(a,1/2)"){
//         each::sqrt(a,    sqrt1);
//         each::pow (a,0.5f,sqrt2);
//         CHECK(adapter.equal(sqrt1 , sqrt2));
//     }

// }

TEST_CASE( "vector series log consistency", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a     = series::vector_interleave<3>(series::unit_interval_noise(10.0,1.0e4));
    auto log1_ = std::vector<glm::dvec3>(30);
    auto log2_ = std::vector<glm::dvec3>(30);

    SECTION("log2(a) must equal log(a)/log(2)"){
        each::log2(a, log1_);
        each::log (a, log2_);
        each::div (log2_, series::uniform(std::log(2.0)), log2_);
        CHECK(adapter.equal(log1_ , log2_));
    }
}

TEST_CASE( "vector series log/exp properties", "[each]" ) {
    each::Adapter<double> adapter (1e-5);

    auto positive_vectors = std::vector{
        series::vector_interleave<3>(series::unit_interval_noise(10.0,1.0e4)),
        series::vector_interleave<3>(series::unit_interval_noise(11.0,1.1e4)),
        series::vector_interleave<3>(series::unit_interval_noise(12.0,1.2e4))
    };

    REQUIRE(test::determinism(adapter,
        "each::log",  EACH_TEST_UNARY_OUT_PARAMETER (glm::dvec3,30,each::log),
        positive_vectors
    ));

    REQUIRE(test::determinism(adapter,
        "each::exp",  EACH_TEST_UNARY_OUT_PARAMETER(glm::dvec3,30,each::exp),
        positive_vectors
    ));

    REQUIRE(test::unary_invertibility(adapter,
        "each::log",  EACH_TEST_UNARY_OUT_PARAMETER (glm::dvec3,30,each::log),
        "each::exp",  EACH_TEST_UNARY_OUT_PARAMETER(glm::dvec3,30,each::exp),
        positive_vectors
    ));
}

TEST_CASE( "vector series log/exp consistency", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a     = series::vector_interleave<3>(series::unit_interval_noise(10.0,1.0e4));
    auto b     = series::vector_interleave<3>(series::unit_interval_noise(10.0,1.0e4));
    auto ab    = std::vector<glm::dvec3>(30);
    auto loga  = std::vector<glm::dvec3>(30);
    auto logb  = std::vector<glm::dvec3>(30);
    auto loga_logb = std::vector<glm::dvec3>(30);
    auto out   = std::vector<glm::dvec3>(30);

    SECTION("exp(log(a)+log(b)) must equal a*b"){
        each::log(a, loga);
        each::log(b, logb);
        each::add(loga, logb, loga_logb);
        each::exp(loga_logb, out);
        each::mult(a, b, ab);
        CHECK(adapter.equal(out, ab));
    }
}

TEST_CASE( "vector series log2/exp2 properties", "[each]" ) {
    each::Adapter<double> adapter (1e-5);

    auto positive_vectors = std::vector{
        series::vector_interleave<3>(series::unit_interval_noise(10.0,1.0e4)),
        series::vector_interleave<3>(series::unit_interval_noise(11.0,1.1e4)),
        series::vector_interleave<3>(series::unit_interval_noise(12.0,1.2e4))
    };

    REQUIRE(test::determinism(adapter,
        "each::log2",  EACH_TEST_UNARY_OUT_PARAMETER (glm::dvec3,30,each::log2),
        positive_vectors
    ));

    REQUIRE(test::determinism(adapter,
        "each::exp2",  EACH_TEST_UNARY_OUT_PARAMETER(glm::dvec3,30,each::exp2),
        positive_vectors
    ));

    REQUIRE(test::invertibility(adapter,
        "each::log2",  EACH_TEST_UNARY_OUT_PARAMETER (glm::dvec3,30,each::log2),
        "each::exp2",  EACH_TEST_UNARY_OUT_PARAMETER(glm::dvec3,30,each::exp2),
        positive_vectors
    ));
}

TEST_CASE( "vector series log2/exp2 consistency", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a     = series::vector_interleave<3>(series::unit_interval_noise(10.0,1.0e4));
    auto b     = series::vector_interleave<3>(series::unit_interval_noise(10.0,1.0e4));
    auto ab    = std::vector<glm::dvec3>(30);
    auto loga  = std::vector<glm::dvec3>(30);
    auto logb  = std::vector<glm::dvec3>(30);
    auto loga_logb = std::vector<glm::dvec3>(30);
    auto out   = std::vector<glm::dvec3>(30);

    SECTION("exp2(log2(a)+log2(b)) must equal a*b"){
        each::log2(a, loga);
        each::log2(b, logb);
        each::add(loga, logb, loga_logb);
        each::exp2(loga_logb, out);
        each::mult(a, b, ab);
        CHECK(adapter.equal(out, ab));
    }
}

























TEST_CASE( "Series<T> distance() purity", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(7.0,7.0e4);
    auto b = series::unit_vector_noise<3>(7.1,7.1e4);
    auto c1 = std::vector<double>(30);
    auto c2 = std::vector<double>(30);

    SECTION("distance() must be called repeatedly without changing the output"){
        each::distance(a, b, c1);
        each::distance(a, b, c2);
        CHECK(adapter.equal(c1, c2));
    }
}

// TEST_CASE( "Series<T> distance() identity", "[each]" ) {
//     each::Adapter<double> adapter (1e-5);
//     auto a = series::unit_vector_noise<3>(7.2,7.2e4);
//     auto b = series::unit_vector_noise<3>(7.3,7.3e4);
//     auto ab = series::unit_vector_noise<3>(7.4,7.4e4);
//     auto ba = series::unit_vector_noise<3>(7.5,7.5e4);
//
//     SECTION("a+I must equal a"){
//         CHECK(adapter.equal(, 1e-5a+zeros, a));
//         CHECK(adapter.equal(, 1e-5a-zeros, a));
//         CHECK(adapter.equal(, 1e-5a*ones , a));
//         CHECK(adapter.equal(, 1e-5a/ones , a));
//         CHECK(adapter.equal(, 1e-5a-a, zeros));
//         CHECK(adapter.equal(, 1e-5a/a, ones ));
//     }
// }

TEST_CASE( "Series<T> distance() commutativity", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(7.6,7.6e4);
    auto b = series::unit_vector_noise<3>(7.7,7.7e4);
    auto ab = std::vector<double>(30);
    auto ba = std::vector<double>(30);

    SECTION("distance(a,b) must equal distance(b,a)"){
        each::distance(a, b, ab);
        each::distance(a, b, ba);
        CHECK(adapter.equal(ab, ba));
    }
}

// TEST_CASE( "Series<T> distance() associativity", "[each]" ) {
//     each::Adapter<double> adapter (1e-5);
//     auto a = series::unit_vector_noise<3>(7.8,7.8e4);
//     auto b = series::unit_vector_noise<3>(7.9,7.9e4);
//     auto c = series::interleave({0.0f,1.0f,2.0f,3.0f,4.0f});
//     auto bc = series::unit_vector_noise<3>(8.0,8.0e4);
//     auto ab_c = std::vector<double>(30);
//     auto a_bc = std::vector<double>(30);

//     SECTION("distance(distance(a,b),c) must equal distance(a,distance(b,c))"){
//         each::distance(a, b, ab_c);
//         each::mult(ab_c, c, ab_c);
//         each::mult(b, c, bc);
//         each::distance(a, bc, a_bc);
//         CHECK(adapter.equal(, 1e-5ab_c, a_bc));
//     }
// }

TEST_CASE( "Series<T> distance() translational invariance", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(8.1,8.1e4);
    auto b = series::unit_vector_noise<3>(8.2,8.2e4);
    auto c = series::unit_vector_noise<3>(8.3,8.3e4);
    auto ac = std::vector<glm::dvec3>(30);
    auto bc = std::vector<glm::dvec3>(30);
    auto ab = std::vector<double>(30);
    auto ac_bc = std::vector<double>(30);

    SECTION("a+b must equal b+a"){
        each::add(a, c, ac);
        each::add(b, c, bc);
        each::distance(ac, bc, ac_bc);
        each::distance(a, b, ab);
        CHECK(adapter.equal(ac_bc, ab));
    }
}






TEST_CASE( "Series<T>/singleton distance() purity", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(8.6,8.6e4);
    auto b = series::uniform(glm::dvec3(-1,0.5,2));
    auto c1 = std::vector<double>(30);
    auto c2 = std::vector<double>(30);

    SECTION("distance() must be called repeatedly without changing the output"){
        each::distance(a, b, c1);
        each::distance(a, b, c2);
        CHECK(adapter.equal(c1, c2));
    }
}

// TEST_CASE( "Series<T>/singleton distance() identity", "[each]" ) {
//     each::Adapter<double> adapter (1e-5);
//     auto a = series::unit_vector_noise<3>(8.7,8.7e4);
//     auto b = series::uniform(glm::dvec3(-1,0.5,2));
//     auto ab = series::unit_vector_noise<3>(8.8,8.8e4);
//     auto ba = series::unit_vector_noise<3>(8.9,8.9e4);
//
//     SECTION("a+I must equal a"){
//         CHECK(adapter.equal(, 1e-5each::add (a,zeros), a));
//         CHECK(adapter.equal(, 1e-5each::sub (a,zeros), a));
//         CHECK(adapter.equal(, 1e-5each::mult(a,ones ), a));
//         CHECK(adapter.equal(, 1e-5each::div (a,ones ), a));
//         CHECK(adapter.equal(, 1e-5each::sub (a,a), zeros));
//         CHECK(adapter.equal(, 1e-5each::div (a,a), ones ));
//     }
// }

TEST_CASE( "Series<T>/singleton distance() commutativity", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(9.0,9.0e4);
    auto b = series::uniform(glm::dvec3(-1,0.5,2));
    auto ab = std::vector<double>(30);
    auto ba = std::vector<double>(30);

    SECTION("distance(a,b) must equal distance(b,a)"){
        each::distance(a, b, ab);
        each::distance(a, b, ba);
        CHECK(adapter.equal(ab, ba));
    }
}

// TEST_CASE( "Series<T>/singleton distance() associativity", "[each]" ) {
//     each::Adapter<double> adapter (1e-5);
//     auto a = series::unit_vector_noise<3>(9.1,9.1e4);
//     auto b = series::uniform(glm::dvec3(-1,0.5,2));
//     auto c = series::interleave({0.0f,1.0f,2.0f,3.0f,4.0f});
//     auto bc = series::unit_vector_noise<3>(9.2,9.2e4);
//     auto ab_c = std::vector<double>(30);
//     auto a_bc = std::vector<double>(30);

//     SECTION("distance(distance(a,b),c) must equal distance(a,distance(b,c))"){
//         each::distance(a, b, ab_c);
//         each::mult(ab_c, c, ab_c);
//         each::mult(b, c, bc);
//         each::distance(a, bc, a_bc);
//         CHECK(adapter.equal(, 1e-5ab_c, a_bc));
//     }
// }

TEST_CASE( "Series<T>/singleton distance() translational invariance", "[each]" ) {
    each::Adapter<double> adapter (1e-5);
    auto a = series::unit_vector_noise<3>(9.3,9.3e4);
    auto b = series::uniform(glm::dvec3(-1,0.5,2));
    auto c = series::unit_vector_noise<3>(9.4,9.4e4);
    auto ac = std::vector<glm::dvec3>(30);
    auto bc = std::vector<glm::dvec3>(30);
    auto ab = std::vector<double>(30);
    auto ac_bc = std::vector<double>(30);

    SECTION("a+b must equal b+a"){
        each::add(a, c, ac);
        each::add(b, c, bc);
        each::distance(ac, bc, ac_bc);
        each::distance(a, b, ab);
        CHECK(adapter.equal(ac_bc, ab));
    }
}






// TEST_CASE( "Series<T> length() scale invariance", "[each]" ) {
//     each::Adapter<double> adapter (1e-5);
//     auto a = series::unit_vector_noise<3>(9.7,9.7e4);
//     auto a5 = series::unit_vector_noise<3>(9.8,9.8e4);
//     auto l_a5 = std::vector<glm::dvec3>(30);
//     auto la_5 = std::vector<glm::dvec3>(30);

//     SECTION("length(a*b) must equal length(a)*b for b>=0"){
//         each::mult(a, series::uniform(5.0f), a5);
//         each::length(a5, l_a5);
//         each::length(a, la_5);
//         each::mult(la_5, series::uniform(5.0f), la_5);
//         CHECK(adapter.equal(la_5, l_a5));
//     }
// }

