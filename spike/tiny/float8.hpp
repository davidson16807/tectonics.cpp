#include <cassert>         // assert
#include <iostream>        // std::cout
#include <iterator>        // std::ostream_iterator
#include <array>           // std::array
#include <algorithm>       // std::algorithm

template <float lo, float hi, bool islog>
class float8 {
    std::uint8_t v1;

public:
    ~float8()
    {
    }
    float8()
    {
    }

    inline size_type size()     const { return max_size(); }
    inline size_type capacity() const { return max_size(); }
    const unsigned int operator[](size_type index) const
    {
        assert(index < max_size());
        switch(index){
            case 0: return v0;
            case 1: return v1;
            default: return 0;
        }
    }
    unsigned int serialize() const
    {
        unsigned int output = v1;
        output <<= bit_count(); output |= v0;
        return output;
    }
    void deserialize(unsigned int input)
    {
        v0 = std::clamp(input & max_value(), 0u, max_value()); input >>= bit_count();
        v1 = std::clamp(input,               0u, max_value());
    }

    template<typename TIterator>
    TIterator decompress(TIterator output_iterator) const
    {
        *output_iterator++ = v0;
        *output_iterator++ = v1;
        return output_iterator;
    }
    template<typename TIterator>
    TIterator compress(TIterator input_iterator)
    {
        v0 = std::clamp(int(*input_iterator++), 0, max_value());
        v1 = std::clamp(int(*input_iterator++), 0, max_value());
        return input_iterator;
    }
};

// This template represents a contiguous block of stack memory occupied by primitive data of the same arbitrary type
// T is a data type 
template <class TInputIterator, class TOutputIterator>
TOutputIterator flatten (TInputIterator first, TInputIterator last, TOutputIterator output) {
    for (; first!=last; first++) {
        output = first->decompress(output);
    }
    return output;
}
// This template represents a contiguous block of stack memory occupied by primitive data of the same arbitrary type
// T is a data type 
template <class TInputIterator, class TOutputIterator>
TOutputIterator unflatten (TInputIterator first, TInputIterator last, TOutputIterator output) {
    for (; first!=last; output++) {
        first = output->compress(first);
    }
    return output;
}

// template <class TContainer>
// std::ostream& operator<<(std::ostream& os, const TContainer& c)
// {
//     std::copy(c.begin(),
//               c.end(),
//               std::ostream_iterator<typename TContainer::value_type>(os, " "));
//     return os;
// }
// int main()
// {
//     std::array<float,2> representation1 = {14,16};
//     std::array<float,2> representation2;
//     std::array<float,4> representation3;
//     std::array<float8,2> model1 = {float8(14,16), float8(1,2)};
//     std::array<float8,2> model2 = {float8(), float8()};

//     float8 a = float8();
//     std::cout << sizeof a << std::endl;

//     a.compress(representation1.begin());
//     a.decompress(representation2.begin());
//     std::cout << model1[1][1] << std::endl;
//     std::cout << representation2 << std::endl;

//     flatten(model1.begin(), model1.end(), representation3.begin());
//     unflatten(representation3.begin(), representation3.end(), model2.begin());
//     std::cout << representation3 << std::endl;
//     std::cout << model2[0][1] << std::endl;

//     unsigned int canonical = a.serialize();
//     std::cout << canonical << std::endl;
//     a.deserialize(canonical);
//     canonical = a.serialize();
//     std::cout << canonical << std::endl;
// }