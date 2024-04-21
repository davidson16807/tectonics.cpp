
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "rules.hpp"

namespace mix
{

    template<typename T>
    struct PropertyAdapter{
        const T threshold;

        PropertyAdapter(T threshold):
            threshold(threshold)
        {}

        template<typename T2>
        bool equal(const T2& a, const T2& b) const {
            const T2 denominator(si::abs(a)+si::abs(b));
            return denominator == 0.0*denominator || 2 * si::distance(a, b) / denominator < threshold;
        }
        bool equal(const double a, const double b) const {
            const double denominator(std::abs(a)+std::abs(b));
            return denominator == 0.0*denominator || 2 * std::abs(a-b) / denominator < threshold;
        }

        template<typename T2>
        std::string print(const std::vector<T2>& a) const {
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
        template<typename T2>
        std::string print(const T2& a) const {
            return si::to_string(a);
        }
        std::string print(const double& a) const {
            return std::to_string(a);
        }

    };

}

TEST_CASE( "LinearRule", "[mix]" ) {
    mix::PropertyAdapter<double> narrow (1e-6);

    std::vector<double> scalars {2.0,3.0};
    std::vector<si::length<double>> meters {2.0*si::meter,3.0*si::meter};
    auto scalar_rule = mix::linear_rule(scalars, mix::MassFractions<double>());
    auto length_rule = mix::linear_rule(meters,  mix::MassFractions<double>());
    std::vector<std::vector<si::mass<double>>> mass_vectors;
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0)
        {
          std::vector<si::mass<double>> volumes{i*si::kilogram, j*si::kilogram};
          mass_vectors.push_back(volumes);
        }
      }
    }

    REQUIRE(test::determinism(narrow,
        "scalar_rule(…)", scalar_rule,
        mass_vectors
    ));

    REQUIRE(test::codomain(narrow,
        "within expected range", [=](auto property){ return 2.0-1e-4 <= property && property <= 3.0+1e-4; },
        "scalar_rule(…)", scalar_rule,
        mass_vectors
    ));

    // REQUIRE(test::predicate(narrow,
    //     "Arguments can be applied in any order and still produce the same results", 
    //     [=](auto properties){ 
    //       std::vector<si::mass<double>> swapped {properties[1], properties[0]};
    //       return test::Results( 
    //         scalar_rule(properties) == scalar_rule(swapped), 
    //         "f(a)          : " + test::indent(narrow.print(length_rule(properties)), "  ")+"\n" +
    //         "f(reverse(a)) : " + test::indent(narrow.print(length_rule(swapped)),    "  ") );
    //     },
    //     mass_vectors
    // ));

    REQUIRE(test::determinism(narrow,
        "length_rule(…)", length_rule,
        mass_vectors
    ));

    REQUIRE(test::codomain(narrow,
        "within expected range", [=](auto property){ return (2.0-1e-4)*si::meter <= property && property <= (3.0+1e-4)*si::meter; },
        "length_rule(…)", length_rule,
        mass_vectors
    ));

    // REQUIRE(test::predicate(narrow,
    //     "Arguments can be applied in any order and still produce the same results", 
    //     [=](auto properties){ 
    //       std::vector<si::mass<double>> swapped {properties[1], properties[0]};
    //       return test::Results( length_rule(properties) == length_rule(swapped), 
    //         "f(a)          : " + test::indent(narrow.print(length_rule(properties)), "  ")+"\n" +
    //         "f(reverse(a)) : " + test::indent(narrow.print(length_rule(swapped)),    "  ") );
    //     },
    //     mass_vectors
    // ));

}

TEST_CASE( "ParallelRule", "[mix]" ) {
    mix::PropertyAdapter<double> narrow (1e-6);

    std::vector<double> scalars {2.0,3.0};
    std::vector<si::length<double>> meters {2.0*si::meter,3.0*si::meter};
    auto scalar_rule = mix::parallel_rule<double>(scalars, mix::MassFractions<double>());
    auto length_rule = mix::parallel_rule<double>(meters,  mix::MassFractions<double>());
    std::vector<std::vector<si::mass<double>>> mass_vectors;
    for (int i = 0; i<3; i++) {
      for (int j = 0; j<3; j++) {
        if (i+j>0)
        {
          std::vector<si::mass<double>> volumes{i*si::kilogram, j*si::kilogram};
          mass_vectors.push_back(volumes);
        }
      }
    }

    REQUIRE(test::determinism(narrow,
        "scalar_rule(…)", scalar_rule,
        mass_vectors
    ));

    REQUIRE(test::codomain(narrow,
        "within expected range", [=](auto property){ return 2.0-1e-4 <= property && property <= 3.0+1e-4; },
        "scalar_rule(…)", scalar_rule,
        mass_vectors
    ));

    // REQUIRE(test::predicate(narrow,
    //     "Arguments can be applied in any order and still produce the same results", 
    //     [=](auto properties){ 
    //       std::vector<si::mass<double>> swapped {properties[1], properties[0]};
    //       return test::Results( 
    //         scalar_rule(properties) == scalar_rule(swapped), 
    //         "f(a)          : " + test::indent(narrow.print(length_rule(properties)), "  ")+"\n" +
    //         "f(reverse(a)) : " + test::indent(narrow.print(length_rule(swapped)),    "  ") );
    //     },
    //     mass_vectors
    // ));

    REQUIRE(test::determinism(narrow,
        "length_rule(…)", length_rule,
        mass_vectors
    ));

    REQUIRE(test::codomain(narrow,
        "within expected range", [=](auto property){ return (2.0-1e-4)*si::meter <= property && property <= (3.0+1e-4)*si::meter; },
        "length_rule(…)", length_rule,
        mass_vectors
    ));

    // REQUIRE(test::predicate(narrow,
    //     "Arguments can be applied in any order and still produce the same results", 
    //     [=](auto properties){ 
    //       std::vector<si::mass<double>> swapped {properties[1], properties[0]};
    //       return test::Results( length_rule(properties) == length_rule(swapped), 
    //         "f(a)          : " + test::indent(narrow.print(length_rule(properties)), "  ")+"\n" +
    //         "f(reverse(a)) : " + test::indent(narrow.print(length_rule(swapped)),    "  ") );
    //     },
    //     mass_vectors
    // ));

}
