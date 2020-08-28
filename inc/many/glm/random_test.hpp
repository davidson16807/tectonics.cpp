

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch/catch.hpp"

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#include <glm/vec2.hpp>               // *vec3
#include <glm/vec3.hpp>               // *vec3

#include <many/many.hpp>  
#include <many/convenience.hpp>  
#include <many/glm/glm.hpp>         // *vec*s
#include <many/glm/random.hpp>      // get_elias_noise
#include <many/glm/convenience.hpp> //  operators, etc.

TEST_CASE( "many noise generation purity", "[many]" ) {
    SECTION("noise(seed) must generate the same output when called repeatedly"){
        CHECK(many::noise(0.1f)==many::noise(0.1f));
    }
    SECTION("noise(seeds) must generate the same output when called repeatedly"){
        CHECK(many::noise(glm::vec3(0.1f))==many::noise(glm::vec3(0.1f)));
    }
}
TEST_CASE( "many noise generation nontriviality", "[many]" ) {
    SECTION("noise(seed) must generate nontrivial output"){
        CHECK(many::noise(0.1f) > 0.f);
    }
    SECTION("noise(seeds) must generate nontrivial output"){
        CHECK(many::dot(many::noise(glm::vec3(1.f, 2.f, 3.f)), glm::vec3(1.f)) > 0.f);
    }
}

TEST_CASE( "many get_elias_noise generation determinism", "[many]" ) {
    many::floats a = many::floats({1,2,3,4,5,6});
    many::floats b = many::floats({1,1,2,3,5,8});
    many::vec3s positions({
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
        many::get_elias_noise(positions, generator, a);
        ss >> generator;
        many::get_elias_noise(positions, generator, b);
        CHECK(many::equal(a,b));
    }
}
TEST_CASE( "many get_elias_noise generation nonpurity", "[many]" ) {
    many::floats a = many::floats({1,2,3,4,5,6});
    many::floats b = many::floats({1,2,3,4,5,6});
    many::vec3s positions({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_elias_noise(positions, generator) must generate different output when called repeatedly"){
        std::mt19937 generator(2);
        many::get_elias_noise(positions, generator, a);
        many::get_elias_noise(positions, generator, b);
        CHECK(many::notEqual(a,b));
    }
}
TEST_CASE( "many get_elias_noise generation nontriviality", "[many]" ) {
    many::floats a = many::floats({1,2,3,4,5,6});
    many::vec3s positions({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_elias_noise(positions, generator) must generate nontrivial output"){
        std::mt19937 generator(2);
        many::get_elias_noise(positions, generator, a);
        CHECK(many::sum(a) > 0.f);
    }
}

TEST_CASE( "many get_perlin_noise generation purity", "[many]" ) {
    many::floats a = many::floats({1,2,3,4,5,6});
    many::floats b = many::floats({1,2,3,4,5,6});
    many::vec3s positions({
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
        CHECK(many::equal(a,b));
    }
}
TEST_CASE( "many get_perlin_noise generation nontriviality", "[many]" ) {
    many::floats a = many::floats({1,2,3,4,5,6});
    many::vec3s positions({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_perlin_noise(positions) must generate nontrivial output"){
        get_perlin_noise(positions, a);
        CHECK(many::sum(a) > 0.f);
    }
}

TEST_CASE( "many get_worley_noise generation purity", "[many]" ) {
    many::floats a = many::floats({1,2,3,4,5,6});
    many::floats b = many::floats({1,2,3,4,5,6});
    many::vec3s positions({
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
        CHECK(many::equal(a,b));
    }
}
TEST_CASE( "many get_worley_noise generation nontriviality", "[many]" ) {
    many::floats a = many::floats({1,2,3,4,5,6});
    many::vec3s positions({
            glm::vec3( 1, 0, 0),
            glm::vec3(-1, 0, 0),
            glm::vec3( 0, 1, 0),
            glm::vec3( 0,-1, 0),
            glm::vec3( 0, 0, 1),
            glm::vec3( 0, 0,-1),
        });
    SECTION("get_worley_noise(positions) must generate nontrivial output"){
        get_worley_noise(positions, a);
        CHECK(many::sum(a) > 0.f);
    }
}