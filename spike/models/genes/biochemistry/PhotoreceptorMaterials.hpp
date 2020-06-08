#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <models/genes/coding.hpp>                  // encode_*(), decode_*()
#include <models/genes/constituents/Photopigment.hpp>    

namespace genes
{
    /*
    Rainbow shrimp have 12-16 photoreceptor types, the largest number known, 
    but most have no more than 4 (e.g. RGB and UV)
    */
    static constexpr int PHOTOPIGMENT_COUNT = 8;

    /*
    Photopigments are regularly sensitive to 280nm but are blocked by corneas
    Some fish have photopigments with a max sensitivity at 620nm. 
    Most photopigments seem to have a sensitivity range of 50nm, 
    we see this in humans and mantis shrimp
    */
    static constexpr int WAVELENGTH_SAMPLE_COUNT = 8;

    struct PhotoreceptorMaterials
    {
        // rods, cones
        std::array<Photopigment, PHOTOPIGMENT_COUNT> photopigments;
        std::array<float, WAVELENGTH_SAMPLE_COUNT> cover_extinction_coefficients;
        std::array<float, WAVELENGTH_SAMPLE_COUNT> humor_extinction_coefficients;

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            for (auto photopigment = photopigments.begin(); 
                 photopigment != photopigments.end(); ++photopigment)
            {
                output = photopigment->encode(output);
            }
            for (auto wavelength = cover_extinction_coefficients.begin(); 
                 wavelength != cover_extinction_coefficients.end(); ++wavelength)
            {
                *output++ = encode_fraction(*wavelength);
            }
            for (auto wavelength = humor_extinction_coefficients.begin(); 
                 wavelength != humor_extinction_coefficients.end(); ++wavelength)
            {
                *output++ = encode_fraction(*wavelength);
            }
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            for (auto photopigment = photopigments.begin(); 
                 photopigment != photopigments.end(); ++photopigment)
            {
                input = photopigment->decode(input);
            }
            for (auto wavelength = cover_extinction_coefficients.begin(); 
                 wavelength != cover_extinction_coefficients.end(); ++wavelength)
            {
                *wavelength = decode_fraction(*input++);
            }
            for (auto wavelength = humor_extinction_coefficients.begin(); 
                 wavelength != humor_extinction_coefficients.end(); ++wavelength)
            {
                *wavelength = decode_fraction(*input++);
            }
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            for (int i = 0; i < PHOTOPIGMENT_COUNT; ++i)
            {
                output = Photopigment::getMutationRates(output);
            }
            return std::fill_n(output, 2*WAVELENGTH_SAMPLE_COUNT, 1);
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            for (int i = 0; i < PHOTOPIGMENT_COUNT; ++i)
            {
                output = Photopigment::getAttributeSizes(output);
            }
            return std::fill_n(output, 2*WAVELENGTH_SAMPLE_COUNT, 4);
        }
        static constexpr unsigned int bit_count = 
            PHOTOPIGMENT_COUNT * Photopigment::bit_count 
            + 2*4*WAVELENGTH_SAMPLE_COUNT;
        static constexpr unsigned int attribute_count = 
            PHOTOPIGMENT_COUNT * Photopigment::attribute_count 
            + 2*WAVELENGTH_SAMPLE_COUNT;

    };


}
