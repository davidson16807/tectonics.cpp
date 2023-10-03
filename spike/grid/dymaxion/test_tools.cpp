#pragma once

// NOTE: no natural distance function exists on a Point 
// that does not require the implementation of the very things we are testing.
// This is why we implement a custom adapter.
struct DymaxionAdapter{

    DymaxionAdapter() {}

    template<typename id, typename scalar>
    bool equal(const dymaxion::Point<id,scalar>& a, const dymaxion::Point<id,scalar>& b) const {
        return 
            a.square_id == b.square_id && 
            glm::distance(glm::vec2(a.square_position), glm::vec2(b.square_position)) <= scalar(1e-4); 
    }

    template<int L, typename T, glm::qualifier Q>
    bool equal(const glm::vec<L,T,Q>& a, const glm::vec<L,T,Q>& b) const {
        return glm::distance(a, b) < 1e-4; 
    }

    template<typename id, typename scalar>
    std::string print(const dymaxion::Point<id,scalar>& a) const {
        return dymaxion::to_string(a);
    }

    template<int L, typename T, glm::qualifier Q>
    std::string print(const glm::vec<L,T,Q>& v) const {
        return glm::to_string(v);
    }

};
