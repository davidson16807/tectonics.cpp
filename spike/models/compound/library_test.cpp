
// 3rd party libraries
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

// in house libraries
#include "library.hpp"
#include "library_test_utils.hpp"

TEST_CASE( "CompletedCompound order of magnitude", "[field]" ) {
    SECTION("Properties of compounds should not fall outside orders of magnitude for known values")
    {
        COMPLETED_COMPOUND_VALID(compound::library::water);
        COMPLETED_COMPOUND_VALID(compound::library::hydrogen);
        COMPLETED_COMPOUND_VALID(compound::library::helium);
        COMPLETED_COMPOUND_VALID(compound::library::argon);
        COMPLETED_COMPOUND_VALID(compound::library::nitrogen);
        COMPLETED_COMPOUND_VALID(compound::library::oxygen);
        COMPLETED_COMPOUND_VALID(compound::library::ammonia);
        COMPLETED_COMPOUND_VALID(compound::library::ozone);
        COMPLETED_COMPOUND_VALID(compound::library::sulfur_dioxide);
        COMPLETED_COMPOUND_VALID(compound::library::nitrous_oxide);
        COMPLETED_COMPOUND_VALID(compound::library::nitric_oxide);
        COMPLETED_COMPOUND_VALID(compound::library::carbon_dioxide);
        COMPLETED_COMPOUND_VALID(compound::library::carbon_monoxide);
        COMPLETED_COMPOUND_VALID(compound::library::hydrogen_cyanide);
        COMPLETED_COMPOUND_VALID(compound::library::methane);
        COMPLETED_COMPOUND_VALID(compound::library::perflouromethane);
        COMPLETED_COMPOUND_VALID(compound::library::formaldehyde);
        COMPLETED_COMPOUND_VALID(compound::library::formic_acid);
        COMPLETED_COMPOUND_VALID(compound::library::benzene);
        COMPLETED_COMPOUND_VALID(compound::library::pyrimidine);
        COMPLETED_COMPOUND_VALID(compound::library::ethane);
        COMPLETED_COMPOUND_VALID(compound::library::ethanol);
        COMPLETED_COMPOUND_VALID(compound::library::quartz);
        COMPLETED_COMPOUND_VALID(compound::library::corundum);
        COMPLETED_COMPOUND_VALID(compound::library::carbon);
        COMPLETED_COMPOUND_VALID(compound::library::orthoclase);
        COMPLETED_COMPOUND_VALID(compound::library::andesine);
        COMPLETED_COMPOUND_VALID(compound::library::augite);
        COMPLETED_COMPOUND_VALID(compound::library::forsterite);
        COMPLETED_COMPOUND_VALID(compound::library::hematite);
        COMPLETED_COMPOUND_VALID(compound::library::goethite);
        COMPLETED_COMPOUND_VALID(compound::library::magnetite);
        COMPLETED_COMPOUND_VALID(compound::library::pyrite);
        COMPLETED_COMPOUND_VALID(compound::library::chalcocite);
        COMPLETED_COMPOUND_VALID(compound::library::chalcopyrite);
        COMPLETED_COMPOUND_VALID(compound::library::copper);
        COMPLETED_COMPOUND_VALID(compound::library::silver);
        COMPLETED_COMPOUND_VALID(compound::library::gold);
        COMPLETED_COMPOUND_VALID(compound::library::halite);
        COMPLETED_COMPOUND_VALID(compound::library::calcite);
        COMPLETED_COMPOUND_VALID(compound::library::apatite);
    }
}
