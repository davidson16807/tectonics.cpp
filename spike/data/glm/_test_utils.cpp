#pragma once

// std libraries
#include <random>

// in-house libraries
#include "../Series.hpp"
#include "relational.hpp"

namespace data
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
    data::Series<glm::vec3> get_random_vec3s(std::size_t size, Tdistribution& distribution, Tgenerator& generator)
    {
        data::Series<glm::vec3> output(size);
        for (std::size_t j = 0; j < size; ++j)
        {
            output[j].x = distribution(generator);
            output[j].y = distribution(generator);
            output[j].z = distribution(generator);
        }
        return output;
    }
    template<typename Tgenerator>
    data::Series<glm::vec3> get_random_vec3s(std::size_t size, Tgenerator& generator)
    {
        std::uniform_real_distribution<double> distribution(-5.0,5.0);
        return get_random_vec3s(size, distribution, generator);
    }


    template<typename Tgenerator>
    data::Series<glm::bvec3> get_random_bvec3s(std::size_t size, Tgenerator& generator)
    {
        data::Series<glm::bvec3> output(size);
        data::Series<glm::vec3> temp = get_random_vec3s(size, generator);
        data::vec::greaterThan(temp, data::uniform(glm::vec3(0.5f)), output);
        return output;
    }
}

