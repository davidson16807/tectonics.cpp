#pragma once

// in-house libraries
#include <models/crust/Crust.hpp> // Crust
#include <models/strata/Strata_operators.hpp> // deposit

namespace crust
{
    template<int L, int M>
    void get_sediment(
        const Crust<L,M>& input, 
        Crust<1,M>& output
    ) {
        assert(input.size() == output.size());
        strata::Strata<L,M> input_strata;
        strata::Strata<1,M> output_strata;
        for (std::size_t i = 0; i < input.size(); ++i)
        {
            input[i].unpack(input_strata);
            strata::get_sediment(input_strata, output_strata);
            output[i].pack(output_strata);
        }
    }

    template<int L1, int L2, int L3, int M>
    void overlap(
        const Crust<L1,M>& top, 
        const Crust<L2,M>& bottom,
        Crust<L3,M>& output
    ) {
        assert(top.size() == bottom.size());
        assert(top.size() == output.size());
        strata::Strata<L1,M> top_strata;
        strata::Strata<L2,M> bottom_strata;
        strata::Strata<L3,M> output_strata;
        for (std::size_t i = 0; i < top.size(); ++i)
        {
            top[i].unpack(top_strata);
            bottom[i].unpack(bottom_strata);
            strata::overlap(top_strata, bottom_strata, output_strata);
            output[i].pack(output_strata);
        }
    }
}
