
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include <units/si.hpp>
#include "units/si_test_utils.hpp"

#include "OptionalConstantField.hpp"

TEST_CASE( "OptionalConstantField value_or() purity", "[field]" ) {
    compound::field::EmptyParameters X;
    
    compound::field::OptionalConstantField<double> unknown  = std::monostate();
    compound::field::OptionalConstantField<double> constant  = 1.0;

    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        CHECK(unknown.value_or(unknown ) 
           == unknown.value_or(unknown ));
        CHECK(constant.value_or(constant) 
           == constant.value_or(constant));
    }
}

TEST_CASE( "OptionalConstantField value_or() identity", "[field]" ) {
    compound::field::EmptyParameters X;
    
    compound::field::OptionalConstantField<double> unknown  = std::monostate();
    compound::field::OptionalConstantField<double> constant  = 1.0;

    SECTION("There exists a entry that when applied to a function returns the original entry")
    {
        CHECK(unknown.value_or(unknown) == unknown);
        CHECK(constant.value_or(unknown) == constant);
    }
}

TEST_CASE( "OptionalConstantField value_or() associativity", "[field]" ) {
    compound::field::EmptyParameters X;
    
    compound::field::OptionalConstantField<double> constant1  = 1.0;
    compound::field::OptionalConstantField<double> constant2  = 2.0;
    compound::field::OptionalConstantField<double> constant3  = 3.0;

    SECTION("Functions can be applied in any order and still produce the same results")
    {
        CHECK(constant1.value_or(constant2.value_or(constant3)) == 
              constant1.value_or(constant2).value_or(constant3));

        CHECK(constant1.value_or(constant3.value_or(constant2)) == 
              constant1.value_or(constant3).value_or(constant2));



        CHECK(constant2.value_or(constant1.value_or(constant3)) == 
              constant2.value_or(constant1).value_or(constant3));

        CHECK(constant2.value_or(constant3.value_or(constant1)) == 
              constant2.value_or(constant3).value_or(constant1));



        CHECK(constant3.value_or(constant1.value_or(constant2)) == 
              constant3.value_or(constant1).value_or(constant2));

        CHECK(constant3.value_or(constant2.value_or(constant1)) == 
              constant3.value_or(constant2).value_or(constant1));

    }
}


TEST_CASE( "OptionalConstantField value_or() increasing", "[field]" ) {
    compound::field::EmptyParameters X;
    
    compound::field::OptionalConstantField<double> unknown  = std::monostate();
    compound::field::OptionalConstantField<double> constant  = 1.0;

    SECTION("An attribute of a function's return entry either increases or remains the same when compared to the same attribute of the input entry")
    {
        CHECK(unknown.value_or(unknown).index() >= unknown.index());
        CHECK(unknown.value_or(constant).index() >= unknown.index());

        CHECK(constant.value_or(unknown).index() >= constant.index());
        CHECK(constant.value_or(constant).index() >= constant.index());
    }
}






TEST_CASE( "OptionalConstantField value_or(f,a) purity", "[field]" ) {
    compound::field::EmptyParameters X;
    
    compound::field::OptionalConstantField<double> unknown  = std::monostate();
    compound::field::OptionalConstantField<double> constant  = 1.0;

    compound::field::OptionalConstantField<int> unknown_i  = std::monostate();
    compound::field::OptionalConstantField<int> constant_i  = 1.0;

    std::function<double(const double)> f_d  = [](const double first){ return 1.0 - 2.0*first; };
    std::function<double(const int)> f_i  = [](const int first){ return 1.0 - 2.0*first; };

    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        CHECK(unknown.value_or(f_i, X, unknown_i ) == unknown.value_or(f_i, X, unknown_i ));
        CHECK(constant.value_or(f_i, X, constant_i) == constant.value_or(f_i, X, constant_i));

        CHECK(unknown.value_or(f_d, X, unknown ) == unknown.value_or(f_d, X, unknown ));
        CHECK(constant.value_or(f_d, X, constant) == constant.value_or(f_d, X, constant));
    }
}

TEST_CASE( "OptionalConstantField value_or(f,a) identity", "[field]" ) {
    compound::field::EmptyParameters X;

    compound::field::OptionalConstantField<double> unknown  = std::monostate();
    compound::field::OptionalConstantField<double> constant  = 1.0;

    std::function<double(const double)> I  = [](const double first){ return first; };

    SECTION("There exists a function that when applied returns the original entry")
    {
        CHECK(unknown.value_or(I, X, unknown ) == unknown);
        CHECK(constant.value_or(I, X, constant) == constant);
    }
}





