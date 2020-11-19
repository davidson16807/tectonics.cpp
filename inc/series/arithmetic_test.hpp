
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <series/series.hpp>  
#include <series/arithmetic.hpp>  
#include <series/convenience.hpp>  
#include <series/operators.hpp>  


TEST_CASE( "Series<T> arithmetic purity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5});
    series::floats b = series::floats({-1,1,-2,2,3});
    series::floats c1 = series::floats({0,0,0,0,0});
    series::floats c2 = series::floats({0,0,0,0,0});

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

TEST_CASE( "Series<T> arithmetic identity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5});
    series::floats zeros = series::floats({0,0,0,0,0});
    series::floats ones  = series::floats({1,1,1,1,1});

    SECTION("a+I must equal a"){
        CHECK(a+zeros==a);
        CHECK(a-zeros==a);
        CHECK(a*ones ==a);
        CHECK(a/ones ==a);
        CHECK(a-a==zeros);
        CHECK(a/a==ones );
    }
}

TEST_CASE( "Series<T> arithmetic commutativity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5});
    series::floats b = series::floats({-1,1,-2,2,3});

    SECTION("a+b must equal b+a"){
        CHECK(a+b==b+a);
    }
    SECTION("a*b must equal b*a"){
        CHECK(a*b==b*a);
    }
}

TEST_CASE( "Series<T> arithmetic associativity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5});
    series::floats b = series::floats({-1,1,-2,2,3});
    series::floats c = series::floats({1,1,2,3,5});

    SECTION("(a+b)+c must equal a+(b+c)"){
        CHECK((a+b)+c==a+(b+c));
    }
    SECTION("(a*b)*c must equal a*(b*c)"){
        CHECK((a*b)*c==a*(b*c));
    }
}

TEST_CASE( "Series<T> arithmetic distributivity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5});
    series::floats b = series::floats({-1,1,-2,2,3});
    series::floats c = series::floats({1,1,2,3,5});

    SECTION("a+b must equal b+a"){
        CHECK((a+b)*c==(a*c+b*c));
    }
    SECTION("a+b must equal b+a"){
        CHECK((a+b)/c==(a/c+b/c));
    }
}







TEST_CASE( "Series<T>/scalar arithmetic purity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5});
    float b = 2.0f;
    series::floats c1 = series::floats({0,0,0,0,0});
    series::floats c2 = series::floats({0,0,0,0,0});

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

TEST_CASE( "Series<T>/scalar arithmetic identity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5});
    float zeros = 0;
    float ones  = 1;

    SECTION("a+I must equal a"){
        CHECK(a+zeros==a);
        CHECK(a-zeros==a);
        CHECK(a*ones ==a);
        CHECK(a/ones ==a);
        CHECK(a-a==zeros);
        CHECK(a/a==ones );
    }
}

TEST_CASE( "Series<T>/scalar arithmetic commutativity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5});
    float b = 2.0f;

    SECTION("a+b must equal b+a"){
        CHECK(a+b==b+a);
    }
    SECTION("a*b must equal b*a"){
        CHECK(a*b==b*a);
    }
}


TEST_CASE( "Series<T>/scalar arithmetic associativity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5});
    float b = 2.0f;
    series::floats c = series::floats({1,1,2,3,5});

    SECTION("(a+b)+c must equal a+(b+c)"){
        CHECK((a+b)+c==a+(b+c));
    }
    SECTION("(a*b)*c must equal a*(b*c)"){
        CHECK((a*b)*c==a*(b*c));
    }
}


TEST_CASE( "Series<T>/scalar arithmetic distributivity", "[many]" ) {
    series::floats a = series::floats({1,2,3,4,5});
    float b = 2.0f;
    series::floats c = series::floats({1,1,2,3,5});

    SECTION("a+b must equal b+a"){
        CHECK((a+b)*c==(a*c+b*c));
    }
    SECTION("a+b must equal b+a"){
        CHECK((a+b)/c==(a/c+b/c));
    }
}

