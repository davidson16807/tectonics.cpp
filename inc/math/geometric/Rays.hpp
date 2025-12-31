#pragma once

// std libraries
#include <algorithm> // std::max

// 3rd-party libraries
#include <glm/vec3.hpp>      // *vector3
#include <glm/geometric.hpp> // length

namespace geometric
{

  /*
  `Rays` addresses any geometry that is an offset from a point by a vector of a known direction.
  This definition applies for any number of dimensions, N.
  */
  template<int N, typename scalar>
  struct Rays
  {

    using vector = glm::vec<N,scalar,quality>;

    Rays()
    {}

    template<glm::qualifier quality=glm::defaultp>
    inline scalar nearest_distance_along_to_point(
      const vector ray_reference, 
      const vector ray_offset, 
      const vector point
    ) const {
      scalar ray_length(glm::length(ray_offset));
      return std::max(
        scalar(0), 
        nearest_distance_to_point(ray_reference, ray_offset/ray_length, point),
      );
    }

    template<glm::qualifier quality=glm::defaultp>
    vector nearest_offset_to_point(
      const vector ray_reference, 
      const vector ray_direction, 
      const vector point
    ) const {
      vector rejection = 
        point - nearest_distance_along_to_point(ray_reference, ray_direction, point) * ray_direction;
      return rejection-ray_reference;
    }

    template<glm::qualifier quality=glm::defaultp>
    inline scalar nearest_distance_to_point(
      const vector ray_reference, 
      const vector ray_offset, 
      const vector point
    ) const {
      return glm::length(nearest_offset_to_point(ray_reference, ray_offset, point));
    }

    /*
    template<glm::qualifier quality=glm::defaultp>
    bool distance_along_ray_nearest_to_line(
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


