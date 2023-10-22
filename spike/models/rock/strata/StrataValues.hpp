#pragma once

#include <array>

namespace strata
{
    template <typename T, std::size_t L>
    struct StrataValues
    {
        std::array<T, L> values;
        unsigned int count;
        ~StrataValues(){}
        StrataValues(): values(), count(0){}
        StrataValues(std::initializer_list<T> list): 
        	values(), 
        	count(list.size()
		){
        	assert(count <= list.size());
        	std::copy(list.begin(), list.end(), values.begin());
        }
    };
}
