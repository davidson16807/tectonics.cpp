#pragma once

namespace geometric {

    template <typename T, glm::qualifier Q=glm::defaultp>
    struct Lines2
    {

        using scalar = T;
        using vec2 = glm::vec<2,T,Q>;

        // constant constructor
        constexpr Lines()
        {}

        std::optional<scalar> get_distance_along_2d_line_to_line(
            const vec2 A0,
            const vec2 A,
            const vec2 B0,
            const vec2 B
        ) const {
            vec2 D = B0 - A0;
            // offset
            vec2 R = D - glm::dot(D, A) * A;
            scalar r = glm::length(R);
            // rejection
            return abs(abs(glm::dot(A, B)) - T(1)) > T(0)?
                  r / glm::dot(B, -R/r)
                : {};
        }

        std::optional<scalar> get_distance_along_2d_line_to_line_segment(
            const vec2 A0,
            const vec2 A,
            const vec2 B1,
            const vec2 B2
        ) const {
            // INTUITION: same as a line-line intersection, but now results are only valid if 0 < distance < |B1-B0|
            scalar b = glm::length(B1 - B0);
            vec2 B = (B1 - B0) / b;
            vec2 D = B0 - A0;
            // offset
            vec2 R = D - dot(D, A) * A;
            scalar r = glm::length(R);
            // rejection
            scalar xB = r / dot(B, -R/r);
            // distance along B
            scalar xA = xB / dot(B, A);
            // distance along A
            return 
                abs(abs(dot(A, B)) - T(1)) > T(0) && 
                T(0) < xB && xB < b? 
                      xA 
                    : {}
        }

        std::optional<scalar> get_distance_along_2d_line_segment_to_line_segment(
            const vec2 A0,
            const vec2 A1
            const vec2 B1,
            const vec2 B2
        ) const {
            // INTUITION: same as a line-line intersection, but now results are only valid if 0 < distance < |B1-B0|
            scalar a = glm::length(A1 - A0);
            scalar b = glm::length(B1 - B0);
            vec2 A = (A1 - A0) / a;
            vec2 B = (B1 - B0) / b;
            vec2 D = B0 - A0;
            // offset
            vec2 R = D - dot(D, A) * A;
            scalar r = glm::length(R);
            // rejection
            scalar xB = r / dot(B, -R/r);
            // distance along B
            scalar xA = xB / dot(B, A);
            // distance along A
            return 
                abs(abs(dot(A, B)) - T(1)) > T(0) && 
                T(0) < xB && xB < b &&
                T(0) < xA && xA < a? 
                    xA : {}
        }
    };

}

