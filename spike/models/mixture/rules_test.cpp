
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "rules.hpp"


TEST_CASE( "linear_rule purity", "[mixture]" ) {
  SECTION("Calling an operation twice with the same arguments must produce the same results")
  {
    using PolynomialRailyardRelation = compound::relation::PolynomialRailyardRelation<si::temperature<double>, si::pressure<double>, 0, 1>;
    std::vector<PolynomialRailyardRelation> relations {
        PolynomialRailyardRelation(2.0*si::pascal),
        PolynomialRailyardRelation(3.0*si::pascal)
    };
    for (double i = 0; i<1.0; i+=0.33) {
        double j = 1.0 - i;
        CHECK(mixture::linear_rule(relations, std::vector<double>{i, j})(si::standard_temperature) == 
              mixture::linear_rule(relations, std::vector<double>{i, j})(si::standard_temperature) );
    }
  }
}

TEST_CASE( "linear_rule codomain", "[mixture]" ) {
  SECTION("Output must fall within a known range")
  {
    using PolynomialRailyardRelation = compound::relation::PolynomialRailyardRelation<si::temperature<double>, si::pressure<double>, 0, 1>;
    std::vector<PolynomialRailyardRelation> relations {
        PolynomialRailyardRelation(2.0*si::pascal),
        PolynomialRailyardRelation(3.0*si::pascal)
    };
    for (double i = 0; i<1.0; i+=0.33) {
        double j = 1.0 - i;
        CHECK(2.0*si::pascal <= mixture::linear_rule(relations, {i, j})(si::standard_temperature));
        CHECK(mixture::linear_rule(relations, {i, j})(si::standard_temperature) <= 3.0*si::pascal);
    }
  }
}

TEST_CASE( "linear_rule commutativity", "[mixture]" ) {
  SECTION("Arguments can be applied in any order and still produce the same results")
  {
    using PolynomialRailyardRelation = compound::relation::PolynomialRailyardRelation<si::temperature<double>, si::pressure<double>, 0, 1>;
    std::vector<PolynomialRailyardRelation> ab {
        PolynomialRailyardRelation(2.0*si::pascal),
        PolynomialRailyardRelation(3.0*si::pascal)
    };
    std::vector<PolynomialRailyardRelation> ba {
        PolynomialRailyardRelation(3.0*si::pascal),
        PolynomialRailyardRelation(2.0*si::pascal)
    };
    for (double i = 0; i<1.0; i+=0.33) {
        double j = 1.0 - i;
        CHECK(mixture::linear_rule(ab, {i, j})(si::standard_temperature) == 
              mixture::linear_rule(ba, {j, i})(si::standard_temperature));
    }
  }
}
