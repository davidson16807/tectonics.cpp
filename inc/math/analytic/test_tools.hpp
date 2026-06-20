#pragma once

/*
`ExpressionAdapter` uses a distance function 
that is defined for a range of input, [lo,hi]
as with most expressions in `math::`
*/
template<typename T>
struct ExpressionAdapter{
    const T threshold;
    const T lo;
    const T hi;

    ExpressionAdapter(const T threshold, const T lo, const T hi):
        threshold(threshold),
        lo(lo),
        hi(hi)
    {}

    template<typename T2, typename T3>
    bool equal(const T2& a, const T3& b) const {
        return analytic::distance(a, b, lo, hi) <= threshold;
    }
    bool equal(const T a, const T b) const {
        return std::abs(a-b) <= threshold;
    }

    template<typename T2>
    std::string print(const T2& a) const {
        std::ostringstream os;
        os << a;
        return os.str();
    }

};

