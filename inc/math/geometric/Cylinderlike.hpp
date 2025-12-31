#pragma once

// C libraries
#include <cmath> // std::sqrt

// std libraries
#include <algorithm> // std::clamp

// 3rd-party libraries
#include <glm/vec3.hpp>      // *vec3
#include <glm/geometric.hpp> // length

#include "Lines.hpp" // Lines

namespace geometric
{

  /*
  `Cylinderlike` addresses any geometry that is offset from an axis (such as a line, line segment, or ray) 
  by a perpendicular vector of bounded magnitude. This includes cylinders, cylinder rays, and infinite cylinders. 
  This definition generalizes to any number of dimensions, N.
  */
  template<int N, typename Axis, typename scalar>
  struct Cylinderlike
  {

    using vector = glm::vec<N,scalar,quality>;

    const Axis axis;
    const Lines<N,scalar> line;
    const scalar pi;

    Cylinderlike(const Axis& axis, const scalar pi):
      axis(axis),
      line(),
      pi(pi)
    {}

    template<typename length>
    constexpr inline auto circumference(const length radius) const {
        return scalar(2)*pi*radius;
    }
    template<typename length>
    constexpr inline auto surface_area(const length height, const length radius) const {
        return scalar(2)*pi*radius*(radius+height);
    }
    template<typename length>
    constexpr inline auto volume(const length height, const length radius) const {
        return pi*radius*radius*height;
    }

    template<glm::qualifier quality=glm::defaultp>
    scalar surface_distance_to_point(
      const vector capsule_reference, 
      const vector capsule_direction, 
      const scalar capsule_radius, 
      const vector point
    ) const {
      scalar radial_distance(line.nearest_distance_to_point(capsule_reference, glm::normalize(capsule_direction), point));
      scalar nearest_distance(axis.nearest_distance_to_point(capsule_reference, capsule_direction, point));
      scalar axial_distance(std::sqrt(nearest_distance*nearest_distance - radial_distance*radial_distance));
      return std::max(radial_distance - capsule_radius, axial_distance);
    }

    // template<glm::qualifier quality=glm::defaultp>
    // inline scalar surface_normal_at_point(
    //   const vector capsule_reference, 
    //   const vector capsule_direction, 
    //   const scalar capsule_radius, 
    //   const vector point
    // ) const {
    //   scalar radial_distance(line.nearest_distance_to_point(capsule_reference, glm::normalize(capsule_direction), point));
    //   scalar nearest_distance(axis.nearest_distance_to_point(capsule_reference, capsule_direction, point));
    //   scalar axial_distance(std::sqrt(nearest_distance*nearest_distance - radial_distance*radial_distance));
    //   return radial_distance - capsule_radius > axial_distance? : ;
    // }

    template<glm::qualifier quality=glm::defaultp>
    inline bool is_point_inside(
      const vector point, 
      const vector capsule_reference, 
      const vector capsule_direction, 
      const scalar capsule_radius
    ) const {
      return surface_distance_to_point(capsule_reference, capsule_direction, capsule_radius, point) < scalar(0);
    }

    /*
    template<glm::qualifier quality=glm::defaultp>
    bool distance_along_line_nearest_to_line(
      const glm::vector<3,scalar,quality> reference1, 
      const glm::vector<3,scalar,quality> direction1, 
      const glm::vector<3,scalar,quality> reference2, 
      const glm::vector<3,scalar,quality> direction2
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
  Cylinderlike<N,Axis,scalar> cylinderlike(const Axis& axis, const scalar pi)
  {
    return Cylinderlike<N,Axis,scalar>(axis, pi);
  }

}


