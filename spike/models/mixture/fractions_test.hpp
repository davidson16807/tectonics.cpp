
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "fractions.hpp"

TEST_CASE( "volume_fractions purity", "[mixture]" ) {
  SECTION("Calling an operation twice with the same arguments must produce the same results")
  {
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0)
        {
          CHECK(mixture::volume_fractions({i*si::meter3, j*si::meter3}) == mixture::volume_fractions({i*si::meter3, j*si::meter3}));
        }
      }
    }
  }
}
TEST_CASE( "volume_fractions identity", "[mixture]" ) {
  SECTION("There exists a entry that when applied to a function returns the original entry")
  {
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0){
          for (int k = 0; k<2; k++) {
            CHECK(mixture::volume_fractions({i*si::meter3, j*si::meter3})[k] <= 1.0);
            CHECK(0.0 <= mixture::volume_fractions({i*si::meter3, j*si::meter3})[k]);
          }
        }
      }
    }
  }
}
TEST_CASE( "volume_fractions commutativity", "[mixture]" ) {
  SECTION("Arguments can be applied in any order and still produce the same results")
  {
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0){
          CHECK(mixture::volume_fractions({i*si::meter3, j*si::meter3})[0] == mixture::volume_fractions({j*si::meter3, i*si::meter3})[1]);
          CHECK(mixture::volume_fractions({i*si::meter3, j*si::meter3})[1] == mixture::volume_fractions({j*si::meter3, i*si::meter3})[0]);
        }
      }
    }
  }
}
