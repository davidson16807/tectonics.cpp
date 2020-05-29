
#include <cstddef>

#include <array>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <models/genes/constituents/Photopigment.hpp>
#include <models/genes/constituents/ClosedFluidSystemConstituent.hpp>
#include <models/genes/structures/TubeStructure.hpp>
#include <models/genes/structures/CorneousStructure.hpp>
#include <models/genes/structures/PhotoreceptorStructure.hpp>
#include <models/genes/structures/PressureSensingStructure.hpp>
#include <models/genes/structures/SurfaceStructure.hpp>
#include <models/genes/materials/ClosedFluidSystemComposition.hpp>
#include <models/genes/materials/Mineralization.hpp>
#include <models/genes/materials/PhotoreceptorMaterials.hpp>
#include <models/genes/materials/Pigmentation.hpp>
#include <models/genes/segments/GenericSegment.hpp>
#include <models/genes/segments/AppendageSegment.hpp>
#include <models/genes/Appendage.hpp>
#include <models/genes/segments/BodySegment.hpp>
// #include <models/genes/Body.hpp>

using namespace genes;

TEST_CASE( "fraction encode/decode invertibility", "[many]" ) {
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        CHECK(encode_fraction(decode_fraction(0x0))==0x0);
        CHECK(encode_fraction(decode_fraction(0x1))==0x1);
        CHECK(encode_fraction(decode_fraction(0xD))==0xD);
        CHECK(encode_fraction(decode_fraction(0xF))==0xF);
    }
}

TEST_CASE( "portion encode/decode invertibility", "[many]" ) {
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        CHECK(encode_portion(decode_portion(0x0))==0x0);
        CHECK(encode_portion(decode_portion(0x1))==0x1);
        CHECK(encode_portion(decode_portion(0xD))==0xD);
        CHECK(encode_portion(decode_portion(0xF))==0xF);
    }
}

TEST_CASE( "range encode/decode invertibility", "[many]" ) {
    const float lo = 1.6;
    const float hi = 80.7;
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        CHECK(encode_ranged(decode_ranged(0x0, lo, hi), lo, hi)==0x0);
        CHECK(encode_ranged(decode_ranged(0x1, lo, hi), lo, hi)==0x1);
        CHECK(encode_ranged(decode_ranged(0xD, lo, hi), lo, hi)==0xD);
        CHECK(encode_ranged(decode_ranged(0xF, lo, hi), lo, hi)==0xF);
    }
}



TEST_CASE( "fraction encode/decode clamp behavior", "[many]" ) {
    SECTION("decoding an overflown value then reencoding it must indicate clamping behavior"){
        CHECK(encode_fraction(decode_fraction(-0x1))==0x0);
        CHECK(encode_fraction(decode_fraction(0x10))==0xF);
    }
}

TEST_CASE( "portion encode/decode clamp behavior", "[many]" ) {
    SECTION("decoding an overflown value then reencoding it must indicate clamping behavior"){
        CHECK(encode_portion(decode_portion(-0x1))==0x0);
        CHECK(encode_portion(decode_portion(0x10))==0xF);
    }
}

TEST_CASE( "range encode/decode clamp behavior", "[many]" ) {
    const float lo = 1.6;
    const float hi = 80.7;
    SECTION("decoding an overflown value then reencoding it must indicate clamping behavior"){
        CHECK(encode_ranged(decode_ranged(-0x1, lo, hi), lo, hi)==0x0);
        CHECK(encode_ranged(decode_ranged(0x10, lo, hi), lo, hi)==0xF);
    }
}




