
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include "Layers.hpp"
#include "convenience.hpp"


TEST_CASE( "many<T> arithmetic consistency", "[many]" ) {
    layers::Layers<float,5> a1 = layers::Layers<float,5>({1,2,3,4,5});
    layers::Layers<float,5> a2 = layers::Layers<float,5>({1,2,3,4,5});
    layers::Layers<float,5> a3 = layers::Layers<float,5>({1,2,3,4,5});
    layers::Layers<float,5> b  = layers::Layers<float,5>({-1,1,-2,2,3});

    SECTION("a+=b, a=a+b, and add(a,b,a) must all work the same"){
        a1 += b;
        a2 = a2+b;
        add(a3,b,a3);
        
        CHECK(a1 == a2);
        CHECK(a2 == a3);
    }

    SECTION("a*=b, a=a*b, and add(a,b,a) must all work the same"){
        a1 *= b;
        a2 = a2*b;
        mult(a3,b,a3);
        
        CHECK(a1 == a2);
        CHECK(a2 == a3);
    }

    SECTION("a-=b, a=a-b, and add(a,b,a) must all work the same"){
        a1 -= b;
        a2 = a2-b;
        sub(a3,b,a3);
        
        CHECK(a1 == a2);
        CHECK(a2 == a3);
    }

    SECTION("a/=b, a=a/b, and add(a,b,a) must all work the same"){
        a1 /= b;
        a2 = a2/b;
        div(a3,b,a3);
        
        CHECK(a1 == a2);
        CHECK(a2 == a3);
    }
}

