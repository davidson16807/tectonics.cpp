
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "Layers.hpp"
#include "arithmetic.hpp"
#include "convenience.hpp"
#include "operators.hpp"


TEST_CASE( "Layers<T,N> arithmetic purity", "[layers]" ) {
    layers::Layers<float,5> a = layers::Layers<float,5>({1,2,3,4,5});
    layers::Layers<float,5> b = layers::Layers<float,5>({-1,1,-2,2,3});
    layers::Layers<float,5> c1 = layers::Layers<float,5>({0,0,0,0,0});
    layers::Layers<float,5> c2 = layers::Layers<float,5>({0,0,0,0,0});

    SECTION("a+b must be called repeatedly without changing the output"){
        c1 = a + b;
        c2 = a + b;
        CHECK(c1==c2);

        add(a,b,c1);
        add(a,b,c2);
        CHECK(c1==c2);
    }

    SECTION("a*b must be called repeatedly without changing the output"){
        c1 = a * b;
        c2 = a * b;
        CHECK(c1==c2);
        
        mult(a,b,c1);
        mult(a,b,c2);
        CHECK(c1==c2);
    }

    SECTION("a-b must be called repeatedly without changing the output"){
        c1 = a - b;
        c2 = a - b;
        CHECK(c1==c2);
        
        sub(a,b,c1);
        sub(a,b,c2);
        CHECK(c1==c2);
    }

    SECTION("a/b must be called repeatedly without changing the output"){
        c1 = a / b;
        c2 = a / b;
        CHECK(c1==c2);
        
        div(a,b,c1);
        div(a,b,c2);
        CHECK(c1==c2);
    }
}



TEST_CASE( "Layers<T,N> arithmetic identity", "[layers]" ) {
    layers::Layers<float,5> a = layers::Layers<float,5>({1,2,3,4,5});
    layers::Layers<float,5> zeros = layers::Layers<float,5>({0,0,0,0,0});
    layers::Layers<float,5> ones  = layers::Layers<float,5>({1,1,1,1,1});

    SECTION("a+I must equal a"){
        CHECK(a+zeros==a);
        CHECK(a-zeros==a);
        CHECK(a*ones ==a);
        CHECK(a/ones ==a);
        CHECK(a-a==zeros);
        CHECK(a/a==ones );
    }
}

TEST_CASE( "Layers<T,N> arithmetic commutativity", "[layers]" ) {
    layers::Layers<float,5> a = layers::Layers<float,5>({1,2,3,4,5});
    layers::Layers<float,5> b = layers::Layers<float,5>({-1,1,-2,2,3});

    SECTION("a+b must equal b+a"){
        CHECK(a+b==b+a);
    }
    SECTION("a*b must equal b*a"){
        CHECK(a*b==b*a);
    }
}


TEST_CASE( "Layers<T,N> arithmetic associativity", "[layers]" ) {
    layers::Layers<float,5> a = layers::Layers<float,5>({1,2,3,4,5});
    layers::Layers<float,5> b = layers::Layers<float,5>({-1,1,-2,2,3});
    layers::Layers<float,5> c = layers::Layers<float,5>({1,1,2,3,5});

    SECTION("(a+b)+c must equal a+(b+c)"){
        CHECK((a+b)+c==a+(b+c));
    }
    SECTION("(a*b)*c must equal a*(b*c)"){
        CHECK((a*b)*c==a*(b*c));
    }
}


TEST_CASE( "Layers<T,N> arithmetic distributivity", "[layers]" ) {
    layers::Layers<float,5> a = layers::Layers<float,5>({1,2,3,4,5});
    layers::Layers<float,5> b = layers::Layers<float,5>({-1,1,-2,2,3});
    layers::Layers<float,5> c = layers::Layers<float,5>({1,1,2,3,5});

    SECTION("a+b must equal b+a"){
        CHECK((a+b)*c==(a*c+b*c));
    }
    SECTION("a+b must equal b+a"){
        CHECK((a+b)/c==(a/c+b/c));
    }
}