TEST_CASE( "Photopigment encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, Photopigment::bit_count/4> original;
    std::array<std::int16_t, Photopigment::bit_count/4> reconstituted1;
    std::array<std::int16_t, Photopigment::bit_count/4> reconstituted2;
    std::array<std::int16_t, Photopigment::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Photopigment photopigment1;
    Photopigment photopigment2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        photopigment1.decode(original.begin());
        photopigment1.encode(reconstituted1.begin());
        photopigment1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        photopigment2.decode(original.begin());
        photopigment2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
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

TEST_CASE( "Photopigment static method consistency", "[many]" ) {
    std::array<std::int8_t, Photopigment::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, Photopigment::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    Photopigment::getMutationRates(mutation_rates.begin());
    Photopigment::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}







TEST_CASE( "ClosedFluidSystemConstituent encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, ClosedFluidSystemConstituent::bit_count/4> original;
    std::array<std::int16_t, ClosedFluidSystemConstituent::bit_count/4> reconstituted1;
    std::array<std::int16_t, ClosedFluidSystemConstituent::bit_count/4> reconstituted2;
    std::array<std::int16_t, ClosedFluidSystemConstituent::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    ClosedFluidSystemConstituent constituent1;
    ClosedFluidSystemConstituent constituent2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        constituent1.decode(original.begin());
        constituent1.encode(reconstituted1.begin());
        constituent1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        constituent2.decode(original.begin());
        constituent2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
    }
}

TEST_CASE( "ClosedFluidSystemConstituent encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, ClosedFluidSystemConstituent::bit_count/4> original;
    std::array<std::int16_t, ClosedFluidSystemConstituent::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    ClosedFluidSystemConstituent constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "ClosedFluidSystemConstituent static method consistency", "[many]" ) {
    std::array<std::int8_t, ClosedFluidSystemConstituent::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, ClosedFluidSystemConstituent::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    ClosedFluidSystemConstituent::getMutationRates(mutation_rates.begin());
    ClosedFluidSystemConstituent::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}






TEST_CASE( "TubeStructure encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, TubeStructure::bit_count/4> original;
    std::array<std::int16_t, TubeStructure::bit_count/4> reconstituted1;
    std::array<std::int16_t, TubeStructure::bit_count/4> reconstituted2;
    std::array<std::int16_t, TubeStructure::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    TubeStructure constituent1;
    TubeStructure constituent2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        constituent1.decode(original.begin());
        constituent1.encode(reconstituted1.begin());
        constituent1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        constituent2.decode(original.begin());
        constituent2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
    }
}

TEST_CASE( "TubeStructure encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, TubeStructure::bit_count/4> original;
    std::array<std::int16_t, TubeStructure::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    TubeStructure constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "TubeStructure static method consistency", "[many]" ) {
    std::array<std::int8_t, TubeStructure::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, TubeStructure::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    TubeStructure::getMutationRates(mutation_rates.begin());
    TubeStructure::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}






TEST_CASE( "CorneousStructure encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, CorneousStructure::bit_count/4> original;
    std::array<std::int16_t, CorneousStructure::bit_count/4> reconstituted1;
    std::array<std::int16_t, CorneousStructure::bit_count/4> reconstituted2;
    std::array<std::int16_t, CorneousStructure::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    CorneousStructure constituent1;
    CorneousStructure constituent2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        constituent1.decode(original.begin());
        constituent1.encode(reconstituted1.begin());
        constituent1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        constituent2.decode(original.begin());
        constituent2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
    }
}

TEST_CASE( "CorneousStructure encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, CorneousStructure::bit_count/4> original;
    std::array<std::int16_t, CorneousStructure::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    CorneousStructure constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "CorneousStructure static method consistency", "[many]" ) {
    std::array<std::int8_t, CorneousStructure::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, CorneousStructure::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    CorneousStructure::getMutationRates(mutation_rates.begin());
    CorneousStructure::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}






TEST_CASE( "PhotoreceptorStructure encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, PhotoreceptorStructure::bit_count/4> original;
    std::array<std::int16_t, PhotoreceptorStructure::bit_count/4> reconstituted1;
    std::array<std::int16_t, PhotoreceptorStructure::bit_count/4> reconstituted2;
    std::array<std::int16_t, PhotoreceptorStructure::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    PhotoreceptorStructure constituent1;
    PhotoreceptorStructure constituent2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        constituent1.decode(original.begin());
        constituent1.encode(reconstituted1.begin());
        constituent1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        constituent2.decode(original.begin());
        constituent2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
    }
}

TEST_CASE( "PhotoreceptorStructure encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, PhotoreceptorStructure::bit_count/4> original;
    std::array<std::int16_t, PhotoreceptorStructure::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    PhotoreceptorStructure constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "PhotoreceptorStructure static method consistency", "[many]" ) {
    std::array<std::int8_t, PhotoreceptorStructure::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, PhotoreceptorStructure::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    PhotoreceptorStructure::getMutationRates(mutation_rates.begin());
    PhotoreceptorStructure::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}






