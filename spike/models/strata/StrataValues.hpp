#pragma once

#include <array>

namespace strata
{
    template <typename T>
    struct StrataValues
    {
        std::array<T, Strata::max_stratum_count> values;
        int count;
        ~StrataValues(){}
        StrataValues(): values(), count(0){}
    };
}