TEST_CASE( "OptionalConstantField value_or(f,a,b) purity", "[field]" ) {
    compound::field::EmptyParameters X;

    compound::field::OptionalConstantField<double> unknown_d  = std::monostate();
    compound::field::OptionalConstantField<double> constant_d  = 1.0;
    std::function<double(const double, const double)> f_dd  = [](const double first, const double second){ return 1.0 - 2.0*first + 3.0*second; };

    compound::field::OptionalConstantField<int> unknown_i  = std::monostate();
    compound::field::OptionalConstantField<int> constant_i  = 1.0;
    std::function<double(const int, const double)> f_id  = [](const int first, const double second){ return first + second; };
    std::function<double(const double, const int)> f_di  = [](const double first, const int second){ return first + second; };
    std::function<double(const int, const int)> f_ii  = [](const int first, const int second){ return first + second; };

    SECTION("Calling a function twice with the same arguments must produce the same results")
    {
        CHECK(unknown_d.value_or(f_dd, X, unknown_d,  unknown_d) == unknown_d.value_or(f_dd, X, unknown_d,  unknown_d));
        CHECK(unknown_d.value_or(f_dd, X, constant_d, unknown_d) == unknown_d.value_or(f_dd, X, constant_d, unknown_d));

        CHECK(unknown_d.value_or(f_dd, X, unknown_d,  constant_d) == unknown_d.value_or(f_dd, X, unknown_d,  constant_d));
        CHECK(unknown_d.value_or(f_dd, X, constant_d, constant_d) == unknown_d.value_or(f_dd, X, constant_d, constant_d));

        CHECK(constant_d.value_or(f_dd, X, constant_d, constant_d) == constant_d.value_or(f_dd, X, constant_d, constant_d));



        CHECK(unknown_d.value_or(f_ii, X, unknown_i,  unknown_i) == unknown_d.value_or(f_ii, X, unknown_i,  unknown_i));
        CHECK(unknown_d.value_or(f_ii, X, constant_i, unknown_i) == unknown_d.value_or(f_ii, X, constant_i, unknown_i));

        CHECK(unknown_d.value_or(f_ii, X, unknown_i,  constant_i) == unknown_d.value_or(f_ii, X, unknown_i,  constant_i));
        CHECK(unknown_d.value_or(f_ii, X, constant_i, constant_i) == unknown_d.value_or(f_ii, X, constant_i, constant_i));

        CHECK(constant_d.value_or(f_ii, X, constant_i, constant_i) == constant_d.value_or(f_ii, X, constant_i, constant_i));





        CHECK(unknown_d.value_or(f_id, X, unknown_i,  unknown_d) == unknown_d.value_or(f_id, X, unknown_i,  unknown_d));
        CHECK(unknown_d.value_or(f_id, X, constant_i, unknown_d) == unknown_d.value_or(f_id, X, constant_i, unknown_d));

        CHECK(unknown_d.value_or(f_id, X, unknown_i,  constant_d) == unknown_d.value_or(f_id, X, unknown_i,  constant_d));
        CHECK(unknown_d.value_or(f_id, X, constant_i, constant_d) == unknown_d.value_or(f_id, X, constant_i, constant_d));

        CHECK(constant_d.value_or(f_id, X, constant_i, constant_d) == constant_d.value_or(f_id, X, constant_i, constant_d));





        CHECK(unknown_d.value_or(f_di, X, unknown_d,  unknown_i) == unknown_d.value_or(f_di, X, unknown_d,  unknown_i));
        CHECK(unknown_d.value_or(f_di, X, constant_d, unknown_i) == unknown_d.value_or(f_di, X, constant_d, unknown_i));

        CHECK(unknown_d.value_or(f_di, X, unknown_d,  constant_i) == unknown_d.value_or(f_di, X, unknown_d,  constant_i));
        CHECK(unknown_d.value_or(f_di, X, constant_d, constant_i) == unknown_d.value_or(f_di, X, constant_d, constant_i));

        CHECK(constant_d.value_or(f_di, X, constant_d, constant_i) == constant_d.value_or(f_di, X, constant_d, constant_i));


    }
}

TEST_CASE( "OptionalConstantField value_or(f_dd,a,b) free theorem identity", "[field]" ) {
    compound::field::EmptyParameters X;

    compound::field::OptionalConstantField<double> unknown_d  = std::monostate();
    compound::field::OptionalConstantField<double> constant_d  = 1.0;
    std::function<double(const double, const double)> f_dd  = [](const double first, const double second){ return first + second; };
    compound::field::OptionalConstantField<double> I  = 0.0;

    SECTION("A function will have the identity property if it is passed a function that itself has the identity property (this presumes for our case that the function is invoked)")
    {
        CHECK(unknown_d.value_or(f_dd, X, unknown_d,  I) == unknown_d );
        CHECK(unknown_d.value_or(f_dd, X, constant_d, I) == constant_d);
    }
}

TEST_CASE( "OptionalConstantField value_or(f_dd,a,b) free theorem commutativity", "[field]" ) {
    compound::field::EmptyParameters X;

    compound::field::OptionalConstantField<double> unknown_d  = std::monostate();
    compound::field::OptionalConstantField<double> constant_d  = 1.0;
    std::function<double(const double, const double)> f_dd  = [](const double first, const double second){ return first + second; };

    SECTION("A function will have the commutativity property if it is passed a function that itself has the commutativity property (this presumes for our case that the function is invoked)")
    {
        CHECK(unknown_d.value_or(f_dd, X, unknown_d,  unknown_d) == unknown_d.value_or(f_dd, X, unknown_d, unknown_d  ));
        CHECK(unknown_d.value_or(f_dd, X, constant_d, unknown_d) == unknown_d.value_or(f_dd, X, unknown_d, constant_d ));

        CHECK(unknown_d.value_or(f_dd, X, unknown_d,  constant_d) == unknown_d.value_or(f_dd, X, constant_d, unknown_d  ));
        CHECK(unknown_d.value_or(f_dd, X, constant_d, constant_d) == unknown_d.value_or(f_dd, X, constant_d, constant_d ));


    }
}
