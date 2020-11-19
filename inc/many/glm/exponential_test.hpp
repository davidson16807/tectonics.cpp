

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <many/many.hpp>  
#include <many/arithmetic.hpp>  
#include <many/relational.hpp>  
#include <many/convenience.hpp>  
#include <many/operators.hpp>  

#include "exponential.hpp"
#include "relational.hpp"


TEST_CASE( "many<vec3> sqrt purity", "[many]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    many::vec3s a = many::get_random_vec3s(5, distribution, generator);
    many::vec3s c1 = many::get_random_vec3s(5, distribution, generator);
    many::vec3s c2 = many::get_random_vec3s(5, distribution, generator);
    SECTION("sqrt(a) must be called repeatedly without changing the output"){
        many::sqrt(a,c1);
        many::sqrt(a,c2);
        CHECK(many::equal(c1,c2));
    }
}
// TEST_CASE( "many<vec3> sqrt consistency", "[many]" ) {
//     std::mt19937 generator(2);
//     std::uniform_real_distribution<float> distribution(0.1,5.0);
//     many::vec3s a     = many::get_random_vec3s(5, distribution, generator);
//     many::vec3s b     = many::get_random_vec3s(5, distribution, generator);
//     many::vec3s sqrt1 = many::get_random_vec3s(5, distribution, generator);
//     many::vec3s sqrt2 = many::get_random_vec3s(5, distribution, generator);
//     many::vec3s log1_ = many::get_random_vec3s(5, distribution, generator);
//     many::vec3s log2_ = many::get_random_vec3s(5, distribution, generator);

//     SECTION("sqrt(a) must equal pow(a,1/2)"){
//         many::sqrt(a,    sqrt1);
//         many::pow (a,0.5f,sqrt2);
//         CHECK(many::equal(sqrt1 , sqrt2));
//     }

// }
TEST_CASE( "many<vec3> log consistency", "[many]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    many::vec3s a     = many::get_random_vec3s(5, distribution, generator);
    many::vec3s b     = many::get_random_vec3s(5, distribution, generator);
    many::vec3s log1_ = many::get_random_vec3s(5, distribution, generator);
    many::vec3s log2_ = many::get_random_vec3s(5, distribution, generator);

    SECTION("log2(a) must equal log(a)/log(2)"){
        many::log2(a, log1_);
        many::log (a, log2_);
        div (log2_, std::log(2.f), log2_);
        CHECK(many::equal(log1_ , log2_));
    }
}
TEST_CASE( "many<vec3> log/exp consistency", "[many]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    many::vec3s a     = many::get_random_vec3s(5, distribution, generator);
    many::vec3s b     = many::get_random_vec3s(5, distribution, generator);
    many::vec3s loga  = many::get_random_vec3s(5, distribution, generator);
    many::vec3s logb  = many::get_random_vec3s(5, distribution, generator);
    many::vec3s out   = many::get_random_vec3s(5, distribution, generator);

    SECTION("exp(log(a)+log(b)) must equal a*b"){
        many::log(a, loga);
        many::log(b, logb);
        many::exp(loga+logb, out);
        CHECK(many::equal(out, a*b));
    }
}
TEST_CASE( "many<vec3> log/exp invertibility", "[many]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    many::vec3s a   = many::get_random_vec3s(5, distribution, generator);
    many::vec3s b   = many::get_random_vec3s(5, distribution, generator);
    many::vec3s out = many::get_random_vec3s(5, distribution, generator);

    SECTION("log(exp(a)) must equal a"){
        many::log (a,   out);
        many::exp (out, out);
        CHECK(many::equal(a, out));
    }
    SECTION("log2(exp2(a)) must equal a"){
        many::log2(a,   out);
        many::exp2(out, out);
        CHECK(many::equal(a, out));
    }
}

TEST_CASE( "many<vec3> log2/exp2 consistency", "[many]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    many::vec3s a     = many::get_random_vec3s(5, distribution, generator);
    many::vec3s b     = many::get_random_vec3s(5, distribution, generator);
    many::vec3s loga  = many::get_random_vec3s(5, distribution, generator);
    many::vec3s logb  = many::get_random_vec3s(5, distribution, generator);
    many::vec3s out   = many::get_random_vec3s(5, distribution, generator);

    SECTION("exp2(log2(a)+log2(b)) must equal a*b"){
        many::log2(a, loga);
        many::log2(b, logb);
        many::exp2(loga+logb, out);
        CHECK(many::equal(out, a*b));
    }
}
TEST_CASE( "many<vec3> log2/exp2 invertibility", "[many]" ) {
    std::mt19937 generator(2);
    std::uniform_real_distribution<float> distribution(0.1,5.0);
    many::vec3s a   = many::get_random_vec3s(5, distribution, generator);
    many::vec3s b   = many::get_random_vec3s(5, distribution, generator);
    many::vec3s out = many::get_random_vec3s(5, distribution, generator);

    SECTION("log2(exp2(a)) must equal a"){
        many::log2 (a,   out);
        many::exp2 (out, out);
        CHECK(many::equal(a, out));
    }
    SECTION("log2(exp2(a)) must equal a"){
        many::log2(a,   out);
        many::exp2(out, out);
        CHECK(many::equal(a, out));
    }
}

