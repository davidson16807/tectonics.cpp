
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <many/many.hpp>  
#include <many/arithmetic.hpp>  
#include <many/convenience.hpp>  
#include <many/operators.hpp>  


TEST_CASE( "many<T> arithmetic purity", "[many]" ) {
    many::floats a = many::floats({1,2,3,4,5});
    many::floats b = many::floats({-1,1,-2,2,3});
    many::floats c1 = many::floats({0,0,0,0,0});
    many::floats c2 = many::floats({0,0,0,0,0});

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



TEST_CASE( "many<T> arithmetic identity", "[many]" ) {
    many::floats a = many::floats({1,2,3,4,5});
    many::floats zeros = many::floats({0,0,0,0,0});
    many::floats ones  = many::floats({1,1,1,1,1});

    SECTION("a+I must equal a"){
        CHECK(a+zeros==a);
        CHECK(a-zeros==a);
        CHECK(a*ones ==a);
        CHECK(a/ones ==a);
        CHECK(a-a==zeros);
        CHECK(a/a==ones );
    }
}

TEST_CASE( "many<T> arithmetic commutativity", "[many]" ) {
    many::floats a = many::floats({1,2,3,4,5});
    many::floats b = many::floats({-1,1,-2,2,3});

    SECTION("a+b must equal b+a"){
        CHECK(a+b==b+a);
    }
    SECTION("a*b must equal b*a"){
        CHECK(a*b==b*a);
    }
}


TEST_CASE( "many<T> arithmetic associativity", "[many]" ) {
    many::floats a = many::floats({1,2,3,4,5});
    many::floats b = many::floats({-1,1,-2,2,3});
    many::floats c = many::floats({1,1,2,3,5});

    SECTION("(a+b)+c must equal a+(b+c)"){
        CHECK((a+b)+c==a+(b+c));
    }
    SECTION("(a*b)*c must equal a*(b*c)"){
        CHECK((a*b)*c==a*(b*c));
    }
}


TEST_CASE( "many<T> arithmetic distributivity", "[many]" ) {
    many::floats a = many::floats({1,2,3,4,5});
    many::floats b = many::floats({-1,1,-2,2,3});
    many::floats c = many::floats({1,1,2,3,5});

    SECTION("a+b must equal b+a"){
        CHECK((a+b)*c==(a*c+b*c));
    }
    SECTION("a+b must equal b+a"){
        CHECK((a+b)/c==(a/c+b/c));
    }
}

