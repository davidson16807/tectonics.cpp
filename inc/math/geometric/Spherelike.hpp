#pragma once

#include <glm/vec3.hpp>      // *vec3
#include <glm/geometric.hpp> // length

namespace geometric
{

  /*
  `Spherelike` addresses any geometry that is an offset from a point by a vector of known magnitude (the "radius").
  This includes points and spheres. These geometries are defined the same way for any number of dimensions, L.
  */
  template<int L, typename scalar>
  struct Spherelike
  {
    const scalar pi;
    constexpr Spherelike(const scalar pi):
      pi(pi)
    {}
    template<typename length>
    constexpr inline auto circumference(const length radius) const {
        return scalar(2)*pi*radius;
    }
    template<typename length>
    constexpr inline auto surface_area(const length radius) const {
        return scalar(4)*pi*radius*radius;
    }
    template<typename length>
    constexpr inline auto volume(const length radius) const {
        return scalar(4)/scalar(3)*pi*radius*radius*radius;
    }
    template<glm::qualifier quality=glm::defaultp>
    scalar surface_distance_from_sphere_to_point(
      const glm::vec<L,scalar,quality> point, 
      const glm::vec<L,scalar,quality> origin, 
      const scalar radius
    ) const {
      return glm::length(point-origin) - radius;
    }
    template<glm::qualifier quality=glm::defaultp>
    scalar surface_normal_for_sphere_at_point(
      const glm::vec<L,scalar,quality> point, 
      const glm::vec<L,scalar,quality> origin, 
      const scalar radius
    ) const {
      return glm::normalize(point-origin);
    }
    template<glm::qualifier quality=glm::defaultp>
    bool is_point_in_sphere(
      const glm::vec<L,scalar,quality> point, 
      const glm::vec<L,scalar,quality> origin, 
      const scalar radius
    ) const {
      return glm::length(point-origin) < radius;
    }
    /*
    template<typename scalar, glm::qualifier quality=glm::defaultp>
    maybe_range distance_to_surface_along_line(
      const glm::vec<L,scalar,quality> reference, 
      const glm::vec<L,scalar,quality> direction, 
      const glm::vec<L,scalar,quality> origin, 
      const scalar radius
    ) const {
      float xz = glm::dot(origin - reference, direction);
      float z = glm::length(reference + direction * xz - origin);
      float y2 = r * r - z * z;
      float dxr = std::sqrt(std::max(y2, 1e-10));
      return maybe_range(
          xz - dxr,
          xz + dxr, 
          y2 > 0.
      );
    }
    maybe_circle intersection_with_sphere(
    ){
      return maybe_circle(O,N,r,exists);
    }
    */
  };

  template<int L, typename scalar>
  Spherelike<L,scalar> points_and_spheres(const scalar pi)
  {
    return Spherelike<L,scalar>(pi);
  }

}



