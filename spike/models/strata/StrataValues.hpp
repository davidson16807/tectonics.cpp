#pragma once

#include <array>

namespace strata
{
    template <typename T, int L>
    struct StrataValues
    {
        std::array<T, L> values;
        int count;
        ~StrataValues(){}
        StrataValues(): values(), count(0){}
    };
}
