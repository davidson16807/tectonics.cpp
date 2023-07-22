
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "Series.hpp"
#include "each.hpp"
#include "morphologic.hpp"

TEST_CASE( "Series<T> morphologic purity", "[many]" ) {
    auto a = data::series({1,0,0,1,0});
    auto b = data::series({0,1,0,1,0});
    auto c1 = data::series({0,0,0,0,0});
    auto c2 = data::series({0,0,0,0,0});

    SECTION("a∩b must be called repeatedly without changing the output"){
        data::intersect(a,b,c1);
        data::intersect(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a∪b must be called repeatedly without changing the output"){
        data::unite(a,b,c1);
        data::unite(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("a-b must be called repeatedly without changing the output"){
        data::differ(a,b,c1);
        data::differ(a,b,c2);
        CHECK(whole::equal(c1, c2));
    }

    SECTION("-b must be called repeatedly without changing the output"){
        data::negate(a,c1);
        data::negate(a,c2);
        CHECK(whole::equal(c1, c2));
    }
}

TEST_CASE( "Series<T> morphologic identity", "[many]" ) {
    auto a = data::series({1,0,0,1,0});
    auto zeros = data::series({0,0,0,0,0});
    auto ones  = data::series({1,1,1,1,1});
    auto c = data::series({0,0,0,0,0});

    SECTION("a∪∅ must equal a"){
        data::unite(a,zeros, c);
        CHECK(whole::equal(c, a));
    }
    SECTION("a∩1 must equal a"){
        data::intersect(a,ones, c);
        CHECK(whole::equal(c, a));
    }
    SECTION("a-∅ must equal a"){
        data::differ(a,zeros, c);
        CHECK(whole::equal(c, a));
    }
}

TEST_CASE( "Series<T> morphologic commutativity", "[many]" ) {
    auto a = data::series({1,0,0,1,0});
    auto b = data::series({0,1,0,1,0});
    auto ab = data::series({0,0,0,0,0});
    auto ba = data::series({0,0,0,0,0});

    SECTION("a∪b must equal b∪a"){
        data::unite(a, b, ab);
        data::unite(b, a, ba);
        CHECK(whole::equal(ab, ba));
    }

    SECTION("a∩b must equal b∩a"){
        data::intersect(a, b, ab);
        data::intersect(b, a, ba);
        CHECK(whole::equal(ab, ba));
    }

}

TEST_CASE( "Series<T> negation involutivity", "[many]" ) {
    auto a = data::series({1,0,0,1,0});
    auto c = data::series({1,1,2,3,5});

    SECTION("--a must equal a"){
        data::negate(a, c);
        data::negate(c, c);
        CHECK(whole::equal(c, a));
    }

}

TEST_CASE( "Series<T> morphologic associativity", "[many]" ) {

    auto a = data::series({1,0,0,1,0});
    auto b = data::series({0,1,0,1,0});
    auto c = data::series({1,1,2,3,5});
    auto ab_c = data::series({0,0,0,0,0});
    auto a_bc = data::series({0,0,0,0,0});

    SECTION("(a∪b)∪c must equal a∪(b∪c)"){
        data::unite(a,b,    ab_c);
        data::unite(ab_c,c, ab_c);
        data::unite(b,c,    a_bc);
        data::unite(a,a_bc, a_bc);
        CHECK(whole::equal(ab_c, a_bc));
    }
    SECTION("(a∩b)∩c must equal a∩(b∩c)"){
        data::intersect(a,b,    ab_c);
        data::intersect(ab_c,c, ab_c);
        data::intersect(b,c,    a_bc);
        data::intersect(a,a_bc, a_bc);
        CHECK(whole::equal(ab_c, a_bc));
    }
}

TEST_CASE( "Series<T> morphologic distributivity", "[many]" ) {
    auto a = data::series({1,0,0,1,0});
    auto b = data::series({0,1,0,1,0});
    auto c = data::series({1.0,1.0,2.0,3.0,5.0});
    auto ab_c  = data::series({0.0,0.0,0.0,0.0,0.0});
    auto ac_bc = data::series({0.0,0.0,0.0,0.0,0.0});
    auto ac = data::series({0.0,0.0,0.0,0.0,0.0});
    auto bc = data::series({0.0,0.0,0.0,0.0,0.0});

    SECTION("a+b*c must equal a*c+b*c"){
        data::unite(a,b,    ab_c);
        data::intersect(ab_c,c, ab_c);
        data::intersect(a,c,    ac);
        data::intersect(b,c,    bc);
        data::unite(ac,bc,  ac_bc);
        CHECK(whole::equal(ab_c, ac_bc));
    }
}







