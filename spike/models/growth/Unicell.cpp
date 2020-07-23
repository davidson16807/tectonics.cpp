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
    class Unicell
    {
        // materials of the lens, retina, cornea, vitreous humor, etc.
        PhotoreceptorMaterials photoreceptor_materials;
        // minerals within endoskeleton
        Mineralization interior_mineralization;
        // TODO: toxins
        // Toxin toxin;

        /*
        NOTE: I would normally create encode() and decode() as pure functions outside the class,
        but some classes in the genotype/phenotype pipeline use encapsulation 
        to prevent underflow/overflow and prevent generation of illegal state space.
        These classes require encode() and decode() as methods, 
        and I design the other classes to follow suit to create consistent design.
        */
        void decode(Body& genome)
        {
            
        }
    };
}