TEST_CASE( "PressureSensingStructure encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, PressureSensingStructure::bit_count/4> original;
    std::array<std::int16_t, PressureSensingStructure::bit_count/4> reconstituted1;
    std::array<std::int16_t, PressureSensingStructure::bit_count/4> reconstituted2;
    std::array<std::int16_t, PressureSensingStructure::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    PressureSensingStructure constituent1;
    PressureSensingStructure constituent2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        constituent1.decode(original.begin());
        constituent1.encode(reconstituted1.begin());
        constituent1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        constituent2.decode(original.begin());
        constituent2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
    }
}

TEST_CASE( "PressureSensingStructure encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, PressureSensingStructure::bit_count/4> original;
    std::array<std::int16_t, PressureSensingStructure::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    PressureSensingStructure constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "PressureSensingStructure static method consistency", "[many]" ) {
    std::array<std::int8_t, PressureSensingStructure::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, PressureSensingStructure::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    PressureSensingStructure::getMutationRates(mutation_rates.begin());
    PressureSensingStructure::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}






TEST_CASE( "SurfaceStructure encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, SurfaceStructure::bit_count/4> original;
    std::array<std::int16_t, SurfaceStructure::bit_count/4> reconstituted1;
    std::array<std::int16_t, SurfaceStructure::bit_count/4> reconstituted2;
    std::array<std::int16_t, SurfaceStructure::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    SurfaceStructure constituent1;
    SurfaceStructure constituent2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        constituent1.decode(original.begin());
        constituent1.encode(reconstituted1.begin());
        constituent1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        constituent2.decode(original.begin());
        constituent2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
    }
}

TEST_CASE( "SurfaceStructure encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, SurfaceStructure::bit_count/4> original;
    std::array<std::int16_t, SurfaceStructure::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    SurfaceStructure constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "SurfaceStructure static method consistency", "[many]" ) {
    std::array<std::int8_t, SurfaceStructure::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, SurfaceStructure::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    SurfaceStructure::getMutationRates(mutation_rates.begin());
    SurfaceStructure::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}







TEST_CASE( "PhotoreceptorMaterials encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, PhotoreceptorMaterials::bit_count/4> original;
    std::array<std::int16_t, PhotoreceptorMaterials::bit_count/4> reconstituted1;
    std::array<std::int16_t, PhotoreceptorMaterials::bit_count/4> reconstituted2;
    std::array<std::int16_t, PhotoreceptorMaterials::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    PhotoreceptorMaterials constituent1;
    PhotoreceptorMaterials constituent2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        constituent1.decode(original.begin());
        constituent1.encode(reconstituted1.begin());
        constituent1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        constituent2.decode(original.begin());
        constituent2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
    }
}

TEST_CASE( "PhotoreceptorMaterials encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, PhotoreceptorMaterials::bit_count/4> original;
    std::array<std::int16_t, PhotoreceptorMaterials::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    PhotoreceptorMaterials constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "PhotoreceptorMaterials static method consistency", "[many]" ) {
    std::array<std::int8_t, PhotoreceptorMaterials::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, PhotoreceptorMaterials::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    PhotoreceptorMaterials::getMutationRates(mutation_rates.begin());
    PhotoreceptorMaterials::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}







TEST_CASE( "ClosedFluidSystemComposition encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, ClosedFluidSystemComposition::bit_count/4> original;
    std::array<std::int16_t, ClosedFluidSystemComposition::bit_count/4> reconstituted1;
    std::array<std::int16_t, ClosedFluidSystemComposition::bit_count/4> reconstituted2;
    std::array<std::int16_t, ClosedFluidSystemComposition::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    ClosedFluidSystemComposition constituent1;
    ClosedFluidSystemComposition constituent2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        constituent1.decode(original.begin());
        constituent1.encode(reconstituted1.begin());
        constituent1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        constituent2.decode(original.begin());
        constituent2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
    }
}

