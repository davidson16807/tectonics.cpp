#pragma once

#include <glm/vec3.hpp>      // *vec3
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
    LineSegments()
    {}
    template<glm::qualifier quality=glm::defaultp>
    inline scalar distance_along_direction_nearest_to_point(
      const glm::vec<N,scalar,quality> segment_origin, 
      const glm::vec<N,scalar,quality> segment_offset, 
      const glm::vec<N,scalar,quality> point
    ) const {
      scalar segment_length(glm::length(segment_offset));
      return std::clamp(
        glm::dot(point - segment_origin, segment_offset/segment_length),
        scalar(0), segment_length
      );
    }

    template<glm::qualifier quality=glm::defaultp>
    scalar nearest_offset_to_point(
      const glm::vec<N,scalar,quality> segment_origin, 
      const glm::vec<N,scalar,quality> segment_offset, 
      const glm::vec<N,scalar,quality> point
    ) const {
      scalar segment_length(glm::length(segment_offset));
      glm::vec<N,scalar,quality> rejection = point - 
        distance_along_direction_nearest_to_point(segment_origin, segment_offset, point) * 
        glm::normalize(segment_offset);
      return rejection-segment_origin;
    }

    template<glm::qualifier quality=glm::defaultp>
    inline scalar nearest_distance_to_point(
      const glm::vec<N,scalar,quality> segment_origin, 
      const glm::vec<N,scalar,quality> segment_offset, 
      const glm::vec<N,scalar,quality> point
    ) const {
      return glm::length(nearest_offset_to_point(segment_origin, segment_offset, point));
    }

    /*
    template<glm::qualifier quality=glm::defaultp>
    bool distance_along_line_nearest_to_line(
      const glm::vec<3,scalar,quality> reference1, 
      const glm::vec<3,scalar,quality> direction1, 
      const glm::vec<3,scalar,quality> reference2, 
      const glm::vec<3,scalar,quality> direction2
    ) const {
      const vec3 A = direction1;
      const vec3 B = direction2;
      const vec3 A0 = reference1;
      const vec3 B0 = reference2;
      const vec3 D = B0 - A0;
      // offset
      const vec3 C = normalize(cross(B, A));
      // cross
      const vec3 R = D - dot(D, A) * A - dot(D, C) * C;
      // rejection
      return maybe_float(
          length(R) / -dot(B, normalize(R)), 
          abs(abs(dot(A, B)) - 1.0) > 0.0
      );
    }
    */

  };

  template<typename scalar>
  LineSegments<scalar> line_segments(const scalar pi)
  {
    return LineSegments<scalar>(pi);
  }

}


