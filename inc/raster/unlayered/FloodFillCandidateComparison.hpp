#pragma once

#include <utility> // std::pair

namespace unlayered
{
    template<typename id, typename scalar>
    struct FloodFillCandidateComparison
    {
        FloodFillCandidateComparison(){}
        bool operator()(const std::pair<id,scalar> a, const std::pair<id,scalar> b)
        {
            return a.second > b.second;
        }
    };
}
