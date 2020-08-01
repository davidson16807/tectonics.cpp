#pragma once

// C libraries
#include <cmath>

// in-house libraries
#include "Strata.hpp"
#include "Stratum_enums.hpp"
#include "Stratum_operators.hpp"

namespace strata
{
    /*
    "simplify" is a regular function that iterates through layers, 
    combines similar layers together, and stores results in output.
    */
    static void simplify(const Strata& input, Strata& output)
    {
        /*
        NOTE: we only want to combine layers when they "resemble" each other.
        We first have to define what "resemblance" means. 
        Since resemblance serves as an identity function in this particular context, 
        it defines what rocks layers are, in a way.
        The utility of a model guides us in assigning the definition of its concepts,
        so it's probably best to start with what the purpose a rock layer serves within our model,
        and use that purpose to define its identity function.
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
        To conserve effort, we construct `get_rock_type_hash()`, 
        which represents a superset of the abstract values that may be returned by `get_rock_type()`,
        and is a cartesian product of the data types that are used within that function.
        */
        int i = 0;
        int j = 0;
        for (; i < input.count-1; ++j)
        {
            if (get_rock_type_hash(input.strata[i]) == get_rock_type_hash(input.strata[i+1]))
            {
                Stratum::combine(input.strata[i], input.strata[i+1], output.strata[j]);
                i += 2;
            }
            else
            {
                output.strata[j] = input.strata[i];
                i += 1;
            }
        }
        output.count = j;
    }
    /*
    Deposits a new layer to the top of the strata.
    Merges the deposited layer with the second layer down if there is a resembance between them.
    If the are more layers than what can be stored in memory, we combine the bottom excess layers into one. 
    We only allow combining the bottom two since doing so simplifies the problem when working with stack memory.
    This is fine, since we care the least about properly representing the bottom layers,
    just as long as the mass in each pool is conserved.
    Functionally equivalent to `overlap()` if `bottom` were a Strata object with 0 or 1 layers,
    */
    static void deposit(const Strata& input, const Stratum top, const Strata& output)
    {
        int i;
        int offset = 0;
        if (get_rock_type_hash(input.strata[0]) != get_rock_type_hash(top))
        {
            output.strata[0] = top;
            offset = 1;
        }
        for (i = 0; i < bottom.count && offset+i < max_stratum_count; ++i)
        {
            output.strata[offset+i] = bottom.strata[i];
        }
        for (; i < bottom.count && offset+i >= max_stratum_count; ++i)
        {
            Stratum::combine(output.strata[max_stratum_count-1], bottom.strata[i], output.strata[max_stratum_count-1]);
        }
        output.count = min(input.count+offset, max_stratum_count);
    }

    /*
    Returns a new strata that represents the top strata subducting the bottom strata
    */
    static void overlap(const Strata& top, const Strata& bottom, const Strata& output)
    {
        int i;
        for (i = 0; i < top.count; ++i)
        {
            output.strata[i] = top.strata[i];
        }
        for (i = 0; i < bottom.count && top.count+i < max_stratum_count; ++i)
        {
            output.strata[top.count+i] = bottom.strata[i];
        }
        for (; i < bottom.count && top.count+i >= max_stratum_count; ++i)
        {
            Stratum::combine(output.strata[max_stratum_count-1], bottom.strata[i], output.strata[max_stratum_count-1]);
        }
        output.count = min(top.count+bottom.count, max_stratum_count);
    }
}
