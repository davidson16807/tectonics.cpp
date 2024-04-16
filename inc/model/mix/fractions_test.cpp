
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "fractions.hpp"

#include <test/predicate.hpp>
#include <test/properties.hpp>

namespace mix{

    template<typename T>
    struct FractionsAdapter{
        T threshold;

        FractionsAdapter(const T threshold):
            threshold(threshold)
        {}

        template<typename Series1, typename Series2>
        bool equal(const Series1& a, const Series2& b) const {
          if(a.size() != b.size())
            return false;
          for (std::size_t i = 0; i<a.size(); i++) { 
            if (std::abs(a[i]-b[i]) > threshold*std::max(a[i],b[i]))
              return false; 
          }  
          return true;
        }

        template<typename Series>
        std::string print(const Series& a) const {
          std::ostringstream os;
          for (std::size_t i = 0; i<a.size(); i++) { 
            if (i!=0)
            {
              os << ", ";
            }
            os << a[i]; 
          }  
          return os.str();
        }

    };

}

TEST_CASE( "volume_fractions", "[mix]" ) {
    mix::FractionsAdapter<double> narrow (1e-6);

    std::vector<std::vector<si::volume<double>>> volume_vectors;

    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0)
        {
          std::vector<si::volume<double>> volumes{i*si::meter3, j*si::meter3};
          volume_vectors.push_back(volumes);
        }
      }
    }

    REQUIRE(test::determinism(narrow,
        "volume_fractions(…)",   [=](auto properties){ return mix::volume_fractions(properties)(properties); },
        volume_vectors
    ));

    REQUIRE(test::codomain(narrow,
        "within expected range", [=](auto properties){ for (std::size_t i = 0; i<properties.size(); i++) { if (!(0.0 <= properties[i] && properties[i] <= 1.0)) return false; }  return true; },
        "volume_fractions(…)",   [=](auto properties){ return mix::volume_fractions(properties)(properties); },
        volume_vectors
    ));

    REQUIRE(test::predicate(narrow,
        "Arguments can be applied in any order and still produce the same results", 
        [=](auto properties){ 
          std::vector<si::volume<double>> swapped {properties[1], properties[0]};
          return test::Results( 
            mix::volume_fractions(properties)(properties)[0] == mix::volume_fractions(swapped)(swapped)[1] &&
            mix::volume_fractions(properties)(properties)[1] == mix::volume_fractions(swapped)(swapped)[0], ""
          );
        },
        volume_vectors
    ));

}

TEST_CASE( "mass_fractions", "[mix]" ) {
    mix::FractionsAdapter<double> narrow (1e-6);

    std::vector<std::vector<si::mass<double>>> mass_vectors;
    std::vector<std::vector<si::density<double>>> density_vectors;

    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0)
        {
          std::vector<si::mass<double>> masses{i*si::kilogram, j*si::kilogram};
          mass_vectors.push_back(masses);
          std::vector<si::density<double>> densities{i*si::kilogram/si::meter3, j*si::kilogram/si::meter3};
          density_vectors.push_back(densities);
        }
      }
    }

    REQUIRE(test::determinism(narrow,
        "mass_fractions(…)",   [=](auto properties){ return mix::mass_fractions(properties)(properties); },
        mass_vectors
    ));
    REQUIRE(test::determinism(narrow,
        "mass_fractions(…)",   [=](auto properties){ return mix::mass_fractions(properties)(properties); },
        density_vectors
    ));

    REQUIRE(test::codomain(narrow,
        "within expected range", [=](auto properties){ for (std::size_t i = 0; i<properties.size(); i++) { if (!(0.0 <= properties[i] && properties[i] <= 1.0)) return false; }  return true; },
        "mass_fractions(…)",   [=](auto properties){ return mix::mass_fractions(properties)(properties); },
        mass_vectors
    ));
    REQUIRE(test::codomain(narrow,
        "within expected range", [=](auto properties){ for (std::size_t i = 0; i<properties.size(); i++) { if (!(0.0 <= properties[i] && properties[i] <= 1.0)) return false; }  return true; },
        "mass_fractions(…)",   [=](auto properties){ return mix::mass_fractions(properties)(properties); },
        density_vectors
    ));

    REQUIRE(test::predicate(narrow,
        "Arguments can be applied in any order and still produce the same results", 
        [=](auto properties){ 
          std::vector<si::mass<double>> swapped {properties[1], properties[0]};
          return test::Results( 
            mix::mass_fractions(properties)(properties)[0] == mix::mass_fractions(swapped)(swapped)[1] &&
            mix::mass_fractions(properties)(properties)[1] == mix::mass_fractions(swapped)(swapped)[0], ""
          );
        },
        mass_vectors
    ));
    REQUIRE(test::predicate(narrow,
        "Arguments can be applied in any order and still produce the same results", 
        [=](auto properties){ 
          std::vector<si::density<double>> swapped {properties[1], properties[0]};
          return test::Results( 
            mix::mass_fractions(properties)(properties)[0] == mix::mass_fractions(swapped)(swapped)[1] &&
            mix::mass_fractions(properties)(properties)[1] == mix::mass_fractions(swapped)(swapped)[0], ""
          );
        },
        density_vectors
    ));

}

