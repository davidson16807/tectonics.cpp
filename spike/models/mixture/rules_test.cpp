
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "rules.hpp"


TEST_CASE( "linear_rule purity for doubles", "[mixture]" ) {
  SECTION("Calling an operation twice with the same arguments must produce the same results")
  {
    std::vector<double> relations {
        2.0,
        3.0
    };
    for (double i = 0; i<1.0; i+=0.2) {
        double j = 1.0 - i;
        CHECK(mixture::linear_rule(relations, {i, j}) == 
              mixture::linear_rule(relations, {i, j}) );
    }
  }
}

TEST_CASE( "linear_rule codomain for doubles", "[mixture]" ) {
  SECTION("Output must fall within a known range")
  {
    std::vector<double> relations {
        2.0,
        3.0
    };
    for (double i = 0; i<1.0; i+=0.2) {
        double j = 1.0 - i;
        CHECK(2.0 <= mixture::linear_rule(relations, {i, j}));
        CHECK(mixture::linear_rule(relations, {i, j}) <= 3.0);
    }
  }
}

TEST_CASE( "linear_rule commutativity for doubles", "[mixture]" ) {
  SECTION("Arguments can be consistently rearranged in any order and still produce the same results")
  {
    std::vector<double> ab {
        2.0,
        3.0
    };
    std::vector<double> ba {
        3.0,
        2.0
    };
    for (double i = 0; i<1.0; i+=0.2) {
        double j = 1.0 - i;
        CHECK(mixture::linear_rule(ab, {i, j}) == 
              mixture::linear_rule(ba, {j, i}));
    }
  }
}


TEST_CASE( "linear_rule purity for PolynomialRailyardRelation", "[mixture]" ) {
  SECTION("Calling an operation twice with the same arguments must produce the same results")
  {
    using PolynomialRailyardRelation = compound::relation::PolynomialRailyardRelation<si::temperature<double>, si::pressure<double>, 0, 1>;
    std::vector<PolynomialRailyardRelation> relations {
        PolynomialRailyardRelation(2.0*si::pascal),
        PolynomialRailyardRelation(3.0*si::pascal)
    };
    for (double i = 0; i<1.0; i+=0.2) {
        double j = 1.0 - i;
        CHECK(mixture::linear_rule(relations, {i, j})(si::standard_temperature) == 
              mixture::linear_rule(relations, {i, j})(si::standard_temperature) );
    }
  }
}

TEST_CASE( "linear_rule codomain for PolynomialRailyardRelation", "[mixture]" ) {
  SECTION("Output must fall within a known range")
  {
    using PolynomialRailyardRelation = compound::relation::PolynomialRailyardRelation<si::temperature<double>, si::pressure<double>, 0, 1>;
    std::vector<PolynomialRailyardRelation> relations {
        PolynomialRailyardRelation(2.0*si::pascal),
        PolynomialRailyardRelation(3.0*si::pascal)
    };
    for (double i = 0; i<1.0; i+=0.2) {
        double j = 1.0 - i;
        CHECK(2.0*si::pascal <= mixture::linear_rule(relations, {i, j})(si::standard_temperature));
        CHECK(mixture::linear_rule(relations, {i, j})(si::standard_temperature) <= 3.0*si::pascal);
    }
  }
}

TEST_CASE( "linear_rule commutativity for PolynomialRailyardRelation", "[mixture]" ) {
  SECTION("Arguments can be consistently rearranged in any order and still produce the same results")
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
    for (double i = 0; i<1.0; i+=0.2) {
        double j = 1.0 - i;
        CHECK(mixture::linear_rule(ab, {i, j})(si::standard_temperature) == 
              mixture::linear_rule(ba, {j, i})(si::standard_temperature));
    }
  }
}


TEST_CASE( "linear_rule purity for ExponentiatedPolynomialRailyardRelation", "[mixture]" ) {
  SECTION("Calling an operation twice with the same arguments must produce the same results")
  {
    using ExponentiatedPolynomialRailyardRelation = compound::relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>, si::pressure<double>, 0, 1>;
    std::vector<ExponentiatedPolynomialRailyardRelation> relations {
        ExponentiatedPolynomialRailyardRelation(2.0*si::pascal),
        ExponentiatedPolynomialRailyardRelation(3.0*si::pascal)
    };
    for (double i = 0; i<1.0; i+=0.2) {
        double j = 1.0 - i;
        CHECK(mixture::linear_rule(relations, {i, j})(si::standard_temperature) == 
              mixture::linear_rule(relations, {i, j})(si::standard_temperature) );
    }
  }
}

TEST_CASE( "linear_rule codomain for ExponentiatedPolynomialRailyardRelation", "[mixture]" ) {
  SECTION("Output must fall within a known range")
  {
    using ExponentiatedPolynomialRailyardRelation = compound::relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>, si::pressure<double>, 0, 1>;
    std::vector<ExponentiatedPolynomialRailyardRelation> relations {
        ExponentiatedPolynomialRailyardRelation(2.0*si::pascal),
        ExponentiatedPolynomialRailyardRelation(3.0*si::pascal)
    };
    for (double i = 0; i<1.0; i+=0.2) {
        double j = 1.0 - i;
        CHECK(2.0*si::pascal <= mixture::linear_rule(relations, {i, j})(si::standard_temperature));
        CHECK(mixture::linear_rule(relations, {i, j})(si::standard_temperature) <= 3.0*si::pascal);
    }
  }
}

TEST_CASE( "linear_rule commutativity for ExponentiatedPolynomialRailyardRelation", "[mixture]" ) {
  SECTION("Arguments can be consistently rearranged in any order and still produce the same results")
  {
    using ExponentiatedPolynomialRailyardRelation = compound::relation::ExponentiatedPolynomialRailyardRelation<si::temperature<double>, si::pressure<double>, 0, 1>;
    std::vector<ExponentiatedPolynomialRailyardRelation> ab {
        ExponentiatedPolynomialRailyardRelation(2.0*si::pascal),
        ExponentiatedPolynomialRailyardRelation(3.0*si::pascal)
    };
    std::vector<ExponentiatedPolynomialRailyardRelation> ba {
        ExponentiatedPolynomialRailyardRelation(3.0*si::pascal),
        ExponentiatedPolynomialRailyardRelation(2.0*si::pascal)
    };
    for (double i = 0; i<1.0; i+=0.2) {
        double j = 1.0 - i;
        CHECK(mixture::linear_rule(ab, {i, j})(si::standard_temperature) == 
              mixture::linear_rule(ba, {j, i})(si::standard_temperature));
    }
  }
}
