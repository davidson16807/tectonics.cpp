

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <series/series.hpp>  
#include <series/arithmetic.hpp>  
#include <series/relational.hpp>  
#include <series/convenience.hpp>  
#include <series/operators.hpp>  

#include "exponential.hpp"
#include "relational.hpp"


TEST_CASE( "many<vec3> sqrt purity", "[many]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    series::vec3s a = series::get_random_vec3s(5, distribution, generator);
    series::vec3s c1 = series::get_random_vec3s(5, distribution, generator);
    series::vec3s c2 = series::get_random_vec3s(5, distribution, generator);
    SECTION("sqrt(a) must be called repeatedly without changing the output"){
        series::sqrt(a,c1);
        series::sqrt(a,c2);
        CHECK(series::equal(c1,c2));
    }
}
// TEST_CASE( "many<vec3> sqrt consistency", "[many]" ) {
//     std::mt19937 generator(2);
//     std::uniform_real_distribution<float> distribution(0.1,5.0);
//     series::vec3s a     = series::get_random_vec3s(5, distribution, generator);
//     series::vec3s b     = series::get_random_vec3s(5, distribution, generator);
//     series::vec3s sqrt1 = series::get_random_vec3s(5, distribution, generator);
//     series::vec3s sqrt2 = series::get_random_vec3s(5, distribution, generator);
//     series::vec3s log1_ = series::get_random_vec3s(5, distribution, generator);
//     series::vec3s log2_ = series::get_random_vec3s(5, distribution, generator);

//     SECTION("sqrt(a) must equal pow(a,1/2)"){
//         series::sqrt(a,    sqrt1);
//         series::pow (a,0.5f,sqrt2);
//         CHECK(series::equal(sqrt1 , sqrt2));
//     }

// }
TEST_CASE( "many<vec3> log consistency", "[many]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    series::vec3s a     = series::get_random_vec3s(5, distribution, generator);
    series::vec3s b     = series::get_random_vec3s(5, distribution, generator);
    series::vec3s log1_ = series::get_random_vec3s(5, distribution, generator);
    series::vec3s log2_ = series::get_random_vec3s(5, distribution, generator);

    SECTION("log2(a) must equal log(a)/log(2)"){
        series::log2(a, log1_);
        series::log (a, log2_);
        div (log2_, std::log(2.f), log2_);
        CHECK(series::equal(log1_ , log2_));
    }
}
TEST_CASE( "many<vec3> log/exp consistency", "[many]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    series::vec3s a     = series::get_random_vec3s(5, distribution, generator);
    series::vec3s b     = series::get_random_vec3s(5, distribution, generator);
    series::vec3s loga  = series::get_random_vec3s(5, distribution, generator);
    series::vec3s logb  = series::get_random_vec3s(5, distribution, generator);
    series::vec3s out   = series::get_random_vec3s(5, distribution, generator);

    SECTION("exp(log(a)+log(b)) must equal a*b"){
        series::log(a, loga);
        series::log(b, logb);
        series::exp(loga+logb, out);
        CHECK(series::equal(out, a*b));
    }
}
TEST_CASE( "many<vec3> log/exp invertibility", "[many]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    series::vec3s a   = series::get_random_vec3s(5, distribution, generator);
    series::vec3s b   = series::get_random_vec3s(5, distribution, generator);
    series::vec3s out = series::get_random_vec3s(5, distribution, generator);

    SECTION("log(exp(a)) must equal a"){
        series::log (a,   out);
        series::exp (out, out);
        CHECK(series::equal(a, out));
    }
    SECTION("log2(exp2(a)) must equal a"){
        series::log2(a,   out);
        series::exp2(out, out);
        CHECK(series::equal(a, out));
    }
}

TEST_CASE( "many<vec3> log2/exp2 consistency", "[many]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    series::vec3s a     = series::get_random_vec3s(5, distribution, generator);
    series::vec3s b     = series::get_random_vec3s(5, distribution, generator);
    series::vec3s loga  = series::get_random_vec3s(5, distribution, generator);
    series::vec3s logb  = series::get_random_vec3s(5, distribution, generator);
    series::vec3s out   = series::get_random_vec3s(5, distribution, generator);

    SECTION("exp2(log2(a)+log2(b)) must equal a*b"){
        series::log2(a, loga);
        series::log2(b, logb);
        series::exp2(loga+logb, out);
        CHECK(series::equal(out, a*b));
    }
}
TEST_CASE( "many<vec3> log2/exp2 invertibility", "[many]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    series::vec3s a   = series::get_random_vec3s(5, distribution, generator);
    series::vec3s b   = series::get_random_vec3s(5, distribution, generator);
    series::vec3s out = series::get_random_vec3s(5, distribution, generator);

    SECTION("log2(exp2(a)) must equal a"){
        series::log2 (a,   out);
        series::exp2 (out, out);
        CHECK(series::equal(a, out));
    }
    SECTION("log2(exp2(a)) must equal a"){
        series::log2(a,   out);
        series::exp2(out, out);
        CHECK(series::equal(a, out));
    }
}

