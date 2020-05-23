#include <cassert>         // assert
#include <iostream>        // std::cout
#include <iterator>        // std::ostream_iterator
#include <array>           // std::array
#include <algorithm>       // std::algorithm
/*
Several places in the library require storing hundreds of MB worth of contiguous data structures.
The data structures are often accessed out of order, and so data locality has a huge impact on performance.
Optimizing for performance often reduces to a matter of minimizing memory footprint.
To reduce memory footprint, the attributes of the most frequently used data structures 
are densely packed into small data types such as uint8s or uint4s.
There are typically many such attributes within a single data structure,
and they typically represent real numbers that in ideal situations would be stored within floats.
Each attribute has values that commonly fall within a known range.
Some may require precision on a linear scale, while others may require precision on a log scale.
We may try manipulating some attributes within these data structures, 
but when doing so we must consider how they are represented as integers,
since the amount incremented over a single timestep may be to small to represent within a uint8s or uint4s.
We must also avoid overflow or underflow errors when doing so.
Since uint4s are represented differently within memory depending on platform,
we must also find a way to store these attributes on a hard drive using a single canonical representation.

A "uint4_byte" is a data structure that addresses these considerations.
It represents a 4 byte word that is populated by an array of 4 bit unsigned integers.
It is the smallest size collection of uint4s that can be addressed
It hides the underyling uint4s since their modification is highly error prone.
It instead offers conversions to and from several less restrictive formats,
each of which suite different use cases.

There are 3 formats it is intended to convert between:
* a "canonical" byte representation that can be stored on a hard drive and shared across computers 
* an integer representation of a parameter space that can be easily modified 
  without causing errors related to precision, overflow, or underflow.
* a floating point representation that maps to a set of numbers that can be easily interpreted by the user
*/
class uint4_byte {
    std::uint8_t v0 : 4;
    std::uint8_t v1 : 4;

public:
    uint4_byte(): 
        v0(0), v1(0)
    {
    }
    uint4_byte(unsigned int v0, unsigned int v1): 
        v0(std::clamp(v0, 0u, 0xFu)), 
        v1(std::clamp(v1, 0u, 0xFu))
    {
    }

    /*
    stl-like container methods
    NOTE: we do not include .begin(), .end(), or operator[] assignment,
    since the individual uint4s are too small to uniquely address by pointers
    */
    typedef int size_type;
    inline size_type max_size() const { return 2; }
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
        output <<= 4; output |= v0;
        return output;
    }
    void deserialize(unsigned int input)
    {
        v0 = std::clamp(input & 0xF, 0u, 0xFu); input >>= 4;
        v1 = std::clamp(input,       0u, 0xFu);
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
        v0 = std::clamp(int(*input_iterator++), 0, 0xF);
        v1 = std::clamp(int(*input_iterator++), 0, 0xF);
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
//     std::array<uint4_byte,2> model1 = {uint4_byte(14,16), uint4_byte(1,2)};
//     std::array<uint4_byte,2> model2 = {uint4_byte(), uint4_byte()};

//     uint4_byte a = uint4_byte();
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