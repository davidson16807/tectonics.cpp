#pragma once

// std libraries
#include <random>
#include <vector>

// in-house libraries
#include "../series/Interleave.hpp"
#include "each.hpp"

namespace each
{
    /*
    NOTE: 
    This file does not exist within the concept of the mathematical category defined in README.md.
    This is because the file is only relevant within the context of several potential unit test files.
    We differ from the usual style of method signature here since performance does not matter in test code. 
    We also use macros since we require running unit tests on individual attributes for granularity, 
    and our testing framework itself uses macros to run the unit tests.
    Our chief concern here is to simplify writing unit tests and interpreting their output.
    */

    template<typename Tgenerator, typename Tdistribution>
    auto get_random_vec3s(std::size_t size, Tdistribution& distribution, Tgenerator& generator)
    {
        auto output = series::interleave(size, glm::vec3());
        for (std::size_t j = 0; j < size; ++j)
        {
            output[j].x = distribution(generator);
            output[j].y = distribution(generator);
            output[j].z = distribution(generator);
        }
        return output;
    }
    template<typename Tgenerator>
    auto get_random_vec3s(std::size_t size, Tgenerator& generator)
    {
        std::uniform_real_distribution<double> distribution(-5.0,5.0);
        return get_random_vec3s(size, distribution, generator);
    }


    template<typename Tgenerator>
    auto get_random_bvec3s(std::size_t size, Tgenerator& generator)
    {
        auto output = series::interleave(size, glm::bvec3());
        series::Interleave<std::vector<glm::vec3>> temp = get_random_vec3s(size, generator);
        each::greaterThan(temp, series::uniform(glm::vec3(0.5f)), output);
        return output;
    }
}

