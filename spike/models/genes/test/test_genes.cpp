
#include <cstddef>

#include <iostream>
#include <array>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>

#include <models/genes/constituents/Photopigment.hpp>
#include <models/genes/constituents/ClosedFluidSystemConstituent.hpp>
#include <models/genes/constituents/Endosymbiont.hpp>
#include <models/genes/structures/TubeStructure.hpp>
#include <models/genes/structures/CorneousStructure.hpp>
#include <models/genes/structures/PhotoreceptorStructure.hpp>
#include <models/genes/structures/PressureSensingStructure.hpp>
#include <models/genes/structures/SurfaceStructure.hpp>
#include <models/genes/materials/ClosedFluidSystemComposition.hpp>
#include <models/genes/materials/Mineralization.hpp>
#include <models/genes/materials/PhotoreceptorMaterials.hpp>
#include <models/genes/materials/Pigmentation.hpp>
#include <models/genes/materials/Toxins.hpp>
#include <models/genes/materials/CellSignals.hpp>
#include <models/genes/segments/GenericSegment.hpp>
#include <models/genes/segments/AppendageSegment.hpp>
#include <models/genes/Appendage.hpp>
#include <models/genes/segments/BodySegment.hpp>
#include <models/genes/Body.hpp>
#include <models/genes/Genome.hpp>

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
    std::array<std::int16_t, Photopigment::attribute_count> original;
    std::array<std::int16_t, Photopigment::attribute_count> reconstituted1;
    std::array<std::int16_t, Photopigment::attribute_count> reconstituted2;
    std::array<std::int16_t, Photopigment::attribute_count> reconstituted3;
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
    std::array<std::int16_t, Photopigment::attribute_count> original;
    std::array<std::int16_t, Photopigment::attribute_count> reconstituted;
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
    std::array<std::int8_t, Photopigment::attribute_count+1> mutation_rates;
    std::array<std::int8_t, Photopigment::attribute_count+1> attribute_sizes;
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
    std::array<std::int16_t, ClosedFluidSystemConstituent::attribute_count> original;
    std::array<std::int16_t, ClosedFluidSystemConstituent::attribute_count> reconstituted1;
    std::array<std::int16_t, ClosedFluidSystemConstituent::attribute_count> reconstituted2;
    std::array<std::int16_t, ClosedFluidSystemConstituent::attribute_count> reconstituted3;
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
    std::array<std::int16_t, ClosedFluidSystemConstituent::attribute_count> original;
    std::array<std::int16_t, ClosedFluidSystemConstituent::attribute_count> reconstituted;
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
    std::array<std::int8_t, ClosedFluidSystemConstituent::attribute_count+1> mutation_rates;
    std::array<std::int8_t, ClosedFluidSystemConstituent::attribute_count+1> attribute_sizes;
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






