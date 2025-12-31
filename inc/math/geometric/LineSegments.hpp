#pragma once

// std libraries
#include <algorithm> // std::clamp

// 3rd-party libraries
#include <glm/vec3.hpp>      // *vector3
#include <glm/geometric.hpp> // length

namespace geometric
{

  /*
  `Lines` addresses any geometry that is an offset from a point 
  by a vector of a known direction and bounded length. 
  Both direction and length are represented using an offset vector
  This definition applies for any number of dimensions, N.
  */
  template<int N, typename scalar>
  struct LineSegments
  {

    using vector = glm::vec<N,scalar,quality>;

    LineSegments()
    {}

    template<glm::qualifier quality=glm::defaultp>
    inline scalar nearest_distance_along_to_point(
      const vector segment_origin, 
      const vector segment_offset, 
      const vector point
    ) const {
      scalar segment_length(glm::length(segment_offset));
      return std::clamp(
        glm::dot(point - segment_origin, segment_offset/segment_length),
        scalar(0), segment_length
      );
    }

    template<glm::qualifier quality=glm::defaultp>
    vector nearest_offset_to_point(
      const vector segment_origin, 
      const vector segment_offset, 
      const vector point
    ) const {
      scalar segment_length(glm::length(segment_offset));
      vector rejection = point - 
        distance_along_direction_nearest_to_point(segment_origin, segment_offset, point) * 
        glm::normalize(segment_offset);
      return rejection-segment_origin;
    }

    template<glm::qualifier quality=glm::defaultp>
    inline scalar nearest_distance_to_point(
      const vector segment_origin, 
      const vector segment_offset, 
      const vector point
    ) const {
      return glm::length(nearest_offset_to_point(segment_origin, segment_offset, point));
    }

    /*
    template<glm::qualifier quality=glm::defaultp>
    bool distance_along_line_nearest_to_line(
      const glm::vector<3,scalar,quality> reference1, 
      const glm::vector<3,scalar,quality> direction1, 
      const glm::vector<3,scalar,quality> reference2, 
      const glm::vector<3,scalar,quality> direction2
    ) const {
      const vector3 A = direction1;
      const vector3 B = direction2;
      const vector3 A0 = reference1;
      const vector3 B0 = reference2;
      const vector3 D = B0 - A0;
      // offset
      const vector3 C = normalize(cross(B, A));
      // cross
      const vector3 R = D - dot(D, A) * A - dot(D, C) * C;
      // rejection
      return maybe_float(
          length(R) / -dot(B, normalize(R)), 
          abs(abs(dot(A, B)) - 1.0) > 0.0
      );
    }
    */

  };

}

