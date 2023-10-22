#pragma once

// std libraries
#include <string>  // to_string

// 3rd party libraries
#include <glm/geometric.hpp>        // distance
#include <glm/gtx/string_cast.hpp>  // to_string

// NOTE: no natural distance function exists on a Point 
// that does not require the implementation of the very things we are testing.
// This is why we implement a custom adapter.
struct DymaxionAdapter{

    const double threshold;

    DymaxionAdapter(double threshold=1e-4):
        threshold(threshold)
    {}

    template<typename id, typename scalar>
    bool equal(const healpix::Point<id,scalar>& a, const healpix::Point<id,scalar>& b) const {
        return 
            a.square_id == b.square_id && 
            glm::distance(glm::vec2(a.square_position), glm::vec2(b.square_position)) <= scalar(1e-4); 
    }

    template<int L, typename T, glm::qualifier Q>
    bool equal(const glm::vec<L,T,Q>& a, const glm::vec<L,T,Q>& b) const {
        return glm::distance(a, b) < threshold; 
    }

    bool equal(const int& a, const int& b) const {
        return a==b; 
    }

    template<typename id, typename scalar>
    std::string print(const healpix::Point<id,scalar>& a) const {
        return healpix::to_string(a);
    }

    template<int L, typename T, glm::qualifier Q>
    std::string print(const glm::vec<L,T,Q>& v) const {
        return glm::to_string(v);
    }

    std::string print(const int& v) const {
        return std::to_string(v);
    }

};
