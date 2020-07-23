#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <models/genes/coding.hpp>    // encode_*(), decode_*()
#include <models/genes/segments/BodySegment.hpp>
#include <models/genes/materials/ClosedFluidSystemComposition.hpp>
#include <models/genes/materials/PhotoreceptorMaterials.hpp>
#include <models/genes/materials/Mineralization.hpp>

namespace bodies
{
    /*
    a "Body" describes all attributes of an organism that contribute to fitness.

    It attempts to be:
    * properly partial (represents a proper subset of all possible organisms, i.e. no illegal state space)
    It does not attempt to be:
    * total (does not describe all possible organisms)
    * unique (allows multiple ways to express the same organism)
    * unambiguous (an instance can describe more than one real life organism, e.g. armadillos vs turtles)

    A body data structure describes:
    * segmentation patterns (radial symmetry, segment count, etc.)
    * segment specializations
    * materials that can be synthesized by the body

    See [designing-an-evolutionary-model.md](designing-an-evolutionary-model.md) for more information
    */
    class Body
    {

        // arms, legs, flippers, wings, antenna, eye stalks, facial structures
        std::array<BodySegment, BODY_SEGMENT_COUNT> segments;
        // blood, lymph, hydraulic systems, etc.
        ClosedFluidSystemComposition closed_fluid_system_composition;
        // materials of the lens, retina, cornea, vitreous humor, etc.
        PhotoreceptorMaterials photoreceptor_materials;
        // minerals within endoskeleton
        Mineralization interior_mineralization;

        // basic dimensions, in meters, log2 scale
        float length;
        float max_width; 
        float max_height;
        int   axial_segment_count;
        int   radial_segment_count;
        int   angular_segment_count;
        float angular_segment_axial_offset; // for segment whorling or alternation

        /* 
        whether the organism exists as a valid Unicell at some point during development
        the entire class can be thought of as a maybe monad, but since we want to 
        encapsulate potentially invalid attributes, we do this instead.
        */
        bool  exists;
        /*
        BODY SCALING SYSTEM:
        total body length                       meters, log scale
        total body width                        max body segment width found within the body
        total body height                       max body segment height found within the body
        body segment length                     relative portion of total body length
        body segment height                     multiple of total body length, log scale
        body segment width                      multiple of total body length, log scale
        appendage length                        multiple of total body length, log scale
        appendage width                         max appendage segment width found within the appendage
        appendage height                        max appendage segment height found within the appendage
        appendage segment length                relative portion of appendage length
        appendage segment height                fraction of body segment height
        appendage segment width                 fraction of body segment length
        appendage segment insertion             fraction of parent appendage segment
        appendage segment thickness attributes  proportion of half the appendage segment width or height, whichever is smallest
        appendage segment vessel radius         proportion of half the appendage segment width or height, whichever is smallest
        appendage segment vessel thickness      fraction of corresponding vessel radius
        appendage segment coverage              fraction of appendage segment surface area
        */

        /*
        Advances Unicell to the next growth stage,
        as according to a given genetic instruction set.
        Stores result within the object
        */
        void advance(Unicell& cell, genes::Body& stage)
        {
            length = exp2(stage.log2_total_length) * units::meter;
            /* 
            0.2mm is the upper bound for one estimate of the size of a blastula
            we postulate there is a size threshold below which diffusion
            cannot occur slow enough for basic pattern formation,
            see "designing-an-evolutionary-model.md"
            */
            exists = cell.exists && 
               !stage.gene_disabled &&
                morulation_factor > 0.5 && 
                gastrulation_factor > 0.5 && 
                length > 0.2*units::millimeter;

            axial_segment_count          = std::clamp(stage.axial_segment_count, 0.0f, BODY_SEGMENT_COUNT);
            radial_segment_count         = std::clamp(stage.radial_segment_count, 0.0f, APPENDAGE_SEGMENT_COUNT);
            angular_segment_count        = std::clamp(stage.angular_segment_count, 0.0f, 16.0f);
            angular_segment_axial_offset = std::clamp(stage.angular_segment_axial_offset, 0.0f, 1.0f);

            // create temporary estimates for max width and height
            max_width = exp2(log2_max_width_as_multiple_of_length);
            max_height = exp2(log2_max_height_as_multiple_of_length);

            // first pass: run through segment, counting up portions of length
            float segment_portion_count = 0;
            auto last_active = stage.segments.begin();
            for (auto gene = stage.segments.begin(); gene != stage.segments.end(); ++gene)
            {
                last_active = gene->homeotic_gene_disabled > 0.5? last_active : gene;
                segment_portion_count += last_active->length_as_portion_of_total;
            }

            float length_per_segment_portion = length/segment_portion_count;
            auto gene = gene.segments.begin(); 
            auto part = part.segments.begin(); 
            auto last_active = gene.segments.begin();
            for (i=0; i<axial_segment_count; ++i)
            {
                last_active = gene->homeotic_gene_disabled > 0.5? last_active : gene;
                part->decode(last_active, length, max_width, max_height, length_per_segment_portion);
                ++gene; ++part;
            }
            gene  = closed_fluid_system_composition.decode(gene);
            gene  = photoreceptor_materials.decode(gene);
            gene  = interior_mineralization.decode(gene);

            return gene;
        }

        /*
        Advances given life stage to the next growth stage,
        as according to a given genetic instruction set.
        Stores result within the object
        */
        void advance(bodies::Body& stage, genes::Body& stage)
        {
            length = exp2(stage.log2_total_length) * units::meter;
            /* 
            0.2mm is the upper bound for one estimate of the size of a blastula
            we postulate there is a size threshold below which diffusion
            cannot occur slow enough for basic pattern formation,
            see "designing-an-evolutionary-model.md"
            */
            exists = cell.exists && 
               !stage.gene_disabled &&
                morulation_factor > 0.5 && 
                gastrulation_factor > 0.5 && 
                length > 0.2*units::millimeter;

            axial_segment_count          = std::clamp(stage.axial_segment_count, 0.0f, BODY_SEGMENT_COUNT);
            radial_segment_count         = std::clamp(stage.radial_segment_count, 0.0f, 16.0f);
            angular_segment_count        = std::clamp(stage.angular_segment_count, 0.0f, 16.0f);
            angular_segment_axial_offset = std::clamp(stage.angular_segment_axial_offset, 0.0f, 1.0f);

            // create temporary estimates for max width and height
            max_width = exp2(log2_max_width_as_multiple_of_length);
            max_height = exp2(log2_max_height_as_multiple_of_length);

            // first pass: run through segment, counting up portions of length
            float segment_portion_count = 0;
            auto last_active = stage.segments.begin();
            for (auto current = stage.segments.begin(); 
                 current != stage.segments.end(); ++current){
                last_active = current->homeotic_gene_disabled > 0.5? last_active : current;
                segment_portion_count += last_active->length_as_portion_of_total;
            }

            float length_per_segment_portion = length/segment_portion_count;
            auto gene = gene.segments.begin(); 
            auto part = part.segments.begin(); 
            auto last_active = gene.segments.begin();
            for (i=0; i<axial_segment_count; ++i)
            {
                last_active = gene->homeotic_gene_disabled > 0.5? last_active : gene;
                part->decode(last_active, length, max_width, max_height, length_per_segment_portion);
                ++gene; ++part;
            }
            gene  = closed_fluid_system_composition.decode(gene);
            gene  = photoreceptor_materials.decode(gene);
            gene  = interior_mineralization.decode(gene);

            return gene;
        }
    };
}
