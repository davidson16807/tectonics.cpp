#pragma once

// C libraries
#include <cmath>

// in-house libraries
#include <models/stratum/Stratum_to_StratumTypes.hpp>
#include <models/stratum/Stratum_operators.hpp>
#include "Strata.hpp"

namespace strata
{


    /*
    "get_sediment" is a regular function that returns a strata with a single layer.
    We return a strata instead of a strata to prevent the categories that invoke this function
    from having to deal with categories below.
    */
    template <std::size_t L, std::size_t M>
    static void get_sediment(const Strata<L,M>& input, Strata<1,M>& output)
    {
        if (input.count > 0 && stratum::get_stratum_types(input.content[0]).dominant_particle_size_bin <= stratum::ParticleSizeBins::sand)
        {
            output.content[0] = input.content[0];
            output.count = 1;
        }
        else 
        {
            output.count = 0;
        }
    }

    /*
    "simplify" is a regular function that iterates through layers, 
    combines similar layers together, and stores results in output.
    */
    template <std::size_t L, std::size_t M>
    static void simplify(const Strata<L,M>& input, Strata<L,M>& output)
    {
        /*
        NOTE: we only want to combine layers when they "resemble" each other.
        We first have to define what "resemblance" means. 
        The utility of a model guides us in assigning the definition of its concepts,
        so it's probably best to start with what the purpose a rock layer serves within our model,
        and use that purpose to define this equality function.
        Rock layers don't have a very strong affect on model behavior,
        they do affect erosion rates and thermal conductivity, 
        but as long as those result in values that lie within a plausible range,
        we cannot err significantly enough in our definition to cause a loss of confidence in the model.
        The only other reason to represent rock layer information is to present that information to the user,
        Based on these observations, we conclude layers should only be distinguished 
        based on whether that distinction is currently meaningful to the user, or has the potential to be later on.
        `get_rock_type()` defines every type of rock in geology that can be represented by the model,
        so we can study it for guidance, however there are some distinctions it makes that we don't care about,
        and it sometimes ignores distinctions that become useful later on. 
        For instance, ultramafic sedimentary rock is indistinguishable from other sedimentary rocks,
        but if metamorphosed it becomes very distinctive, so we must always distinguish based on composition type.
        To conserve effort, we construct `get_stratum_types().hash()`, 
        which represents a superset of the abstract values that may be returned by `get_rock_type()`,
        and is a cartesian product of the data types that are used within that function.
        */
        std::size_t i(0);
        std::size_t j(0);
        for (; i < input.count; ++j)
        {
            if (i+1 < input.count && 
                stratum::get_stratum_types(input.content[i]).hash() == stratum::get_stratum_types(input.content[i+1]).hash())
            {
                stratum::combine(input.content[i], input.content[i+1], output.content[j]);
                i += 2;
                j += 1;
            }
            else
            {
                output.content[j] = input.content[i];
                i += 1;
                j += 1;
            }
        }
        output.count = j-1;
    }

    /*
    Returns a new strata that represents the top strata overlapping the bottom strata, 
    Merges adjacent layers if there is a resembance between them.
    If the are more layers than what can be stored in memory, we combine the bottom excess layers into one. 
    We allow combining the bottom regardless of resemblance, 
    since we care the least about properly representing the bottom layers, 
    and we are required to fit within stack memory, just as long as the mass in each pool is conserved.
    */
    template <std::size_t L1, std::size_t L2, std::size_t L3, std::size_t M>
    static void overlap(const Strata<L1,M>& top, const Strata<L2,M>& bottom, Strata<L3,M>& output)
    {
        std::size_t top_i(0);
        std::size_t bottom_i(0);
        for (; top_i < top.count && top_i < L3; ++top_i)
        {
            output.content[top_i] = top.content[top_i];
        }
        for (; top_i < top.count && top_i >= L3; ++top_i)
        {
            stratum::combine(output.content[L3-1], top.content[top_i], output.content[L3-1]);
        }
        // combine the adjacent layers of top and bottom, if similar
        std::size_t adjacent_i = std::min(top.count, L3)-1;
        if (stratum::get_stratum_types(output.content[adjacent_i]).hash() == 
            stratum::get_stratum_types(bottom.content.front()).hash())
        {
            stratum::combine(output.content[adjacent_i], bottom.content.front(), output.content[adjacent_i]);
            bottom_i++;
        }
        for (; bottom_i < bottom.count && top_i+bottom_i < L3; ++bottom_i)
        {
            output.content[top_i+bottom_i] = bottom.content[bottom_i];
        }
        for (; bottom_i < bottom.count && top_i+bottom_i >= L3; ++bottom_i)
        {
            stratum::combine(output.content[L3-1], bottom.content[bottom_i], output.content[L3-1]);
        }
        output.count = std::min(top.count+bottom.count, L3);
    }
}
