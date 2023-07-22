
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec3.hpp>               // *vec3
#include <glm/geometric.hpp>

#include "each.hpp" 

#include "../series/Interleave.hpp" 
#include "../series/Uniform.hpp"
#include "../whole.hpp" 
#include "_test_utils.cpp"


TEST_CASE( "Must be able to test series::interleave equivalence for boolean vectors using the catch framework", "[each]" ) {
    std::mt19937 generator(2);
    auto ref       = each::get_random_vec3s(4, generator);
    auto ref_copy  = series::interleave(ref);
    auto ref_tweak = series::interleave(ref); ref_tweak[1].z = 1.0f;
    auto ref_fewer = each::get_random_vec3s(3, generator); ref_fewer[0]=ref[0]; ref_fewer[1]=ref[1]; ref_fewer[2]=ref[2];

    SECTION("Must be able to equate object with itself"){
        CHECK(whole::equal(ref, ref));
    }
    SECTION("Must be able to equate Series of the same content and size"){
        CHECK(whole::equal(ref, ref_copy));
    }
    SECTION("Must be able to distinguish Series of slightly different content"){
        CHECK(whole::notEqual(ref, ref_tweak));
    }
    SECTION("Must be able to distinguish Series of slightly different size"){
        CHECK(whole::notEqual(ref, ref_fewer));
    }
}

TEST_CASE( "Must be able to test series::interleave equivalence for vectors using the catch framework", "[each]" ) {
    std::mt19937 generator(2);
    auto ref       = each::get_random_bvec3s(4, generator);
    auto ref_tweak = series::interleave(ref); ref_tweak[1].z = true; ref[1].z = false;
    auto ref_copy  = series::interleave(ref);
    auto ref_fewer = each::get_random_bvec3s(3, generator); ref_fewer[0]=ref[0]; ref_fewer[1]=ref[1]; ref_fewer[2]=ref[2];

    SECTION("Must be able to equate object with itself"){
        CHECK(whole::equal(ref, ref));
    }
    SECTION("Must be able to equate Series of the same content and size"){
        CHECK(whole::equal(ref, ref_copy));
    }
    SECTION("Must be able to distinguish Series of slightly different content"){
        CHECK(whole::notEqual(ref, ref_tweak));
    }
    SECTION("Must be able to distinguish Series of slightly different size"){
        CHECK(whole::notEqual(ref, ref_fewer));
    }
}



TEST_CASE( "Series<vec3> comparator purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto c1 = each::get_random_bvec3s(5, generator);
    auto c2 = each::get_random_bvec3s(5, generator);
    SECTION("greaterThanEqual(a,b) must be called repeatedly without changing the output"){
        each::greaterThanEqual(a,b,c1);
        each::greaterThanEqual(a,b,c2);
        CHECK(whole::equal(c1,c2));
    }
    SECTION("greaterThan(a,b) must be called repeatedly without changing the output"){
        each::greaterThan(a,b,c1);
        each::greaterThan(a,b,c2);
        CHECK(whole::equal(c1,c2));
    }
    SECTION("lessThanEqual(a,b) must be called repeatedly without changing the output"){
        each::lessThanEqual(a,b,c1);
        each::lessThanEqual(a,b,c2);
        CHECK(whole::equal(c1,c2));
    }
    SECTION("lessThan(a,b) must be called repeatedly without changing the output"){
        each::lessThan(a,b,c1);
        each::lessThan(a,b,c2);
        CHECK(whole::equal(c1,c2));
    }
    SECTION("equal(a,b) must be called repeatedly without changing the output"){
        each::equal(a,b,c1);
        each::equal(a,b,c2);
        CHECK(whole::equal(c1,c2));
    }
}
TEST_CASE( "Series<vec3> comparator consistency", "[each]" ) {
    std::mt19937 generator(2);
    auto a    = each::get_random_vec3s(5, generator);
    auto b    = each::get_random_vec3s(5, generator);
    auto gt   = each::get_random_bvec3s(5, generator);
    auto lt   = each::get_random_bvec3s(5, generator);
    auto eq   = each::get_random_bvec3s(5, generator);
    auto ne1  = each::get_random_bvec3s(5, generator);
    auto ne2  = each::get_random_bvec3s(5, generator);
    auto gte1 = each::get_random_bvec3s(5, generator);
    auto gte2 = each::get_random_bvec3s(5, generator);
    auto lte1 = each::get_random_bvec3s(5, generator);
    auto lte2 = each::get_random_bvec3s(5, generator);

    SECTION("a>=b must equal a>b || a==b"){
        each::greaterThanEqual(a,b,   gte1);
        each::greaterThan     (a,b,   gt);
        each::equal           (a,b,   eq);
        each::unite           (gt,eq, gte2);
        CHECK(whole::equal(gte1, gte2));
    }

    SECTION("a<=b must equal a<b || a==b"){
        each::greaterThanEqual(a,b,   lte1);
        each::greaterThan     (a,b,   lt);
        each::equal           (a,b,   eq);
        each::unite           (lt,eq, lte2);
        CHECK(whole::equal(lte1, lte2));
    }

    // NOTE: bvec3 does not have an overload for ! 
    // SECTION("a!=b must equal !(a==b)"){
    //     each::equal           (a,b, eq);
    //     each::negate          (eq,  ne1);
    //     each::notEqual        (a,b, ne2);
    //     CHECK(whole::equal(ne1, ne2));
    // }
}









