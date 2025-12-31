#pragma once

#include <cmath>

namespace combinatoric {

    // number of possible ways to order a set of candidates
    // (repetition is not allowed, order matters)
    template<typename I>
    constexpr I factorial(const I candidates)
    {
        I y(1);
        for (I i = 1; i <= candidates; ++i)
        {
            y *= i;
        }
        return y;
    }

    // number of possible sets of any size composed from a set of candidates
    // (repetition is not allowed, order does not matter)
    template<typename I>
    I subset(const I candidates)
    {
        return std::pow(2.0, candidates);
    }

    // number of possible sets of given size composed from a set of candidates
    // (repetition is not allowed, order does not matter)
    template<typename I>
    constexpr I combination(const I candidates, const I size)
    {
        return size > candidates? 0 : factorial(candidates) / (factorial(size) * factorial(candidates - size));
    }

    // number of possible bags of given size composed from a set of candidates
    // (repetition is allowed, order does not matter)
    template<typename I>
    constexpr I bag(const I candidates, const I size)
    {
        return combination(candidates+size-I(1), size);
    }

    // number of possible permutations of given size composed from a set of candidates
    // (repetition is not allowed, order matters)
    template<typename I>
    constexpr I permutation(const I candidates, const I size)
    {
        return size > candidates? 0 : factorial(candidates) / factorial(candidates-size);
    }

    // number of possible sequences of given size composed from a set of candidates
    // (repetition is allowed, order matters)
    template<typename I>
    I sequence(const I candidates, const I size)
    {
        return std::pow(candidates,size);
    }

    // number of possible allocations of indistiguishable resources among candidate bags
    template<typename I>
    constexpr I allocation(const I resources, const I candidates)
    {
        return combination(resources+candidates-I(1), candidates-I(1));
    }

}
