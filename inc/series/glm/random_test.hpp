

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec2.hpp>               // *vec3
#include <glm/vec3.hpp>               // *vec3

#include <series/series.hpp>  
#include <series/relational.hpp>  
#include <series/convenience.hpp>  
#include <series/glm/glm.hpp>         // *vec*s
#include <series/glm/random.hpp>      // get_elias_noise
#include <series/glm/convenience.hpp> //  operators, etc.

TEST_CASE( "many noise generation purity", "[many]" ) {
    SECTION("noise(seed) must generate the same output when called repeatedly"){
        CHECK(series::noise(0.1f)==series::noise(0.1f));
    }
    SECTION("noise(seeds) must generate the same output when called repeatedly"){
        CHECK(series::noise(glm::vec3(0.1f))==series::noise(glm::vec3(0.1f)));
    }
}
TEST_CASE( "many noise generation nontriviality", "[many]" ) {
    SECTION("noise(seed) must generate nontrivial output"){
        CHECK(series::noise(0.1f) > 0.f);
    }
    SECTION("noise(seeds) must generate nontrivial output"){
        CHECK(series::dot(series::noise(glm::vec3(1.f, 2.f, 3.f)), glm::vec3(1.f)) > 0.f);
    }
}

TEST_CASE( "many get_elias_noise generation determinism", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5,6});
    series::floats b = series::floats({1,1,2,3,5,8});
    series::vec3s positions({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_elias_noise(positions, generator) must generate the same many when given the same state of generator"){
        std::stringstream ss;
        std::mt19937 generator(2);
        ss << generator;
        series::get_elias_noise(positions, generator, a);
        ss >> generator;
        series::get_elias_noise(positions, generator, b);
        CHECK(series::equal(a,b));
    }
}
TEST_CASE( "many get_elias_noise generation nonpurity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5,6});
    series::floats b = series::floats({1,2,3,4,5,6});
    series::vec3s positions({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_elias_noise(positions, generator) must generate different output when called repeatedly"){
        std::mt19937 generator(2);
        series::get_elias_noise(positions, generator, a);
        series::get_elias_noise(positions, generator, b);
        CHECK(series::notEqual(a,b));
    }
}
TEST_CASE( "many get_elias_noise generation nontriviality", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5,6});
    series::vec3s positions({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_elias_noise(positions, generator) must generate nontrivial output"){
        std::mt19937 generator(2);
        series::get_elias_noise(positions, generator, a);
        CHECK(series::sum(a) > 0.f);
    }
}

TEST_CASE( "many get_perlin_noise generation purity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5,6});
    series::floats b = series::floats({1,2,3,4,5,6});
    series::vec3s positions({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_perlin_noise(positions) must generate different output when called repeatedly"){
        get_perlin_noise(positions, a);
        get_perlin_noise(positions, b);
        CHECK(series::equal(a,b));
    }
}
TEST_CASE( "many get_perlin_noise generation nontriviality", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5,6});
    series::vec3s positions({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_perlin_noise(positions) must generate nontrivial output"){
        get_perlin_noise(positions, a);
        CHECK(series::sum(a) > 0.f);
    }
}

TEST_CASE( "many get_worley_noise generation purity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5,6});
    series::floats b = series::floats({1,2,3,4,5,6});
    series::vec3s positions({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_worley_noise(positions) must generate different output when called repeatedly"){
        get_worley_noise(positions, a);
        get_worley_noise(positions, b);
        CHECK(series::equal(a,b));
    }
}
TEST_CASE( "many get_worley_noise generation nontriviality", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5,6});
    series::vec3s positions({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_worley_noise(positions) must generate nontrivial output"){
        get_worley_noise(positions, a);
        CHECK(series::sum(a) > 0.f);
    }
}