TEST_CASE( "vec3s abs purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto out1 = each::get_random_vec3s(5, generator);
    auto out2 = each::get_random_vec3s(5, generator);
    SECTION("abs(a) must generate the same output when called repeatedly"){
        each::abs(a, out1);
        each::abs(a, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "vec3s sign purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto out1 = each::get_random_vec3s(5, generator);
    auto out2 = each::get_random_vec3s(5, generator);
    SECTION("sign(a) must generate the same output when called repeatedly"){
        each::sign(a, out1);
        each::sign(a, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "vec3s abs/sign consistency", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto abs_a = each::get_random_vec3s(5, generator);
    auto sign_a = each::get_random_vec3s(5, generator);
    auto sign_a_abs_a = each::get_random_vec3s(5, generator);
    SECTION("sign(a)*abs(a) must reconstruct a"){
        each::abs(a, abs_a);
        each::sign(a, sign_a);
        each::mult(sign_a, abs_a, sign_a_abs_a);
        CHECK(whole::equal(sign_a_abs_a, a));
    }
}
TEST_CASE( "vec3s floor purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto out1 = each::get_random_vec3s(5, generator);
    auto out2 = each::get_random_vec3s(5, generator);
    SECTION("floor(a) must generate the same output when called repeatedly"){
        each::floor(a, out1);
        each::floor(a, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "vec3s fract purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto out1 = each::get_random_vec3s(5, generator);
    auto out2 = each::get_random_vec3s(5, generator);
    SECTION("fract(a) must generate the same output when called repeatedly"){
        each::fract(a, out1);
        each::fract(a, out2);
        CHECK(whole::equal(out1, out2));
    }
}
TEST_CASE( "vec3s floor/fract consistency", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto floor_a = each::get_random_vec3s(5, generator);
    auto fract_a = each::get_random_vec3s(5, generator);
    auto floor_a_fract_a = each::get_random_vec3s(5, generator);
    SECTION("fract(a)+floor(a) must reconstruct a"){
        each::floor(a, floor_a);
        each::fract(a, fract_a);
        each::add(floor_a, fract_a, floor_a_fract_a);
        CHECK(whole::equal(floor_a_fract_a, a, 1e-7));
    }
}

// TEST_CASE( "vec3s mod purity", "[each]" ) {
//     std::mt19937 generator(2);
//  auto a = each::get_random_vec3s(5, generator);
//  auto b = each::get_random_vec3s(5, generator);
//  auto out1 = each::get_random_vec3s(5, generator);
//  auto out2 = each::get_random_vec3s(5, generator);
//     SECTION("each::mod(a,b) must generate the same output when called repeatedly"){
//         each::mod(a,b, out1);
//         each::mod(a,b, out2);
//         CHECK(whole::equal(out1, out2));
//     }
// }
// TEST_CASE( "vec3s mod/fract consistency", "[each]" ) {
//     std::mt19937 generator(2);
//  auto a = each::get_random_vec3s(5, generator);
//  auto ones = each::get_random_vec3s(5, generator);
//  auto out1 = each::get_random_vec3s(5, generator);
//  auto out2 = each::get_random_vec3s(5, generator);
//     SECTION("each::mod(a,1) must generate the same output as fract(a) for positive numbers"){
//         each::mod(a,1, out1);
//         each::fract(a, out2);
//         CHECK(whole::equal(out1, out2));
//     }
//     SECTION("each::mod(a,ones) must generate the same output as fract(a) for positive numbers"){
//         each::mod(a,ones, out1);
//         each::fract(a, out2);
//         CHECK(whole::equal(out1, out2));
//     }
// }
TEST_CASE( "vec3s min purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto out1 = each::get_random_vec3s(5, generator);
    auto out2 = each::get_random_vec3s(5, generator);
    SECTION("each::min(a,b) must generate the same output when called repeatedly"){
        each::min(a,b, out1);
        each::min(a,b, out2);
        CHECK(whole::equal(out1, out2));
    }
}
// TEST_CASE( "vec3s min decreasing", "[each]" ) {
//     std::mt19937 generator(2);
//  auto a = each::get_random_vec3s(5, generator);
//  auto b = each::get_random_vec3s(5, generator);
//  auto c = each::get_random_vec3s(5, generator);
//  auto ab = each::get_random_vec3s(5, generator);
//  auto abc = each::get_random_vec3s(5, generator);
//     SECTION("sum(each::min(a,b)) must always return a value less than or equal to the starting value"){
//         each::min(a,b, ab);
//         min(ab,c, abc);
//         CHECK(sum(ab) >= sum(abc));
//     }
// }
TEST_CASE( "vec3s min idempotence", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto ab = each::get_random_vec3s(5, generator);
    auto abb = each::get_random_vec3s(5, generator);
    SECTION("subsequent calls to min(*,b) must return the same value"){
        each::min(a,b, ab);
        each::min(ab,b, abb);
        CHECK(whole::equal(ab, abb));
    }
}

TEST_CASE( "vec3s max purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto out1 = each::get_random_vec3s(5, generator);
    auto out2 = each::get_random_vec3s(5, generator);
    SECTION("each::max(a,b) must generate the same output when called repeatedly"){
        each::max(a,b, out1);
        each::max(a,b, out2);
        CHECK(whole::equal(out1, out2));
    }
}
// TEST_CASE( "vec3s max decreasing", "[each]" ) {
//     std::mt19937 generator(2);
//  auto a = each::get_random_vec3s(5, generator);
//  auto b = each::get_random_vec3s(5, generator);
//  auto c = each::get_random_vec3s(5, generator);
//  auto ab = each::get_random_vec3s(5, generator);
//  auto abc = each::get_random_vec3s(5, generator);
//     SECTION("sum(each::max(a,b)) must always return a value greater than or equal to the starting value"){
//         each::max(a,b, ab);
//         each::max(ab,c, abc);
//         CHECK(sum(ab) <= sum(abc));
//     }
// }
TEST_CASE( "vec3s max idempotence", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto ab = each::get_random_vec3s(5, generator);
    auto abb = each::get_random_vec3s(5, generator);
    SECTION("subsequent calls to max(*,b) must return the same value"){
        each::max(a,b, ab);
        each::max(ab,b, abb);
        CHECK(whole::equal(ab, abb));
    }
}

// TEST_CASE( "vec3s min/max/clamp consistency", "[each]" ) {
//     std::mt19937 generator(2);
//  auto a = each::get_random_vec3s(5, generator);
//  auto lo = each::get_random_vec3s(5, generator);
//  auto hi = each::get_random_vec3s(5, generator);
//  auto alo = each::get_random_vec3s(5, generator);
//  auto alohi1 = each::get_random_vec3s(5, generator);
//  auto alohi2 = each::get_random_vec3s(5, generator);
//     SECTION("min(each::max(a,lo),hi) must return the same value as each::clamp(a,lo,hi)"){
//         each::max(a,lo, alo);
//         min(alo,hi, alohi1);
//         each::clamp(a,lo,hi, alohi2);
//         CHECK(alohi1 == alohi2);
//     }
// }










TEST_CASE( "Series<vec3> sqrt purity", "[each]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    auto a = each::get_random_vec3s(5, distribution, generator);
    auto c1 = each::get_random_vec3s(5, distribution, generator);
    auto c2 = each::get_random_vec3s(5, distribution, generator);
    SECTION("sqrt(a) must be called repeatedly without changing the output"){
        each::sqrt(a,c1);
        each::sqrt(a,c2);
        CHECK(whole::equal(c1,c2));
    }
}
// TEST_CASE( "Series<vec3> sqrt consistency", "[each]" ) {
//     std::mt19937 generator(2);
//     std::uniform_real_distribution<float> distribution(0.1,5.0);
//  auto a     = each::get_random_vec3s(5, distribution, generator);
//  auto b     = each::get_random_vec3s(5, distribution, generator);
//  auto sqrt1 = each::get_random_vec3s(5, distribution, generator);
//  auto sqrt2 = each::get_random_vec3s(5, distribution, generator);
//  auto log1_ = each::get_random_vec3s(5, distribution, generator);
//  auto log2_ = each::get_random_vec3s(5, distribution, generator);

//     SECTION("sqrt(a) must equal pow(a,1/2)"){
//         each::sqrt(a,    sqrt1);
//         each::pow (a,0.5f,sqrt2);
//         CHECK(whole::equal(sqrt1 , sqrt2));
//     }

// }
TEST_CASE( "Series<vec3> log consistency", "[each]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    auto a     = each::get_random_vec3s(5, distribution, generator);
    auto b     = each::get_random_vec3s(5, distribution, generator);
    auto log1_ = each::get_random_vec3s(5, distribution, generator);
    auto log2_ = each::get_random_vec3s(5, distribution, generator);

    SECTION("log2(a) must equal log(a)/log(2)"){
        each::log2(a, log1_);
        each::log (a, log2_);
        each::div (log2_, series::uniform(std::log(2.f)), log2_);
        CHECK(whole::equal(log1_ , log2_, 1e-5));
    }
}
TEST_CASE( "Series<vec3> log/exp consistency", "[each]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    auto a     = each::get_random_vec3s(5, distribution, generator);
    auto b     = each::get_random_vec3s(5, distribution, generator);
    auto ab    = each::get_random_vec3s(5, distribution, generator);
    auto loga  = each::get_random_vec3s(5, distribution, generator);
    auto logb  = each::get_random_vec3s(5, distribution, generator);
    auto loga_logb = each::get_random_vec3s(5, distribution, generator);
    auto out   = each::get_random_vec3s(5, distribution, generator);

    SECTION("exp(log(a)+log(b)) must equal a*b"){
        each::log(a, loga);
        each::log(b, logb);
        each::add(loga, logb, loga_logb);
        each::exp(loga_logb, out);
        each::mult(a, b, ab);
        CHECK(whole::equal(out, ab, 1e-5));
    }
}
TEST_CASE( "Series<vec3> log/exp invertibility", "[each]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    auto a   = each::get_random_vec3s(5, distribution, generator);
    auto b   = each::get_random_vec3s(5, distribution, generator);
    auto out = each::get_random_vec3s(5, distribution, generator);

    SECTION("log(exp(a)) must equal a"){
        each::log (a,   out);
        each::exp (out, out);
        CHECK(whole::equal(a, out, 1e-5));
    }
    SECTION("log2(exp2(a)) must equal a"){
        each::log2(a,   out);
        each::exp2(out, out);
        CHECK(whole::equal(a, out, 1e-5));
    }
}

TEST_CASE( "Series<vec3> log2/exp2 consistency", "[each]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    auto a     = each::get_random_vec3s(5, distribution, generator);
    auto b     = each::get_random_vec3s(5, distribution, generator);
    auto ab    = each::get_random_vec3s(5, distribution, generator);
    auto loga  = each::get_random_vec3s(5, distribution, generator);
    auto logb  = each::get_random_vec3s(5, distribution, generator);
    auto loga_logb = each::get_random_vec3s(5, distribution, generator);
    auto out   = each::get_random_vec3s(5, distribution, generator);

    SECTION("exp2(log2(a)+log2(b)) must equal a*b"){
        each::log2(a, loga);
        each::log2(b, logb);
        each::add(loga, logb, loga_logb);
        each::exp2(loga_logb, out);
        each::mult(a, b, ab);
        CHECK(whole::equal(out, ab, 1e-5));
    }
}
TEST_CASE( "Series<vec3> log2/exp2 invertibility", "[each]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    auto a   = each::get_random_vec3s(5, distribution, generator);
    auto b   = each::get_random_vec3s(5, distribution, generator);
    auto out = each::get_random_vec3s(5, distribution, generator);

    SECTION("log2(exp2(a)) must equal a"){
        each::log2 (a,   out);
        each::exp2 (out, out);
        CHECK(whole::equal(a, out, 1e-5));
    }
    SECTION("log2(exp2(a)) must equal a"){
        each::log2(a,   out);
        each::exp2(out, out);
        CHECK(whole::equal(a, out, 1e-5));
    }
}














TEST_CASE( "Series<T> dot() purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto c1 = series::interleave(5, 0.0f);
    auto c2 = series::interleave(5, 0.0f);

    SECTION("dot() must be called repeatedly without changing the output"){
        each::dot(a, b, c1);
        each::dot(a, b, c2);
        CHECK(whole::equal(c1, c2, 1e-5));
    }
}

// TEST_CASE( "Series<T> dot() identity", "[each]" ) {
//     std::mt19937 generator(2);
//     auto a = each::get_random_vec3s(5, generator);
//     auto b = each::get_random_vec3s(5, generator);
//     auto ab = each::get_random_vec3s(5, generator);
//     auto ba = each::get_random_vec3s(5, generator);
//
//     SECTION("a+I must equal a"){
//         CHECK(whole::equa, 1e-5l(a+zeros, a));
//         CHECK(whole::equa, 1e-5l(a-zeros, a));
//         CHECK(whole::equa, 1e-5l(a*ones , a));
//         CHECK(whole::equa, 1e-5l(a/ones , a));
//         CHECK(whole::equa, 1e-5l(a-a, zeros));
//         CHECK(whole::equa, 1e-5l(a/a, ones ));
//     }
// }

TEST_CASE( "Series<T> dot() commutativity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto ab = series::interleave(5, 0.0f);
    auto ba = series::interleave(5, 0.0f);

    SECTION("dot(a,b) must equal dot(b,a)"){
        each::dot(a, b, ab);
        each::dot(a, b, ba);
        CHECK(whole::equal(ab, ba, 1e-5));
    }
}

TEST_CASE( "Series<T> dot() associativity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto c = series::interleave({0.0f,1.0f,2.0f,3.0f,4.0f});
    auto bc = each::get_random_vec3s(5, generator);
    auto ab_c = series::interleave(5, 0.0f);
    auto a_bc = series::interleave(5, 0.0f);

    SECTION("dot(dot(a,b),c) must equal dot(a,dot(b,c))"){
        each::dot(a, b, ab_c);
        each::mult(ab_c, c, ab_c);
        each::mult(b, c, bc);
        each::dot(a, bc, a_bc);
        CHECK(whole::equal(ab_c, a_bc, 1e-5));
    }
}

TEST_CASE( "Series<T> dot() distributivity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto c = each::get_random_vec3s(5, generator);
    auto ab = each::get_random_vec3s(5, generator);
    auto ac = series::interleave(5, 0.0f);
    auto bc = series::interleave(5, 0.0f);
    auto ab_c = series::interleave(5, 0.0f);
    auto ac_bc = series::interleave(5, 0.0f);

    SECTION("a+b must equal b+a"){
        each::add(a, b, ab);
        each::dot(ab, c, ab_c);
        each::dot(a, c, ac);
        each::dot(b, c, bc);
        each::add(ac, bc, ac_bc);
        CHECK(whole::equal(ab_c, ac_bc, 1e-5));
    }
}






TEST_CASE( "Series<T>/singleton dot() purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = series::uniform(glm::vec3(-1,0.5,2));
    auto c1 = series::interleave(5, 0.0f);
    auto c2 = series::interleave(5, 0.0f);

    SECTION("dot() must be called repeatedly without changing the output"){
        each::dot(a, b, c1);
        each::dot(a, b, c2);
        CHECK(whole::equal(c1, c2, 1e-5));
    }
}

// TEST_CASE( "Series<T>/singleton dot() identity", "[each]" ) {
//     std::mt19937 generator(2);
//     auto a = each::get_random_vec3s(5, generator);
//     auto b = series::uniform(glm::vec3(-1,0.5,2));
//     auto ab = each::get_random_vec3s(5, generator);
//     auto ba = each::get_random_vec3s(5, generator);
//
//     SECTION("a+I must equal a"){
//         CHECK(whole::equa, 1e-5l(a+zeros, a));
//         CHECK(whole::equa, 1e-5l(a-zeros, a));
//         CHECK(whole::equa, 1e-5l(a*ones , a));
//         CHECK(whole::equa, 1e-5l(a/ones , a));
//         CHECK(whole::equa, 1e-5l(a-a, zeros));
//         CHECK(whole::equa, 1e-5l(a/a, ones ));
//     }
// }

TEST_CASE( "Series<T>/singleton dot() commutativity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = series::uniform(glm::vec3(-1,0.5,2));
    auto ab = series::interleave(5, 0.0f);
    auto ba = series::interleave(5, 0.0f);

    SECTION("dot(a,b) must equal dot(b,a)"){
        each::dot(a, b, ab);
        each::dot(a, b, ba);
        CHECK(whole::equal(ab, ba, 1e-5));
    }
}

TEST_CASE( "Series<T>/singleton dot() associativity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = series::uniform(glm::vec3(-1,0.5,2));
    auto c = series::interleave({0.0f,1.0f,2.0f,3.0f,4.0f});
    auto bc = each::get_random_vec3s(5, generator);
    auto ab_c = series::interleave(5, 0.0f);
    auto a_bc = series::interleave(5, 0.0f);

    SECTION("dot(dot(a,b),c) must equal dot(a,dot(b,c))"){
        each::dot(a, b, ab_c);
        each::mult(ab_c, c, ab_c);
        each::mult(b, c, bc);
        each::dot(a, bc, a_bc);
        CHECK(whole::equal(ab_c, a_bc, 1e-5));
    }
}

TEST_CASE( "Series<T>/singleton dot() distributivity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = series::uniform(glm::vec3(-1,0.5,2));
    auto c = each::get_random_vec3s(5, generator);
    auto ab = each::get_random_vec3s(5, generator);
    auto ac = series::interleave(5, 0.0f);
    auto bc = series::interleave(5, 0.0f);
    auto ab_c = series::interleave(5, 0.0f);
    auto ac_bc = series::interleave(5, 0.0f);

    SECTION("a+b must equal b+a"){
        each::add(a, b, ab);
        each::dot(ab, c, ab_c);
        each::dot(a, c, ac);
        each::dot(b, c, bc);
        each::add(ac, bc, ac_bc);
        CHECK(whole::equal(ab_c, ac_bc, 1e-5));
    }
}


















TEST_CASE( "Series<T> cross() purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto c1 = each::get_random_vec3s(5, generator);
    auto c2 = each::get_random_vec3s(5, generator);

    SECTION("cross() must be called repeatedly without changing the output"){
        each::cross(a, b, c1);
        each::cross(a, b, c2);
        CHECK(whole::equal(c1, c2, 1e-5));
    }
}

// TEST_CASE( "Series<T> cross() identity", "[each]" ) {
//     std::mt19937 generator(2);
//     auto a = each::get_random_vec3s(5, generator);
//     auto b = each::get_random_vec3s(5, generator);
//     auto ab = each::get_random_vec3s(5, generator);
//     auto ba = each::get_random_vec3s(5, generator);
//
//     SECTION("a+I must equal a"){
//         CHECK(whole::equa, 1e-5l(a+zeros, a));
//         CHECK(whole::equa, 1e-5l(a-zeros, a));
//         CHECK(whole::equa, 1e-5l(a*ones , a));
//         CHECK(whole::equa, 1e-5l(a/ones , a));
//         CHECK(whole::equa, 1e-5l(a-a, zeros));
//         CHECK(whole::equa, 1e-5l(a/a, ones ));
//     }
// }

TEST_CASE( "Series<T> cross() anticommutativity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto ab = each::get_random_vec3s(5, generator);
    auto ba = each::get_random_vec3s(5, generator);
    auto n_ba = each::get_random_vec3s(5, generator);

    SECTION("cross(a,b) must equal -cross(b,a)"){
        each::cross(a, b, ab);
        each::cross(b, a, ba);
        each::mult(ba, series::uniform(-1.0f), n_ba);
        CHECK(whole::equal(ab, n_ba, 1e-5));
    }
}


TEST_CASE( "Series<T> cross() distributivity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto c = each::get_random_vec3s(5, generator);
    auto ab = each::get_random_vec3s(5, generator);
    auto ac = each::get_random_vec3s(5, generator);
    auto bc = each::get_random_vec3s(5, generator);
    auto ab_c = each::get_random_vec3s(5, generator);
    auto ac_bc = each::get_random_vec3s(5, generator);

    SECTION("a+b must equal b+a"){
        each::add(a, b, ab);
        each::cross(ab, c, ab_c);
        each::cross(a, c, ac);
        each::cross(b, c, bc);
        each::add(ac, bc, ac_bc);
        CHECK(whole::equal(ab_c, ac_bc, 1e-5));
    }
}






TEST_CASE( "Series<T>/singleton cross() purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = series::uniform(glm::vec3(-1,0.5,2));
    auto c1 = each::get_random_vec3s(5, generator);
    auto c2 = each::get_random_vec3s(5, generator);

    SECTION("cross() must be called repeatedly without changing the output"){
        each::cross(a, b, c1);
        each::cross(a, b, c2);
        CHECK(whole::equal(c1, c2, 1e-5));
    }
}

// TEST_CASE( "Series<T>/singleton cross() identity", "[each]" ) {
//     std::mt19937 generator(2);
//     auto a = each::get_random_vec3s(5, generator);
//     auto b = series::uniform(glm::vec3(-1,0.5,2));
//     auto ab = each::get_random_vec3s(5, generator);
//     auto ba = each::get_random_vec3s(5, generator);
//
//     SECTION("a+I must equal a"){
//         CHECK(whole::equa, 1e-5l(a+zeros, a));
//         CHECK(whole::equa, 1e-5l(a-zeros, a));
//         CHECK(whole::equa, 1e-5l(a*ones , a));
//         CHECK(whole::equa, 1e-5l(a/ones , a));
//         CHECK(whole::equa, 1e-5l(a-a, zeros));
//         CHECK(whole::equa, 1e-5l(a/a, ones ));
//     }
// }

TEST_CASE( "Series<T>/singleton cross() anticommutativity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = series::uniform(glm::vec3(-1,0.5,2));
    auto ab = each::get_random_vec3s(5, generator);
    auto ba = each::get_random_vec3s(5, generator);
    auto n_ba = each::get_random_vec3s(5, generator);

    SECTION("cross(a,b) must equal -cross(b,a)"){
        each::cross(a, b, ab);
        each::cross(b, a, ba);
        each::mult(ba, series::uniform(-1.0f), n_ba);
        CHECK(whole::equal(ab, n_ba, 1e-5));
    }
}

TEST_CASE( "Series<T>/singleton cross() distributivity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = series::uniform(glm::vec3(-1,0.5,2));
    auto c = each::get_random_vec3s(5, generator);
    auto ab = each::get_random_vec3s(5, generator);
    auto ac = each::get_random_vec3s(5, generator);
    auto bc = each::get_random_vec3s(5, generator);
    auto ab_c = each::get_random_vec3s(5, generator);
    auto ac_bc = each::get_random_vec3s(5, generator);

    SECTION("a+b must equal b+a"){
        each::add(a, b, ab);
        each::cross(ab, c, ab_c);
        each::cross(a, c, ac);
        each::cross(b, c, bc);
        each::add(ac, bc, ac_bc);
        CHECK(whole::equal(ab_c, ac_bc, 1e-5));
    }
}


























TEST_CASE( "Series<T> distance() purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto c1 = series::interleave(5, 0.0f);
    auto c2 = series::interleave(5, 0.0f);

    SECTION("distance() must be called repeatedly without changing the output"){
        each::distance(a, b, c1);
        each::distance(a, b, c2);
        CHECK(whole::equal(c1, c2, 1e-5));
    }
}

// TEST_CASE( "Series<T> distance() identity", "[each]" ) {
//     std::mt19937 generator(2);
//     auto a = each::get_random_vec3s(5, generator);
//     auto b = each::get_random_vec3s(5, generator);
//     auto ab = each::get_random_vec3s(5, generator);
//     auto ba = each::get_random_vec3s(5, generator);
//
//     SECTION("a+I must equal a"){
//         CHECK(whole::equal(, 1e-5a+zeros, a));
//         CHECK(whole::equal(, 1e-5a-zeros, a));
//         CHECK(whole::equal(, 1e-5a*ones , a));
//         CHECK(whole::equal(, 1e-5a/ones , a));
//         CHECK(whole::equal(, 1e-5a-a, zeros));
//         CHECK(whole::equal(, 1e-5a/a, ones ));
//     }
// }

TEST_CASE( "Series<T> distance() commutativity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto ab = series::interleave(5, 0.0f);
    auto ba = series::interleave(5, 0.0f);

    SECTION("distance(a,b) must equal distance(b,a)"){
        each::distance(a, b, ab);
        each::distance(a, b, ba);
        CHECK(whole::equal(ab, ba, 1e-5));
    }
}

// TEST_CASE( "Series<T> distance() associativity", "[each]" ) {
//     std::mt19937 generator(2);
//     auto a = each::get_random_vec3s(5, generator);
//     auto b = each::get_random_vec3s(5, generator);
//     auto c = series::interleave({0.0f,1.0f,2.0f,3.0f,4.0f});
//     auto bc = each::get_random_vec3s(5, generator);
//     auto ab_c = series::interleave(5, 0.0f);
//     auto a_bc = series::interleave(5, 0.0f);

//     SECTION("distance(distance(a,b),c) must equal distance(a,distance(b,c))"){
//         each::distance(a, b, ab_c);
//         each::mult(ab_c, c, ab_c);
//         each::mult(b, c, bc);
//         each::distance(a, bc, a_bc);
//         CHECK(whole::equal(, 1e-5ab_c, a_bc));
//     }
// }

TEST_CASE( "Series<T> distance() translational invariance", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = each::get_random_vec3s(5, generator);
    auto c = each::get_random_vec3s(5, generator);
    auto ac = each::get_random_vec3s(5, generator);
    auto bc = each::get_random_vec3s(5, generator);
    auto ab = series::interleave(5, 0.0f);
    auto ac_bc = series::interleave(5, 0.0f);

    SECTION("a+b must equal b+a"){
        each::add(a, c, ac);
        each::add(b, c, bc);
        each::distance(ac, bc, ac_bc);
        each::distance(a, b, ab);
        CHECK(whole::equal(ac_bc, ab, 1e-5));
    }
}






TEST_CASE( "Series<T>/singleton distance() purity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = series::uniform(glm::vec3(-1,0.5,2));
    auto c1 = series::interleave(5, 0.0f);
    auto c2 = series::interleave(5, 0.0f);

    SECTION("distance() must be called repeatedly without changing the output"){
        each::distance(a, b, c1);
        each::distance(a, b, c2);
        CHECK(whole::equal(c1, c2, 1e-5));
    }
}

// TEST_CASE( "Series<T>/singleton distance() identity", "[each]" ) {
//     std::mt19937 generator(2);
//     auto a = each::get_random_vec3s(5, generator);
//     auto b = series::uniform(glm::vec3(-1,0.5,2));
//     auto ab = each::get_random_vec3s(5, generator);
//     auto ba = each::get_random_vec3s(5, generator);
//
//     SECTION("a+I must equal a"){
//         CHECK(whole::equal(, 1e-5each::add (a,zeros), a));
//         CHECK(whole::equal(, 1e-5each::sub (a,zeros), a));
//         CHECK(whole::equal(, 1e-5each::mult(a,ones ), a));
//         CHECK(whole::equal(, 1e-5each::div (a,ones ), a));
//         CHECK(whole::equal(, 1e-5each::sub (a,a), zeros));
//         CHECK(whole::equal(, 1e-5each::div (a,a), ones ));
//     }
// }

TEST_CASE( "Series<T>/singleton distance() commutativity", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = series::uniform(glm::vec3(-1,0.5,2));
    auto ab = series::interleave(5, 0.0f);
    auto ba = series::interleave(5, 0.0f);

    SECTION("distance(a,b) must equal distance(b,a)"){
        each::distance(a, b, ab);
        each::distance(a, b, ba);
        CHECK(whole::equal(ab, ba, 1e-5));
    }
}

// TEST_CASE( "Series<T>/singleton distance() associativity", "[each]" ) {
//     std::mt19937 generator(2);
//     auto a = each::get_random_vec3s(5, generator);
//     auto b = series::uniform(glm::vec3(-1,0.5,2));
//     auto c = series::interleave({0.0f,1.0f,2.0f,3.0f,4.0f});
//     auto bc = each::get_random_vec3s(5, generator);
//     auto ab_c = series::interleave(5, 0.0f);
//     auto a_bc = series::interleave(5, 0.0f);

//     SECTION("distance(distance(a,b),c) must equal distance(a,distance(b,c))"){
//         each::distance(a, b, ab_c);
//         each::mult(ab_c, c, ab_c);
//         each::mult(b, c, bc);
//         each::distance(a, bc, a_bc);
//         CHECK(whole::equal(, 1e-5ab_c, a_bc));
//     }
// }

TEST_CASE( "Series<T>/singleton distance() translational invariance", "[each]" ) {
    std::mt19937 generator(2);
    auto a = each::get_random_vec3s(5, generator);
    auto b = series::uniform(glm::vec3(-1,0.5,2));
    auto c = each::get_random_vec3s(5, generator);
    auto ac = each::get_random_vec3s(5, generator);
    auto bc = each::get_random_vec3s(5, generator);
    auto ab = series::interleave(5, 0.0f);
    auto ac_bc = series::interleave(5, 0.0f);

    SECTION("a+b must equal b+a"){
        each::add(a, c, ac);
        each::add(b, c, bc);
        each::distance(ac, bc, ac_bc);
        each::distance(a, b, ab);
        CHECK(whole::equal(ac_bc, ab, 1e-5));
    }
}







// TEST_CASE( "Series<T> length() scale invariance", "[each]" ) {
//     std::mt19937 generator(2);
//     auto a = each::get_random_vec3s(5, generator);
//     auto a5 = each::get_random_vec3s(5, generator);
//     auto l_a5 = series::interleave(5, 0.0f);
//     auto la_5 = series::interleave(5, 0.0f);

//     SECTION("length(a*b) must equal length(a)*b for b>=0"){
//         each::mult(a, series::uniform(5.0f), a5);
//         each::length(a5, l_a5);
//         each::length(a, la_5);
//         each::mult(la_5, series::uniform(5.0f), la_5);
//         CHECK(whole::equal(la_5, l_a5, 1e-5));
//     }
// }

