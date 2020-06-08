#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <models/genes/coding.hpp>    // encode_*(), decode_*()
#include <models/genes/segments/BodySegment.hpp>
#include <models/genes/biochemistry/ClosedFluidSystemComposition.hpp>
#include <models/genes/biochemistry/PhotoreceptorMaterials.hpp>
#include <models/genes/biochemistry/Mineralization.hpp>

namespace genes
{
    /*
    The most complex life cycle we consider is the butterfly, 
    which we consider to have four stages (egg, caterpillar, crysalis, butterfly)
    All sexually reproducing organisms are presumed to have a diploid and haploid stage.
    For a human, a haploid stage is either a sperm or an egg,
    however the haploid stage need not always be unicellular.
    Some "haploid dominant" organisms like fungi have a multicellular haploid stage and unicellular diploid stage.
    Other organisms such as plants have both multicellular diploid and haploid stages.
    */
    const int HAPLOID_LIFE_STAGE_COUNT = 4;
    const int DIPLOID_LIFE_STAGE_COUNT = 4;
    const int ENDOSYMBIONT_COUNT = 4;
    
    /*
    a "Genome" describes all attributes of an organism across all life cycle stages

    It attempts to be:
    * properly partial (represents a proper subset of all possible organisms, i.e. no illegal state space)
    It does not attempt to be:
    * total (does not describe all possible organisms)
    * unique (allows multiple ways to express the same organism)
    * unambiguous (an instance can describe more than one real life organism, e.g. armadillos vs turtles)

    See [designing-an-evolutionary-model.md](designing-an-evolutionary-model.md) for more information
    */
    struct Genome
    {
        std::array<Body, DIPLOID_LIFE_STAGE_COUNT> diploid_stages;
        std::array<Body, HAPLOID_LIFE_STAGE_COUNT> haploid_stages;
        std::array<Endosymbiont, ENDOSYMBIONT_COUNT> endosymbionts;
        ClosedFluidSystemComposition closed_fluid_system_composition;
        PhotoreceptorMaterials photoreceptor_materials;
        Mineralization interior_mineralization;
        Toxins toxins;
        Pigmentation pigmentation_synthesis;
        CellSignals cell_signals;
        Metabolism metabolism;

        /*
        Segmentation plans are caused by chemical reaction-diffusion systems,
        so we postulate they are constant for a single species, 
        analogous to other compounds that an organism can synthesize.
        This implies that body plans would be reused across life stages.
        */
        float axial_segment_count;
        float radial_segment_count;
        float angular_segment_count;
        float angular_segment_axial_offset; // for segment whorling or alternation



        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            output = encode_container(diploid_stages.begin(), diploid_stages.end(), output);
            output = encode_container(haploid_stages.begin(), haploid_stages.end(), output);
            output = encode_container(endosymbionts.begin(),  endosymbionts.end(),  output);
            output = closed_fluid_system_composition.encode(output);
            output = photoreceptor_materials        .encode(output);
            output = interior_mineralization        .encode(output);
            output = toxins                         .encode(output);
            output = pigmentation_synthesis         .encode(output);
            output = cell_signals                   .encode(output);
            output = metabolism                     .encode(output);
            *output++ = encode_ranged  (axial_segment_count, 0, BODY_SEGMENT_COUNT);
            *output++ = encode_ranged  (radial_segment_count, 0, APPENDAGE_SEGMENT_COUNT);
            *output++ = encode_portion (angular_segment_count);
            *output++ = encode_fraction(angular_segment_axial_offset);
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            input = decode_container(input, diploid_stages.begin(), diploid_stages.end());
            input = decode_container(input, haploid_stages.begin(), haploid_stages.end());
            input = decode_container(input, endosymbionts.begin(),  endosymbionts.end() );
            input = closed_fluid_system_composition.decode(input);
            input = photoreceptor_materials        .decode(input);
            input = interior_mineralization        .decode(input);
            input = toxins                         .decode(input);
            input = pigmentation_synthesis         .decode(input);
            input = cell_signals                   .decode(input);
            input = metabolism                     .decode(input);
            axial_segment_count          = decode_ranged  (*input++, 0, BODY_SEGMENT_COUNT);
            radial_segment_count         = decode_ranged  (*input++, 0, APPENDAGE_SEGMENT_COUNT);
            angular_segment_count        = decode_portion (*input++);
            angular_segment_axial_offset = decode_fraction(*input++);
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            for (int i = 0; i < DIPLOID_LIFE_STAGE_COUNT; ++i)
            {
                output = Body::getMutationRates(output);
            }
            for (int i = 0; i < HAPLOID_LIFE_STAGE_COUNT; ++i)
            {
                output = Body::getMutationRates(output);
            }
            for (int i = 0; i < ENDOSYMBIONT_COUNT; ++i)
            {
                output = Endosymbiont::getMutationRates(output);
            }
            output = ClosedFluidSystemComposition::getMutationRates(output);
            output = PhotoreceptorMaterials::getMutationRates(output);
            output = Mineralization::getMutationRates(output);
            output = Toxins::getMutationRates(output);
            output = Pigmentation::getMutationRates(output);
            output = CellSignals::getMutationRates(output);
            output = Metabolism::getMutationRates(output);
            output = std::fill_n(output, 4, 1);

            return output;
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            for (int i = 0; i < DIPLOID_LIFE_STAGE_COUNT; ++i)
            {
                output = Body::getAttributeSizes(output);
            }
            for (int i = 0; i < HAPLOID_LIFE_STAGE_COUNT; ++i)
            {
                output = Body::getAttributeSizes(output);
            }
            for (int i = 0; i < ENDOSYMBIONT_COUNT; ++i)
            {
                output = Endosymbiont::getAttributeSizes(output);
            }
            output = ClosedFluidSystemComposition::getAttributeSizes(output);
            output = PhotoreceptorMaterials::getAttributeSizes(output);
            output = Mineralization::getAttributeSizes(output);
            output = Toxins::getAttributeSizes(output);
            output = Pigmentation::getAttributeSizes(output);
            output = CellSignals::getAttributeSizes(output);
            output = Metabolism::getAttributeSizes(output);
            output = std::fill_n(output, 4, 4);
            return output;
        }
        static constexpr unsigned int bit_count = 
            Body                        ::bit_count * DIPLOID_LIFE_STAGE_COUNT +
            Body                        ::bit_count * HAPLOID_LIFE_STAGE_COUNT +
            Endosymbiont                ::bit_count * ENDOSYMBIONT_COUNT +
            ClosedFluidSystemComposition::bit_count +
            PhotoreceptorMaterials      ::bit_count +
            Mineralization              ::bit_count +
            Toxins                      ::bit_count +
            Pigmentation                ::bit_count +
            CellSignals                 ::bit_count +
            Metabolism                  ::bit_count +
            4*4;
        static constexpr unsigned int attribute_count = 
            Body                        ::attribute_count * DIPLOID_LIFE_STAGE_COUNT +
            Body                        ::attribute_count * HAPLOID_LIFE_STAGE_COUNT +
            Endosymbiont                ::attribute_count * ENDOSYMBIONT_COUNT +
            ClosedFluidSystemComposition::attribute_count +
            PhotoreceptorMaterials      ::attribute_count +
            Mineralization              ::attribute_count +
            Toxins                      ::attribute_count +
            Pigmentation                ::attribute_count +
            CellSignals                 ::attribute_count +
            Metabolism                  ::attribute_count +
            4;
    };
}