TEST_CASE( "molar_fractions", "[mix]" ) {
    mix::FractionsAdapter<double> narrow (1e-6);

    std::vector<std::vector<si::amount<double>>> amount_vectors;
    std::vector<std::vector<si::molar_density<double>>> molar_density_vectors;
    std::vector<std::vector<si::number_density<double>>> number_density_vectors;

    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0)
        {
          std::vector<si::amount<double>> volumes{i*si::mole, j*si::mole};
          amount_vectors.push_back(volumes);
          std::vector<si::molar_density<double>> molar_densities{i*si::mole/si::meter3, j*si::mole/si::meter3};
          molar_density_vectors.push_back(molar_densities);
          std::vector<si::number_density<double>> number_densities{i/si::meter3, j/si::meter3};
          number_density_vectors.push_back(number_densities);
        }
      }
    }

    REQUIRE(test::determinism(narrow,
        "molar_fractions(…)",   [=](auto properties){ return mix::molar_fractions(properties)(properties); },
        amount_vectors
    ));
    REQUIRE(test::determinism(narrow,
        "molar_fractions(…)",   [=](auto properties){ return mix::molar_fractions(properties)(properties); },
        molar_density_vectors
    ));
    REQUIRE(test::determinism(narrow,
        "molar_fractions(…)",   [=](auto properties){ return mix::molar_fractions(properties)(properties); },
        number_density_vectors
    ));

    REQUIRE(test::codomain(narrow,
        "within expected range", [=](auto properties){ for (std::size_t i = 0; i<properties.size(); i++) { if (!(0.0 <= properties[i] && properties[i] <= 1.0)) return false; }  return true; },
        "molar_fractions(…)",   [=](auto properties){ return mix::molar_fractions(properties)(properties); },
        amount_vectors
    ));
    REQUIRE(test::codomain(narrow,
        "within expected range", [=](auto properties){ for (std::size_t i = 0; i<properties.size(); i++) { if (!(0.0 <= properties[i] && properties[i] <= 1.0)) return false; }  return true; },
        "molar_fractions(…)",   [=](auto properties){ return mix::molar_fractions(properties)(properties); },
        molar_density_vectors
    ));
    REQUIRE(test::codomain(narrow,
        "within expected range", [=](auto properties){ for (std::size_t i = 0; i<properties.size(); i++) { if (!(0.0 <= properties[i] && properties[i] <= 1.0)) return false; }  return true; },
        "molar_fractions(…)",   [=](auto properties){ return mix::molar_fractions(properties)(properties); },
        number_density_vectors
    ));

    REQUIRE(test::predicate(narrow,
        "Arguments can be applied in any order and still produce the same results", 
        [=](auto properties){ 
          std::vector<si::amount<double>> swapped {properties[1], properties[0]};
          return test::Results( 
            mix::molar_fractions(properties)(properties)[0] == mix::molar_fractions(swapped)(swapped)[1] &&
            mix::molar_fractions(properties)(properties)[1] == mix::molar_fractions(swapped)(swapped)[0], ""
          );
        },
        amount_vectors
    ));
    REQUIRE(test::predicate(narrow,
        "Arguments can be applied in any order and still produce the same results", 
        [=](auto properties){ 
          std::vector<si::molar_density<double>> swapped {properties[1], properties[0]};
          return test::Results( 
            mix::molar_fractions(properties)(properties)[0] == mix::molar_fractions(swapped)(swapped)[1] &&
            mix::molar_fractions(properties)(properties)[1] == mix::molar_fractions(swapped)(swapped)[0], ""
          );
        },
        molar_density_vectors
    ));
    REQUIRE(test::predicate(narrow,
        "Arguments can be applied in any order and still produce the same results", 
        [=](auto properties){ 
          std::vector<si::number_density<double>> swapped {properties[1], properties[0]};
          return test::Results( 
            mix::molar_fractions(properties)(properties)[0] == mix::molar_fractions(swapped)(swapped)[1] &&
            mix::molar_fractions(properties)(properties)[1] == mix::molar_fractions(swapped)(swapped)[0], ""
          );
        },
        number_density_vectors
    ));

}

