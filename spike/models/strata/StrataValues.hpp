#pragma once

#include <array>

namespace strata
{
    template <typename T, std::size_t L>
    struct StrataValues
    {
        std::array<T, L> values;
        uint count;
        ~StrataValues(){}
        StrataValues(): values(), count(0){}
    };
}
