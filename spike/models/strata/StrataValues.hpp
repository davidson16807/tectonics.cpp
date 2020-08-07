#pragma once

#include <array>

namespace strata
{
    template <int L, typename T>
    struct StrataValues
    {
        std::array<T, L> values;
        int count;
        ~StrataValues(){}
        StrataValues(): values(), count(0){}
    };
}