TEST_CASE( "Endosymbiont encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, Endosymbiont::attribute_count> original;
    std::array<std::int16_t, Endosymbiont::attribute_count> reconstituted1;
    std::array<std::int16_t, Endosymbiont::attribute_count> reconstituted2;
    std::array<std::int16_t, Endosymbiont::attribute_count> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Endosymbiont constituent1;
    Endosymbiont constituent2;

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

TEST_CASE( "Endosymbiont encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, Endosymbiont::attribute_count> original;
    std::array<std::int16_t, Endosymbiont::attribute_count> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Endosymbiont constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "Endosymbiont static method consistency", "[many]" ) {
    std::array<std::int8_t, Endosymbiont::attribute_count+1> mutation_rates;
    std::array<std::int8_t, Endosymbiont::attribute_count+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    Endosymbiont::getMutationRates(mutation_rates.begin());
    Endosymbiont::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}






TEST_CASE( "TubeStructure encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, TubeStructure::attribute_count> original;
    std::array<std::int16_t, TubeStructure::attribute_count> reconstituted1;
    std::array<std::int16_t, TubeStructure::attribute_count> reconstituted2;
    std::array<std::int16_t, TubeStructure::attribute_count> reconstituted3;
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
    std::array<std::int16_t, TubeStructure::attribute_count> original;
    std::array<std::int16_t, TubeStructure::attribute_count> reconstituted;
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
    std::array<std::int8_t, TubeStructure::attribute_count+1> mutation_rates;
    std::array<std::int8_t, TubeStructure::attribute_count+1> attribute_sizes;
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
    std::array<std::int16_t, CorneousStructure::attribute_count> original;
    std::array<std::int16_t, CorneousStructure::attribute_count> reconstituted1;
    std::array<std::int16_t, CorneousStructure::attribute_count> reconstituted2;
    std::array<std::int16_t, CorneousStructure::attribute_count> reconstituted3;
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
    std::array<std::int16_t, CorneousStructure::attribute_count> original;
    std::array<std::int16_t, CorneousStructure::attribute_count> reconstituted;
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
    std::array<std::int8_t, CorneousStructure::attribute_count+1> mutation_rates;
    std::array<std::int8_t, CorneousStructure::attribute_count+1> attribute_sizes;
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
    std::array<std::int16_t, PhotoreceptorStructure::attribute_count> original;
    std::array<std::int16_t, PhotoreceptorStructure::attribute_count> reconstituted1;
    std::array<std::int16_t, PhotoreceptorStructure::attribute_count> reconstituted2;
    std::array<std::int16_t, PhotoreceptorStructure::attribute_count> reconstituted3;
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
    std::array<std::int16_t, PhotoreceptorStructure::attribute_count> original;
    std::array<std::int16_t, PhotoreceptorStructure::attribute_count> reconstituted;
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
    std::array<std::int8_t, PhotoreceptorStructure::attribute_count+1> mutation_rates;
    std::array<std::int8_t, PhotoreceptorStructure::attribute_count+1> attribute_sizes;
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
    std::array<std::int16_t, PressureSensingStructure::attribute_count> original;
    std::array<std::int16_t, PressureSensingStructure::attribute_count> reconstituted1;
    std::array<std::int16_t, PressureSensingStructure::attribute_count> reconstituted2;
    std::array<std::int16_t, PressureSensingStructure::attribute_count> reconstituted3;
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
    std::array<std::int16_t, PressureSensingStructure::attribute_count> original;
    std::array<std::int16_t, PressureSensingStructure::attribute_count> reconstituted;
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
    std::array<std::int8_t, PressureSensingStructure::attribute_count+1> mutation_rates;
    std::array<std::int8_t, PressureSensingStructure::attribute_count+1> attribute_sizes;
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
    std::array<std::int16_t, SurfaceStructure::attribute_count> original;
    std::array<std::int16_t, SurfaceStructure::attribute_count> reconstituted1;
    std::array<std::int16_t, SurfaceStructure::attribute_count> reconstituted2;
    std::array<std::int16_t, SurfaceStructure::attribute_count> reconstituted3;
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
    std::array<std::int16_t, SurfaceStructure::attribute_count> original;
    std::array<std::int16_t, SurfaceStructure::attribute_count> reconstituted;
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
    std::array<std::int8_t, SurfaceStructure::attribute_count+1> mutation_rates;
    std::array<std::int8_t, SurfaceStructure::attribute_count+1> attribute_sizes;
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
    std::array<std::int16_t, PhotoreceptorMaterials::attribute_count> original;
    std::array<std::int16_t, PhotoreceptorMaterials::attribute_count> reconstituted1;
    std::array<std::int16_t, PhotoreceptorMaterials::attribute_count> reconstituted2;
    std::array<std::int16_t, PhotoreceptorMaterials::attribute_count> reconstituted3;
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
    std::array<std::int16_t, PhotoreceptorMaterials::attribute_count> original;
    std::array<std::int16_t, PhotoreceptorMaterials::attribute_count> reconstituted;
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
    std::array<std::int8_t, PhotoreceptorMaterials::attribute_count+1> mutation_rates;
    std::array<std::int8_t, PhotoreceptorMaterials::attribute_count+1> attribute_sizes;
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
    std::array<std::int16_t, ClosedFluidSystemComposition::attribute_count> original;
    std::array<std::int16_t, ClosedFluidSystemComposition::attribute_count> reconstituted1;
    std::array<std::int16_t, ClosedFluidSystemComposition::attribute_count> reconstituted2;
    std::array<std::int16_t, ClosedFluidSystemComposition::attribute_count> reconstituted3;
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
    std::array<std::int16_t, ClosedFluidSystemComposition::attribute_count> original;
    std::array<std::int16_t, ClosedFluidSystemComposition::attribute_count> reconstituted;
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
    std::array<std::int8_t, ClosedFluidSystemComposition::attribute_count+1> mutation_rates;
    std::array<std::int8_t, ClosedFluidSystemComposition::attribute_count+1> attribute_sizes;
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
    std::array<std::int16_t, Mineralization::attribute_count> original;
    std::array<std::int16_t, Mineralization::attribute_count> reconstituted1;
    std::array<std::int16_t, Mineralization::attribute_count> reconstituted2;
    std::array<std::int16_t, Mineralization::attribute_count> reconstituted3;
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
    std::array<std::int16_t, Mineralization::attribute_count> original;
    std::array<std::int16_t, Mineralization::attribute_count> reconstituted;
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
    std::array<std::int8_t, Mineralization::attribute_count+1> mutation_rates;
    std::array<std::int8_t, Mineralization::attribute_count+1> attribute_sizes;
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
    std::array<std::int16_t, Pigmentation::attribute_count> original;
    std::array<std::int16_t, Pigmentation::attribute_count> reconstituted1;
    std::array<std::int16_t, Pigmentation::attribute_count> reconstituted2;
    std::array<std::int16_t, Pigmentation::attribute_count> reconstituted3;
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
    std::array<std::int16_t, Pigmentation::attribute_count> original;
    std::array<std::int16_t, Pigmentation::attribute_count> reconstituted;
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
    std::array<std::int8_t, Pigmentation::attribute_count+1> mutation_rates;
    std::array<std::int8_t, Pigmentation::attribute_count+1> attribute_sizes;
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







TEST_CASE( "Toxins encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, Toxins::attribute_count> original;
    std::array<std::int16_t, Toxins::attribute_count> reconstituted1;
    std::array<std::int16_t, Toxins::attribute_count> reconstituted2;
    std::array<std::int16_t, Toxins::attribute_count> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Toxins constituent1;
    Toxins constituent2;

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

TEST_CASE( "Toxins encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, Toxins::attribute_count> original;
    std::array<std::int16_t, Toxins::attribute_count> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Toxins constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "Toxins static method consistency", "[many]" ) {
    std::array<std::int8_t, Toxins::attribute_count+1> mutation_rates;
    std::array<std::int8_t, Toxins::attribute_count+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    Toxins::getMutationRates(mutation_rates.begin());
    Toxins::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}







TEST_CASE( "CellSignals encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, CellSignals::attribute_count> original;
    std::array<std::int16_t, CellSignals::attribute_count> reconstituted1;
    std::array<std::int16_t, CellSignals::attribute_count> reconstituted2;
    std::array<std::int16_t, CellSignals::attribute_count> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    CellSignals constituent1;
    CellSignals constituent2;

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

TEST_CASE( "CellSignals encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, CellSignals::attribute_count> original;
    std::array<std::int16_t, CellSignals::attribute_count> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    CellSignals constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "CellSignals static method consistency", "[many]" ) {
    std::array<std::int8_t, CellSignals::attribute_count+1> mutation_rates;
    std::array<std::int8_t, CellSignals::attribute_count+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    CellSignals::getMutationRates(mutation_rates.begin());
    CellSignals::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}









TEST_CASE( "AppendageSegment encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, AppendageSegment::attribute_count> original;
    std::array<std::int16_t, AppendageSegment::attribute_count> reconstituted1;
    std::array<std::int16_t, AppendageSegment::attribute_count> reconstituted2;
    std::array<std::int16_t, AppendageSegment::attribute_count> reconstituted3;
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
    std::array<std::int16_t, AppendageSegment::attribute_count> original;
    std::array<std::int16_t, AppendageSegment::attribute_count> reconstituted;
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
    std::array<std::int8_t, AppendageSegment::attribute_count+1> mutation_rates;
    std::array<std::int8_t, AppendageSegment::attribute_count+1> attribute_sizes;
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
    std::array<std::int16_t, Appendage::attribute_count> original;
    std::array<std::int16_t, Appendage::attribute_count> reconstituted1;
    std::array<std::int16_t, Appendage::attribute_count> reconstituted2;
    std::array<std::int16_t, Appendage::attribute_count> reconstituted3;
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
    std::array<std::int16_t, Appendage::attribute_count> original;
    std::array<std::int16_t, Appendage::attribute_count> reconstituted;
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
    std::array<std::int8_t, Appendage::attribute_count+1> mutation_rates;
    std::array<std::int8_t, Appendage::attribute_count+1> attribute_sizes;
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
    std::array<std::int16_t, BodySegment::attribute_count> original;
    std::array<std::int16_t, BodySegment::attribute_count> reconstituted1;
    std::array<std::int16_t, BodySegment::attribute_count> reconstituted2;
    std::array<std::int16_t, BodySegment::attribute_count> reconstituted3;
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
    std::array<std::int16_t, BodySegment::attribute_count> original;
    std::array<std::int16_t, BodySegment::attribute_count> reconstituted;
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
    std::array<std::int8_t, BodySegment::attribute_count+1> mutation_rates;
    std::array<std::int8_t, BodySegment::attribute_count+1> attribute_sizes;
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










TEST_CASE( "Body encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, Body::attribute_count> original;
    std::array<std::int16_t, Body::attribute_count> reconstituted1;
    std::array<std::int16_t, Body::attribute_count> reconstituted2;
    std::array<std::int16_t, Body::attribute_count> reconstituted3;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Body constituent1;
    Body constituent2;

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

TEST_CASE( "Body encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, Body::attribute_count> original;
    std::array<std::int16_t, Body::attribute_count> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Body constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "Body static method consistency", "[many]" ) {
    std::array<std::int8_t, Body::attribute_count+1> mutation_rates;
    std::array<std::int8_t, Body::attribute_count+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    Body::getMutationRates(mutation_rates.begin());
    Body::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}










TEST_CASE( "Genome encode/decode regularity", "[many]" ) {
    std::array<std::int16_t, Genome::attribute_count> original;
    std::array<std::int16_t, Genome::attribute_count> result;
    std::array<std::int16_t, Genome::attribute_count> duplicate_encode_result;
    std::array<std::int16_t, Genome::attribute_count> duplicate_decode_result;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Genome temp1;
    Genome temp2;

    SECTION("decode and encode must be called repeatedly without changing the value of output references"){
        temp1.decode(original.begin());
        temp1.encode(result.begin());
        temp2.decode(original.begin());
        temp2.encode(duplicate_decode_result.begin());
        CHECK(result==duplicate_decode_result);
        temp1.encode(duplicate_encode_result.begin());
        CHECK(result==duplicate_encode_result);
    }
}

TEST_CASE( "Genome encode/decode invertibility", "[many]" ) {
    std::array<std::int16_t, Genome::attribute_count> original;
    std::array<std::int16_t, Genome::attribute_count> reconstituted;
    int count = 0;  
    std::generate(original.begin(), original.end(), [&](){ return count=(count+1)%0xF; });
    Genome constituent;
 
    SECTION("decoding an array then reencoding it must reproduce the original array"){
        constituent.decode(original.begin());
        constituent.encode(reconstituted.begin());
        CHECK(original==reconstituted);
    }
}

TEST_CASE( "Genome static method consistency", "[many]" ) {
    std::array<std::int8_t, Genome::attribute_count+1> mutation_rates;
    std::array<std::int8_t, Genome::attribute_count+1> attribute_sizes;
    std::fill(mutation_rates.begin(), mutation_rates.end(), -1);
    std::fill(attribute_sizes.begin(), attribute_sizes.end(), -1);
    Genome::getMutationRates(mutation_rates.begin());
    Genome::getAttributeSizes(attribute_sizes.begin());
    SECTION("mutation rates and attribute sizes must have the same count"){
        CHECK(mutation_rates.end()[-2] > -1);
        CHECK(mutation_rates.end()[-1] == -1);
        CHECK(attribute_sizes.end()[-2] > -1);
        CHECK(attribute_sizes.end()[-1] == -1);
    }
}





TEST_CASE( "gene namespace data structure size limits", "[many]" ) {
    std::cout << "AppendageSegment compressed: " << AppendageSegment::bit_count / 8 << std::endl;
    std::cout << "Appendage compressed: " << Appendage::bit_count / 8 << std::endl;
    std::cout << "BodySegment compressed: " << BodySegment::bit_count / 8 << std::endl;
    std::cout << "Body compressed: " << Body::bit_count / 8 << std::endl;
    std::cout << "Genome compressed: " << Genome::bit_count / 8 << std::endl;

    std::cout << "AppendageSegment decompressed: " << sizeof(AppendageSegment) << std::endl;
    std::cout << "Appendage decompressed: " << sizeof(Appendage) << std::endl;
    std::cout << "BodySegment decompressed: " << sizeof(BodySegment) << std::endl;
    std::cout << "Body decompressed: " << sizeof(Body) << std::endl;
    std::cout << "Genome decompressed: " << sizeof(Genome) << std::endl;

    SECTION("compressed memory footprint must not fall outside acceptable limits"){
        CHECK(AppendageSegment::bit_count/8 <= 32);
        CHECK(Appendage::bit_count/8 <= 256 );
        CHECK(BodySegment::bit_count/8 <= 256 );
        CHECK(Body::bit_count/8 <= 2048 );
        CHECK(sizeof(AppendageSegment) <= 64 ); // size of a cache line
        CHECK(sizeof(Body) < 64000 ); // conservative size of L1 data cache
        CHECK(sizeof(Genome) < 64000 ); // conservative size of L1 data cache
    }
}

