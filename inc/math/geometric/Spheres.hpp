#pragma once

#include <glm/vec3.hpp>      // *vec3
#include <glm/geometric.hpp> // length

namespace geometric
{

  template<typename scalar>
  struct Spheres
  {
    const scalar pi;
    Spheres(const scalar pi):
      pi(pi)
    {}
    template<typename length>
    auto surface_area(const length radius) const {
        return scalar(4)*pi*radius*radius;
    }
    template<typename length>
    auto volume(const length radius) const {
        return scalar(4)/scalar(3)*pi*radius*radius*radius;
    }
    template<glm::qualifier quality=glm::defaultp>
    bool is_point_inside(
      const glm::vec<3,scalar,quality> point, 
      const glm::vec<3,scalar,quality> origin, 
      const scalar radius
    ) const {
      return glm::length(point-origin) < radius;
    }
    template<glm::qualifier quality=glm::defaultp>
    bool is_sphere_inside(
      const glm::vec<3,scalar,quality> inner_origin, 
      const scalar inner_radius,
      const glm::vec<3,scalar,quality> outer_origin, 
      const scalar outer_radius
    ) const {
      return glm::length(outer_origin-inner_origin) + inner_radius < outer_radius;
    }
    template<glm::qualifier quality=glm::defaultp>
    bool is_sphere_overlapped(
      const glm::vec<3,scalar,quality> origin1, 
      const scalar radius1,
      const glm::vec<3,scalar,quality> origin2, 
      const scalar radius2
    ) const {
      return glm::length(origin2-origin1) < radius2+radius1;
    }
    template<glm::qualifier quality=glm::defaultp>
    bool distance_from_surface_to_point(
      const glm::vec<3,scalar,quality> point, 
      const glm::vec<3,scalar,quality> origin, 
      const scalar radius
    ) const {
      return glm::length(point-origin) - radius;
    }
    template<glm::qualifier quality=glm::defaultp>
    bool distance_from_surface_to_sphere(
      const glm::vec<3,scalar,quality> origin1, 
      const scalar radius1,
      const glm::vec<3,scalar,quality> origin2, 
      const scalar radius2
    ) const {
      return glm::length(origin2-origin1) - (radius2+radius1);
    }
    /*
    template<typename scalar, glm::qualifier quality=glm::defaultp>
    maybe_range distance_to_surface_along_line(
      const glm::vec<3,scalar,quality> reference, 
      const glm::vec<3,scalar,quality> direction, 
      const glm::vec<3,scalar,quality> origin, 
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

  template<typename scalar>
  Spheres<scalar> spheres(const scalar pi)
  {
    return Spheres<scalar>(pi);
  }

}

