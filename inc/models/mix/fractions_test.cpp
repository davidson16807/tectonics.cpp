
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "fractions.hpp"

TEST_CASE( "volume_fractions purity", "[mix]" ) {
  SECTION("Calling an operation twice with the same arguments must produce the same results")
  {
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0)
        {
          CHECK(mix::volume_fractions({i*si::meter3, j*si::meter3}) == mix::volume_fractions({i*si::meter3, j*si::meter3}));
        }
      }
    }
  }
}
TEST_CASE( "volume_fractions codomain", "[mix]" ) {
  SECTION("Output must fall within a known range")
  {
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0){
          for (int k = 0; k<2; k++) {
            CHECK(mix::volume_fractions({i*si::meter3, j*si::meter3})[k] <= 1.0);
            CHECK(0.0 <= mix::volume_fractions({i*si::meter3, j*si::meter3})[k]);
          }
        }
      }
    }
  }
}
TEST_CASE( "volume_fractions commutativity", "[mix]" ) {
  SECTION("Arguments can be applied in any order and still produce the same results")
  {
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0){
          CHECK(mix::volume_fractions({i*si::meter3, j*si::meter3})[0] == mix::volume_fractions({j*si::meter3, i*si::meter3})[1]);
          CHECK(mix::volume_fractions({i*si::meter3, j*si::meter3})[1] == mix::volume_fractions({j*si::meter3, i*si::meter3})[0]);
        }
      }
    }
  }
}

TEST_CASE( "mass_fractions purity", "[mix]" ) {
  SECTION("Calling an operation twice with the same arguments must produce the same results")
  {
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0)
        {
          CHECK(mix::mass_fractions(std::vector<si::mass<double>>{i*si::kilogram, j*si::kilogram}) == mix::mass_fractions(std::vector<si::mass<double>>{i*si::kilogram, j*si::kilogram}));

          CHECK(mix::mass_fractions(std::vector<si::density<double>>{i*si::kilogram/si::meter3, j*si::kilogram/si::meter3}) == mix::mass_fractions(std::vector<si::density<double>>{i*si::kilogram/si::meter3, j*si::kilogram/si::meter3}));
        }
      }
    }
  }
}
TEST_CASE( "mass_fractions codomain", "[mix]" ) {
  SECTION("Output must fall within a known range")
  {
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0){
          for (int k = 0; k<2; k++) {
            CHECK(mix::mass_fractions(std::vector<si::mass<double>>{i*si::kilogram, j*si::kilogram})[k] <= 1.0);
            CHECK(0.0 <= mix::mass_fractions(std::vector<si::mass<double>>{i*si::kilogram, j*si::kilogram})[k]);

            CHECK(mix::mass_fractions(std::vector<si::density<double>>{i*si::kilogram/si::meter3, j*si::kilogram/si::meter3})[k] <= 1.0);
            CHECK(0.0 <= mix::mass_fractions(std::vector<si::density<double>>{i*si::kilogram/si::meter3, j*si::kilogram/si::meter3})[k]);
          }
        }
      }
    }
  }
}
TEST_CASE( "mass_fractions commutativity", "[mix]" ) {
  SECTION("Arguments can be applied in any order and still produce the same results")
  {
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0){
          CHECK(mix::mass_fractions(std::vector<si::mass<double>>{i*si::kilogram, j*si::kilogram})[0] == mix::mass_fractions(std::vector<si::mass<double>>{j*si::kilogram, i*si::kilogram})[1]);
          CHECK(mix::mass_fractions(std::vector<si::mass<double>>{i*si::kilogram, j*si::kilogram})[1] == mix::mass_fractions(std::vector<si::mass<double>>{j*si::kilogram, i*si::kilogram})[0]);

          CHECK(mix::mass_fractions(std::vector<si::density<double>>{i*si::kilogram/si::meter3, j*si::kilogram/si::meter3})[0] == mix::mass_fractions(std::vector<si::density<double>>{j*si::kilogram/si::meter3, i*si::kilogram/si::meter3})[1]);
          CHECK(mix::mass_fractions(std::vector<si::density<double>>{i*si::kilogram/si::meter3, j*si::kilogram/si::meter3})[1] == mix::mass_fractions(std::vector<si::density<double>>{j*si::kilogram/si::meter3, i*si::kilogram/si::meter3})[0]);
        }
      }
    }
  }
}


