#include <cassert>         // assert
#include <iterator>        // std::ostream_iterator
#include <array>           // std::array
#include <algorithm>       // std::algorithm

class uint2byte {
    std::uint8_t v0 : 2;
    std::uint8_t v1 : 2;
    std::uint8_t v2 : 2;
    std::uint8_t v3 : 2;

public:
    ~uint2byte()
    {
    }
    uint2byte(): 
        v0(0), v1(0), v2(0), v3(0)
    {
    }
    /*
    uint2byte(unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3): 
        v0(std::clamp(v0, 0u, max_value())), 
        v1(std::clamp(v1, 0u, max_value())),
        v2(std::clamp(v2, 0u, max_value())),
        v3(std::clamp(v3, 0u, max_value()))
    {
    }
    */

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

//     uint2byte a = uint2byte();
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