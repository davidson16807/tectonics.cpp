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
are densely packed into small data types such as uint8s or uint2s.
There are typically many such attributes within a single data structure,
and they typically represent real numbers that in ideal situations would be stored within floats.
Each attribute has values that commonly fall within a known range.
Some may require precision on a linear scale, while others may require precision on a log scale.
We may try manipulating some attributes within these data structures, 
but when doing so we must consider how they are represented as integers,
since the amount incremented over a single timestep may be to small to represent within a uint8s or uint2s.
We must also avoid overflow or underflow errors when doing so.
Since uint2s are represented differently within memory depending on platform,
we must also find a way to store these attributes on a hard drive using a single canonical representation.

A "uint2_byte" is a data structure that addresses these considerations.
It represents a 4 byte word that is populated by an array of 4 bit unsigned integers.
It is the smallest size collection of uint2s that can be addressed
It hides the underyling uint2s since their modification is highly error prone.
It instead offers conversions to and from several less restrictive formats,
each of which suite different use cases.

There are 3 formats it is intended to convert between:
* a "canonical" byte representation that can be stored on a hard drive and shared across computers 
* an integer representation of a parameter space that can be easily modified 
  without causing errors related to precision, overflow, or underflow.
* a floating point representation that maps to a set of numbers that can be easily interpreted by the user

The class is designed around offering the following guaratees:
* The data structure consumes 1 byte, regardless of hardware
* The underlying uint2s do not underflow or overflow, only clamp.
* There is a canonical representation for the uint2_byte that can be shared across machines.
* The canonical representation consumes the same amount of space as the data structure itself.
*/
class uint2_byte {
    std::uint8_t v0 : 2;
    std::uint8_t v1 : 2;
    std::uint8_t v2 : 2;
    std::uint8_t v3 : 2;

public:
    ~uint2_byte()
    {
    }
    uint2_byte(): 
        v0(0), v1(0), v2(0), v3(0)
    {
    }
    uint2_byte(unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3): 
        v0(std::clamp(v0, 0u, max_value())), 
        v1(std::clamp(v1, 0u, max_value())),
        v2(std::clamp(v2, 0u, max_value())),
        v3(std::clamp(v3, 0u, max_value()))
    {
    }

    inline size_type max_value()  const { return 0x7; }
    inline size_type bit_count() const { return 2; }
    /*
    stl-like container methods
    NOTE: we do not include .begin(), .end(), or operator[] assignment,
    since the individual uint2s are too small to uniquely address by pointers
    */
    typedef int size_type;
    inline size_type max_size() const { return 4; }
    inline size_type size()     const { return max_size(); }
    inline size_type capacity() const { return max_size(); }
    const unsigned int operator[](size_type index) const
    {
        assert(index < max_size());
        switch(index){
            case 0: return v0;
            case 1: return v1;
            case 2: return v2;
            case 3: return v3;
            default: return 0;
        }
    }
    unsigned int serialize() const
    {
        unsigned int output = v3;
        output <<= bit_count(); output |= v2;
        output <<= bit_count(); output |= v1;
        output <<= bit_count(); output |= v0;
        return output;
    }
    void deserialize(unsigned int input)
    {
        v0 = std::clamp(input & max_value(), 0u, max_value()); input >>= bit_count();
        v1 = std::clamp(input & max_value(), 0u, max_value()); input >>= bit_count();
        v2 = std::clamp(input & max_value(), 0u, max_value()); input >>= bit_count();
        v3 = std::clamp(input,               0u, max_value());
    }

    template<typename TIterator>
    TIterator decompress(TIterator output_iterator) const
    {
        *output_iterator++ = v0;
        *output_iterator++ = v1;
        *output_iterator++ = v2;
        *output_iterator++ = v3;
        return output_iterator;
    }
    template<typename TIterator>
    TIterator compress(TIterator input_iterator)
    {
        v0 = std::clamp(int(*input_iterator++), 0, max_value());
        v1 = std::clamp(int(*input_iterator++), 0, max_value());
        v2 = std::clamp(int(*input_iterator++), 0, max_value());
        v3 = std::clamp(int(*input_iterator++), 0, max_value());
        return input_iterator;
    }
};

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
//     std::array<float,4> original = {14,16,1,2};
//     std::array<float,4> reconstituted;

//     uint2_byte a = uint2_byte();
//     std::cout << sizeof a << std::endl;

//     a.compress(original.begin());
//     a.decompress(reconstituted.begin());
//     std::cout << reconstituted << std::endl;

//     unsigned int canonical = a.serialize();
//     std::cout << canonical << std::endl;
//     a.deserialize(canonical);
//     canonical = a.serialize();
//     std::cout << canonical << std::endl;
// }