TEST_CASE( "molar_fractions purity", "[mix]" ) {
  SECTION("Calling an operation twice with the same arguments must produce the same results")
  {
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0)
        {
          CHECK(mix::molar_fractions(std::vector<si::amount<double>>{i*si::mole, j*si::mole}) == mix::molar_fractions(std::vector<si::amount<double>>{i*si::mole, j*si::mole}));

          CHECK(mix::molar_fractions(std::vector<si::molar_density<double>>{i*si::mole/si::meter3, j*si::mole/si::meter3}) == mix::molar_fractions(std::vector<si::molar_density<double>>{i*si::mole/si::meter3, j*si::mole/si::meter3}));

          CHECK(mix::molar_fractions(std::vector<si::number_density<double>>{i/si::meter3, j/si::meter3}) == mix::molar_fractions(std::vector<si::number_density<double>>{i/si::meter3, j/si::meter3}));
        }
      }
    }
  }
}
TEST_CASE( "molar_fractions codomain", "[mix]" ) {
  SECTION("Output must fall within a known range")
  {
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0){
          for (int k = 0; k<2; k++) {
            CHECK(mix::molar_fractions(std::vector<si::amount<double>>{i*si::mole, j*si::mole})[k] <= 1.0);
            CHECK(0.0 <= mix::molar_fractions(std::vector<si::amount<double>>{i*si::mole, j*si::mole})[k]);

            CHECK(mix::molar_fractions(std::vector<si::molar_density<double>>{i*si::mole/si::meter3, j*si::mole/si::meter3})[k] <= 1.0);
            CHECK(0.0 <= mix::molar_fractions(std::vector<si::molar_density<double>>{i*si::mole/si::meter3, j*si::mole/si::meter3})[k]);

            CHECK(mix::molar_fractions(std::vector<si::number_density<double>>{i/si::meter3, j/si::meter3})[k] <= 1.0);
            CHECK(0.0 <= mix::molar_fractions(std::vector<si::number_density<double>>{i/si::meter3, j/si::meter3})[k]);
          }
        }
      }
    }
  }
}
TEST_CASE( "molar_fractions commutativity", "[mix]" ) {
  SECTION("Arguments can be applied in any order and still produce the same results")
  {
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0){
          CHECK(mix::molar_fractions(std::vector<si::amount<double>>{i*si::mole, j*si::mole})[0] == mix::molar_fractions(std::vector<si::amount<double>>{j*si::mole, i*si::mole})[1]);
          CHECK(mix::molar_fractions(std::vector<si::amount<double>>{i*si::mole, j*si::mole})[1] == mix::molar_fractions(std::vector<si::amount<double>>{j*si::mole, i*si::mole})[0]);

          CHECK(mix::molar_fractions(std::vector<si::molar_density<double>>{i*si::mole/si::meter3, j*si::mole/si::meter3})[0] == mix::molar_fractions(std::vector<si::molar_density<double>>{j*si::mole/si::meter3, i*si::mole/si::meter3})[1]);
          CHECK(mix::molar_fractions(std::vector<si::molar_density<double>>{i*si::mole/si::meter3, j*si::mole/si::meter3})[1] == mix::molar_fractions(std::vector<si::molar_density<double>>{j*si::mole/si::meter3, i*si::mole/si::meter3})[0]);

          CHECK(mix::molar_fractions(std::vector<si::number_density<double>>{i/si::meter3, j/si::meter3})[0] == mix::molar_fractions(std::vector<si::number_density<double>>{j/si::meter3, i/si::meter3})[1]);
          CHECK(mix::molar_fractions(std::vector<si::number_density<double>>{i/si::meter3, j/si::meter3})[1] == mix::molar_fractions(std::vector<si::number_density<double>>{j/si::meter3, i/si::meter3})[0]);
        }
      }
    }
  }
}
