#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#include <algorithm>

#include <model/genes/coding.hpp>    // encode_*(), decode_*()

namespace genes
{
    struct Metabolism
    {
        /*
        See the following for an introduction: 
        https://speculativeevolution.fandom.com/wiki/Speculative_bioenergetics#Metabolic_chemistry
        
        The last universal common ancestor of all modern earth life ("LUCA" for short)
        is estimated to have evolved 4 billion years ago. 
        Even this early on, LUCA was a sophisticated organism 
        with a relatively complete set of the basic metabolic pathways,
        including a few pathways that interestingly suggest it lived around hydrothermal vents.

        The smallest genome possible for LUCA would have been around 424 genes (Mat et al. 2008)
        By comparison, the smallest genome we could conceive of for a living organism 
        is also around dex 5 (Forster 2006).
        The smallest genome we see today (Carsonella ruddii) is around 182 genes and dex 5 base pairs,
        and this is likely only possible because the organism lives as a symbiote inside an insect.
        So we suspect LUCA was likely bigger than dex 5 base pairs.
        However even if we assume LUCA was dex 5 pairs, 
        and even if we assume the size of genomes increases exponentially over time, 
        and even if we assume the most aggresive estimates for growth rate based upon these observations,
        we are unable accounted for a genome the size of LUCA's at that point in Earth's history (Sharov 2006).
        There are a number of explanations to account for this discrepancy.
        It may have been that genomes increased in size much more rapidly in a pre-LUCA world,
        or life may have emerged well before Earth even existed, consistent with the panspermia hypothesis.

        Due to a lack of clarity, our model will not attempt to represent life as it was before LUCA.
        It will simply assume that life emerges in a state resembling LUCA
        whenever conditions on a planet cross some conservative threshold needed for earth life.
        We will not represent the RNA world, or even the pre-LUCA world.
        It is fortunate this only prevents us from possibly modeling the first half billion years of evolution.

        Neglecting pre-LUCA life simplifies the task for modeling the evolution of metabolism.
        This is fortunate, because although there is merit in a system 
        that tracks the evolution of every major metabolic pathway,
        even a model for the most important metabolic pathways would be extemely complex.
        It would likely suffer performance issues, and it would be virtually impossible 
        to describe it in a way that is not overly tailored to known earth life.
        See https://en.wikipedia.org/wiki/Metabolic_pathway#Major_metabolic_pathways
        to get an idea how complex such a thing would be.

        We want only to create a simple, performant model that models post-LUCA life
        in a way that might be able to generalize to life on other planets.

        So we will not model how energy is stored. 
        Nor will we model specific pathways like Krebs, or Wood-Ljungdahl
        We will model metabolism in as non-descript a way we can,
        while still meeting the requirements outlined for the rest of the model.
        
        So what are these requirements?
        At a minimum, we must model respresent how life affects the composition of the atmosphere.
        So we do have to describe some chemical reactions,
        but only to the extent we might see in a high school biology course,
        and only for processes that we think will likely have noticeable affects on the atmosphere.
        */
        float fermentation; 
        float photosynthesis;
        float aerobic_respiration;
        float methanogenesis;
        float nitrification;

        template<typename TIterator>
        TIterator encode(TIterator output) const
        {
            *output++ = encode_fraction(fermentation       );
            *output++ = encode_fraction(photosynthesis     );
            *output++ = encode_fraction(aerobic_respiration);
            *output++ = encode_fraction(methanogenesis     );
            *output++ = encode_fraction(nitrification      );
            return output;
        }
        template<typename TIterator>
        TIterator decode(TIterator input)
        {
            fermentation        = decode_fraction(*input++);
            photosynthesis      = decode_fraction(*input++);
            aerobic_respiration = decode_fraction(*input++);
            methanogenesis      = decode_fraction(*input++);
            nitrification       = decode_fraction(*input++);
            return input;
        }
        template<typename TIterator>
        static TIterator getMutationRates(TIterator output)
        {
            return std::fill_n(output, 5, 1);
        }
        template<typename TIterator>
        static TIterator getAttributeSizes(TIterator output)
        {
            return std::fill_n(output, 5, 4);
        }
        static constexpr unsigned int bit_count = 5*4;
        static constexpr unsigned int attribute_count = 5;
    };
}
