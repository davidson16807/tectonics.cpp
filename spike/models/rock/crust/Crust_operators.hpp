#pragma once

// in-house libraries
#include <models/crust/Crust.hpp> // Crust
#include <models/strata/Strata_operators.hpp> // deposit

namespace crust
{
    template<std::size_t L, std::size_t M, typename Tgrid>
    void get_sediment(
        const Crust<L,M,Tgrid>& input, 
        Crust<1,M,Tgrid>& output
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

    template<std::size_t L, std::size_t M, typename Tgrid>
    void simplify(
        const Crust<L,M,Tgrid>& input, 
        Crust<L,M,Tgrid>& output
    ) {
        assert(input.size() == output.size());
        strata::Strata<L,M> input_strata;
        strata::Strata<L,M> output_strata;
        for (std::size_t i = 0; i < input.size(); ++i)
        {
            input[i].unpack(input_strata);
            strata::simplify(input_strata, output_strata);
            output[i].pack(output_strata);
        }
    }

    template<std::size_t L1, std::size_t L2, std::size_t L3, std::size_t M, typename Tgrid>
    void overlap(
        const Crust<L1,M,Tgrid>& top, 
        const Crust<L2,M,Tgrid>& bottom,
        Crust<L3,M,Tgrid>& output
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
