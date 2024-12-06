#pragma once

#include <glm/vec3.hpp>      // *vec3
#include <glm/geometric.hpp> // length

namespace geometric
{

  /*
  `Capsulelike` addresses any geometry that is offset from an axis (such as a line, line segment, or ray) 
  by a vector of known magnitude. This includes capsules, capsule rays, and infinite cylinders. 
  This definition generalizes to any number of dimensions, N.
  */
  template<int N, typename Axis, typename scalar>
  struct Capsulelike
  {
    const Axis axis;

    Capsulelike(const Axis& axis):
      axis(axis)
    {}

    template<glm::qualifier quality=glm::defaultp>
    inline scalar surface_distance_to_point(
      const glm::vec<N,scalar,quality> capsule_reference, 
      const glm::vec<N,scalar,quality> capsule_direction, 
      const scalar capsule_radius, 
      const glm::vec<N,scalar,quality> point
    ) const {
      return glm::length(axis.nearest_offset_to_point(capsule_reference, capsule_direction, point)) - capsule_radius;
    }

    template<glm::qualifier quality=glm::defaultp>
    inline scalar surface_normal_at_point(
      const glm::vec<N,scalar,quality> capsule_reference, 
      const glm::vec<N,scalar,quality> capsule_direction, 
      const scalar capsule_radius, 
      const glm::vec<N,scalar,quality> point
    ) const {
      return glm::normalize(axis.nearest_offset_to_point(capsule_reference, capsule_direction, point));
    }

    template<glm::qualifier quality=glm::defaultp>
    inline bool is_point_inside(
      const glm::vec<N,scalar,quality> point, 
      const glm::vec<N,scalar,quality> capsule_reference, 
      const glm::vec<N,scalar,quality> capsule_direction, 
      const scalar capsule_radius
    ) const {
      return surface_distance_to_point(capsule_reference, capsule_direction, capsule_radius, point) < scalar(0);
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
    distance_along_line_nearest_to_point
    distance_along_line_nearest_to_line
    distance_along_line_nearest_to_ray
    distance_along_line_nearest_to_line_segment
    */
  };

  template<int N, typename scalar, typename Axis>
  CapsuleLike<N,Axis,scalar> capsulelike(const Axis& axis)
  {
    return CapsuleLike<N,Axis,scalar>(pi);
  }

}


