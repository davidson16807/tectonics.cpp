
#include <cstddef>

#include <array>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <models/genes/constituents/Photopigment.hpp>

using namespace genes;


// TEST_CASE( "Photopigment static method consistency", "[many]" ) {
// }
// TEST_CASE( "Photopigment nontriviality", "[many]" ) {
// }
// TEST_CASE( "Photopigment purity", "[many]" ) {
//     vector<int> dense( Photopigment::attribute_count );
//     int count = 0;  
//     generate(dense.begin(), dense.end(), [&](){ return count=(count+1)%0xF; });
//     Photopigment photopigment1;
//     Photopigment photopigment2;

//     SECTION("Photopigment.decode(dense.begin()) must be called repeatedly without changing the output"){
//         photopigment1.decode(dense.begin());
//         photopigment2.decode(dense.begin());
//         CHECK(photopigment1==photopigment2);

//         add(a,b,c1);
//         add(a,b,c2);
//         CHECK(c1==c2);
//     }
// }

TEST_CASE( "fraction encode/decode invertibility", "[many]" ) {
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        CHECK(encode_fraction(decode_fraction(0x0))==0x0);
        CHECK(encode_fraction(decode_fraction(0x1))==0x1);
        CHECK(encode_fraction(decode_fraction(0xD))==0xD);
        CHECK(encode_fraction(decode_fraction(0xF))==0xF);
    }
}

TEST_CASE( "Photopigment encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, Photopigment::bit_count/4> original;
    std::array<std::int16_t, Photopigment::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Photopigment photopigment;

    SECTION("decoding an array then reencoding it must reproduce the original array"){
        photopigment.decode(original.begin());
        photopigment.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

