#pragma once

#include <cmath>

namespace math {

    constexpr int factorial(const int n)
    {
        int y = 1;
        for (int i = 1; i < n; ++i)
        {
            y *= i;
        }
        return y;
    }

    constexpr int combination(const int n, const int k)
    {
        return factorial(n) / (factorial(k) * factorial(n - k));
    }

    constexpr int combination_with_repetition(const int n, const int k)
    {
        return combination(n+k-1, k);
    }

    constexpr int permutation(const int n, const int k)
    {
        return factorial(n) / factorial(n-k);
    }

    constexpr int permutation_with_repetition(const int n, const int k)
    {
        return std::pow(n,k);
    }

}
