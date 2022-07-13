
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "si.hpp"
#include "si_test_utils.hpp"

si::molar_heat_capacity<double> C1 = 2.0*si::joule/(si::kelvin*si::mole);
si::molar_heat_capacity<double> C2 = -3.0*si::joule/(si::kelvin*si::mole);
si::molar_heat_capacity<double> C3 = 4.0*si::joule/(si::kelvin*si::mole);
si::luminous_intensity<double> L1 = 3.0*si::candela;
si::luminous_intensity<double> L2 = -4.0*si::candela;
si::luminous_intensity<double> L3 = 5.0*si::candela;
si::voltage<double> V1 = -5.0*si::volt;
si::voltage<double> V2 = 6.0*si::volt;
si::voltage<double> V3 = -7.0*si::volt;

si::molar_heat_capacity<double> C1copy = 2.0*si::joule/(si::kelvin*si::mole);
si::molar_heat_capacity<double> C2copy = -3.0*si::joule/(si::kelvin*si::mole);
si::molar_heat_capacity<double> C3copy = 4.0*si::joule/(si::kelvin*si::mole);
si::luminous_intensity<double> L1copy = 3.0*si::candela;
si::luminous_intensity<double> L2copy = -4.0*si::candela;
si::luminous_intensity<double> L3copy = 5.0*si::candela;
si::voltage<double> V1copy = -5.0*si::volt;
si::voltage<double> V2copy = 6.0*si::volt;
si::voltage<double> V3copy = -7.0*si::volt;

TEST_CASE( "units multiplication purity", "[si]" ) {
  SECTION("Calling an operation twice with the same arguments must produce the same results")
  {
    CHECK(C1*C2*L1*L2*V1*V2 == C1copy*C2copy*L1copy*L2copy*V1copy*V2copy);
  }
}
TEST_CASE( "units multiplication identity", "[si]" ) {
  SECTION("There exists a entry that when applied to a function returns the original entry")
  {
    CHECK(C1*1.0 == C1);
    CHECK(L1*1.0 == L1);
    CHECK(V1*1.0 == V1);
  }
}

TEST_CASE( "units multiplication associativity", "[si]" ) {
  SECTION("Functions can be applied in any order and still produce the same results")
  {
    CHECK(C1*(L1*V1) == (C1*L1)*V1);
  }
}
TEST_CASE( "units multiplication invertibility", "[si]" ) {
  SECTION("For every function there exists another function that negates its effect")
  {
    CHECK(C1*L1/L1 == C1);
    CHECK(L1*V1/V1 == L1);
    CHECK(V1*C1/C1 == V1);

    CHECK(C1*L1 * 1.0/L1 == C1);
    CHECK(L1*V1 * 1.0/V1 == L1);
    CHECK(V1*C1 * 1.0/C1 == V1);
  }
}
TEST_CASE( "units multiplication commutativity", "[si]" ) {
  SECTION("Functions can be applied in any order and still produce the same results")
  {
    CHECK(C1*L1 == L1*C1);
    CHECK(L1*V1 == V1*L1);
    CHECK(V1*C1 == C1*V1);
  }
}
TEST_CASE( "units multiplication distributivity over addition", "[si]" ) {
  SECTION("Functions can be applied in any order and still produce the same results")
  {
    CHECK(C1*(L1+L2) == C1*L1 + C1*L2);
    CHECK(L1*(V1+V2) == L1*V1 + L1*V2);
    CHECK(V1*(C1+C2) == V1*C1 + V1*C2);
  }
}




TEST_CASE( "units addition purity", "[si]" ) {
  SECTION("Calling an operation twice with the same arguments must produce the same results")
  {
    CHECK(C1+C2 == C1copy+C2copy);
    CHECK(L1+L2 == L1copy+L2copy);
    CHECK(V1+V2 == V1copy+V2copy);
  }
}
TEST_CASE( "units addition identity", "[si]" ) {
  SECTION("There exists a entry that when applied to a function returns the original entry")
  {
    CHECK(C1+0.0*C2 == C1);
    CHECK(L1+0.0*L2 == L1);
    CHECK(V1+0.0*V2 == V1);
  }
}
TEST_CASE( "units addition associativity", "[si]" ) {
  SECTION("Functions can be applied in any order and still produce the same results")
  {
    CHECK(C1+(C2+C3) == (C1+C2)+C3);
    CHECK(L1+(L2+L3) == (L1+L2)+L3);
    CHECK(V1+(V2+V3) == (V1+V2)+V3);
  }
}
TEST_CASE( "units addition invertibility", "[si]" ) {
  SECTION("For every operation there exists another operation that negates its effect")
  {
    CHECK(C1+C2-C2 == C1+C2-C2);
    CHECK(L1+L2-L2 == L1+L2-L2);
    CHECK(V1+V2-V2 == V1+V2-V2);

    CHECK(C1+C2 + -C2 == C1+C2 + -C2);
    CHECK(L1+L2 + -L2 == L1+L2 + -L2);
    CHECK(V1+V2 + -V2 == V1+V2 + -V2);
  }
}
TEST_CASE( "units addition commutativity", "[si]" ) {
  SECTION("Functions can be applied in any order and still produce the same results")
  {
    CHECK(C1+C2 == C2+C1);
    CHECK(L1+L2 == L2+L1);
    CHECK(V1+V2 == V2+V1);
  }
}