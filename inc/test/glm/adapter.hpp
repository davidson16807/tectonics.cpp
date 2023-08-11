#pragma once

// 3rd party libraries
#define GLM_FORCE_PURE          // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL // disable anonymous structs so we can build with ISO C++
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>  // to_string

namespace test {

template<typename id, typename scalar>
struct GlmAdapter{
    const scalar threshold;

    GlmAdapter(scalar threshold):
        threshold(threshold)
    {}

    bool equal(const scalar& a, const scalar& b) const {
        return abs(a - b) <= threshold;
    }

    bool equal(const id& a, const id& b) const {
        return a == b;
    }

    template<int L, glm::qualifier Q>
    bool equal(const glm::vec<L,scalar,Q> a, const glm::vec<L,scalar,Q> b) const {
        return distance(a,b) < threshold;
    }

    template<int L, glm::qualifier Q>
    bool equal(const glm::vec<L,id,Q> a, const glm::vec<L,id,Q> b) const {
        return a == b;
    }

    std::string print(const scalar a) const {
        std::ostringstream os;
        os << a;
        return os.str();
    }

    std::string print(const id a) const {
        std::ostringstream os;
        os << a;
        return os.str();
    }

    template<int L, glm::qualifier Q>
    std::string print(const glm::vec<L,scalar,Q> a) const {
        return to_string(a);
    }

    template<int L, glm::qualifier Q>
    std::string print(const glm::vec<L,id,Q> a) const {
        return to_string(a);
    }

};

}
