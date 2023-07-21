#pragma once

#include <cmath>

namespace math {

    constexpr int factorial(const int candidates)
    {
        int y = 1;
        for (int i = 1; i <= candidates; ++i)
        {
            y *= i;
        }
        return y;
    }

    // number of possible sets of any size composed from a set of candidates
    // (repetition is not allowed, order does not matter)
    constexpr int subset(const int candidates)
    {
        return std::pow(2, candidates);
    }

    // number of possible sets of given size composed from a set of candidates
    // (repetition is not allowed, order does not matter)
    constexpr int combination(const int candidates, const int size)
    {
        return size > candidates? 0 : factorial(candidates) / (factorial(size) * factorial(candidates - size));
    }

    // number of possible bags of given size composed from a set of candidates
    // (repetition is allowed, order does not matter)
    constexpr int bag(const int candidates, const int size)
    {
        return combination(candidates+size-1, size);
    }

    // number of possible permutations of given size composed from a set of candidates
    // (repetition is not allowed, order matters)
    constexpr int permutation(const int candidates, const int size)
    {
        return size > candidates? 0 : factorial(candidates) / factorial(candidates-size);
    }

    // number of possible sequences of given size composed from a set of candidates
    // (repetition is allowed, order matters)
    constexpr int sequence(const int candidates, const int size)
    {
        return std::pow(candidates,size);
    }

    // number of possible allocations of indistiguishable resources among candidate bags
    constexpr int allocation(const int resources, const int candidates)
    {
        return combination(resources+candidates-1, candidates-1);
    }

}