TEST_CASE( "ClosedFluidSystemComposition encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, ClosedFluidSystemComposition::bit_count/4> original;
    std::array<std::int16_t, ClosedFluidSystemComposition::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    ClosedFluidSystemComposition constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "ClosedFluidSystemComposition static method consistency", "[many]" ) {
    std::array<std::int8_t, ClosedFluidSystemComposition::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, ClosedFluidSystemComposition::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    ClosedFluidSystemComposition::getMutationRates(mutation_rates.begin());
    ClosedFluidSystemComposition::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}








TEST_CASE( "Mineralization encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, Mineralization::bit_count/4> original;
    std::array<std::int16_t, Mineralization::bit_count/4> reconstituted1;
    std::array<std::int16_t, Mineralization::bit_count/4> reconstituted2;
    std::array<std::int16_t, Mineralization::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Mineralization constituent1;
    Mineralization constituent2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        constituent1.decode(original.begin());
        constituent1.encode(reconstituted1.begin());
        constituent1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        constituent2.decode(original.begin());
        constituent2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
    }
}

TEST_CASE( "Mineralization encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, Mineralization::bit_count/4> original;
    std::array<std::int16_t, Mineralization::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Mineralization constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "Mineralization static method consistency", "[many]" ) {
    std::array<std::int8_t, Mineralization::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, Mineralization::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    Mineralization::getMutationRates(mutation_rates.begin());
    Mineralization::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}







TEST_CASE( "Pigmentation encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, Pigmentation::bit_count/4> original;
    std::array<std::int16_t, Pigmentation::bit_count/4> reconstituted1;
    std::array<std::int16_t, Pigmentation::bit_count/4> reconstituted2;
    std::array<std::int16_t, Pigmentation::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Pigmentation constituent1;
    Pigmentation constituent2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        constituent1.decode(original.begin());
        constituent1.encode(reconstituted1.begin());
        constituent1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        constituent2.decode(original.begin());
        constituent2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
    }
}

TEST_CASE( "Pigmentation encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, Pigmentation::bit_count/4> original;
    std::array<std::int16_t, Pigmentation::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Pigmentation constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "Pigmentation static method consistency", "[many]" ) {
    std::array<std::int8_t, Pigmentation::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, Pigmentation::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    Pigmentation::getMutationRates(mutation_rates.begin());
    Pigmentation::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}





TEST_CASE( "GenericSegment encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, GenericSegment::bit_count/4> original;
    std::array<std::int16_t, GenericSegment::bit_count/4> reconstituted1;
    std::array<std::int16_t, GenericSegment::bit_count/4> reconstituted2;
    std::array<std::int16_t, GenericSegment::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    GenericSegment constituent1;
    GenericSegment constituent2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        constituent1.decode(original.begin());
        constituent1.encode(reconstituted1.begin());
        constituent1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        constituent2.decode(original.begin());
        constituent2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
    }
}

TEST_CASE( "GenericSegment encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, GenericSegment::bit_count/4> original;
    std::array<std::int16_t, GenericSegment::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    GenericSegment constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "GenericSegment static method consistency", "[many]" ) {
    std::array<std::int8_t, GenericSegment::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, GenericSegment::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    GenericSegment::getMutationRates(mutation_rates.begin());
    GenericSegment::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}





TEST_CASE( "AppendageSegment encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, AppendageSegment::bit_count/4> original;
    std::array<std::int16_t, AppendageSegment::bit_count/4> reconstituted1;
    std::array<std::int16_t, AppendageSegment::bit_count/4> reconstituted2;
    std::array<std::int16_t, AppendageSegment::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    AppendageSegment constituent1;
    AppendageSegment constituent2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        constituent1.decode(original.begin());
        constituent1.encode(reconstituted1.begin());
        constituent1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        constituent2.decode(original.begin());
        constituent2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
    }
}

TEST_CASE( "AppendageSegment encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, AppendageSegment::bit_count/4> original;
    std::array<std::int16_t, AppendageSegment::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    AppendageSegment constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "AppendageSegment static method consistency", "[many]" ) {
    std::array<std::int8_t, AppendageSegment::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, AppendageSegment::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    AppendageSegment::getMutationRates(mutation_rates.begin());
    AppendageSegment::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}





TEST_CASE( "Appendage encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, Appendage::bit_count/4> original;
    std::array<std::int16_t, Appendage::bit_count/4> reconstituted1;
    std::array<std::int16_t, Appendage::bit_count/4> reconstituted2;
    std::array<std::int16_t, Appendage::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Appendage constituent1;
    Appendage constituent2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        constituent1.decode(original.begin());
        constituent1.encode(reconstituted1.begin());
        constituent1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        constituent2.decode(original.begin());
        constituent2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
    }
}

TEST_CASE( "Appendage encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, Appendage::bit_count/4> original;
    std::array<std::int16_t, Appendage::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Appendage constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "Appendage static method consistency", "[many]" ) {
    std::array<std::int8_t, Appendage::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, Appendage::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    Appendage::getMutationRates(mutation_rates.begin());
    Appendage::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}





TEST_CASE( "BodySegment encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, BodySegment::bit_count/4> original;
    std::array<std::int16_t, BodySegment::bit_count/4> reconstituted1;
    std::array<std::int16_t, BodySegment::bit_count/4> reconstituted2;
    std::array<std::int16_t, BodySegment::bit_count/4> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    BodySegment constituent1;
    BodySegment constituent2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        constituent1.decode(original.begin());
        constituent1.encode(reconstituted1.begin());
        constituent1.encode(reconstituted2.begin());
        CHECK(reconstituted1==reconstituted2);
        constituent2.decode(original.begin());
        constituent2.encode(reconstituted3.begin());
        CHECK(reconstituted1==reconstituted3);
    }
}

TEST_CASE( "BodySegment encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, BodySegment::bit_count/4> original;
    std::array<std::int16_t, BodySegment::bit_count/4> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    BodySegment constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "BodySegment static method consistency", "[many]" ) {
    std::array<std::int8_t, BodySegment::bit_count/4+1> mutation_rates;
    std::array<std::int8_t, BodySegment::bit_count/4+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    BodySegment::getMutationRates(mutation_rates.begin());
    BodySegment::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}










// TEST_CASE( "Body encode/decode regularity", "[many]" ) {
//     std::array<std::int16_t, Body::bit_count/4> original;
//     std::array<std::int16_t, Body::bit_count/4> reconstituted1;
//     std::array<std::int16_t, Body::bit_count/4> reconstituted2;
//     std::array<std::int16_t, Body::bit_count/4> reconstituted3;
//     int count = 0;  
//     std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
//     Body constituent1;
//     Body constituent2;

//     SECTION("decode and encode must be called repeatedly without changing the value of output references"){
//         constituent1.decode(original.begin());
//         constituent1.encode(reconstituted1.begin());
//         constituent1.encode(reconstituted2.begin());
//         CHECK(reconstituted1==reconstituted2);
//         constituent2.decode(original.begin());
//         constituent2.encode(reconstituted3.begin());
//         CHECK(reconstituted1==reconstituted3);
//     }
// }

// TEST_CASE( "Body encode/decode invertibility", "[many]" ) {
//     std::array<std::int16_t, Body::bit_count/4> original;
//     std::array<std::int16_t, Body::bit_count/4> reconstituted;
//     int count = 0;  
//     std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
//     Body constituent;
 
//     SECTION("decoding an array then reencoding it must reproduce the original array"){
//         constituent.decode(original.begin());
//         constituent.encode(reconstituted.begin());
//         CHECK(original==reconstituted);
//     }
// }

// TEST_CASE( "Body static method consistency", "[many]" ) {
//     std::array<std::int8_t, Body::bit_count/4+1> mutation_rates;
//     std::array<std::int8_t, Body::bit_count/4+1> attribute_sizes;
//     std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
//     std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
//     Body::getMutationRates(mutation_rates.begin());
//     Body::getAttributeSizes(attribute_sizes.begin());
//     SECTION("mutation rates and attribute sizes must have the same count"){
//         CHECK(mutation_rates.end()[-2] > -1);
//         CHECK(mutation_rates.end()[-1] == -1);
//         CHECK(attribute_sizes.end()[-2] > -1);
//         CHECK(attribute_sizes.end()[-1] == -1);
//     }
// }




