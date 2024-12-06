#pragma once

#include <glm/vec3.hpp>      // *vec3
#include <glm/geometric.hpp> // length

namespace geometric
{

  /*
  `Planelike` addresses any geometry that is defined as an offset from a plane 
  by a vector of known direction (the "surface normal"). This includes planes and halfspaces. 
  These geometries are defined the same way for any number of dimensions, L.
  */
  template<int L, typename scalar>
  struct Planelike
  {
    Planelike()
    {}
    template<glm::qualifier quality=glm::defaultp>
    inline scalar distance_from_plane_to_point(
      const glm::vec<L,scalar,quality> halfspace_reference, 
      const glm::vec<L,scalar,quality> halfspace_normal,
      const glm::vec<L,scalar,quality> point
    ) const {
      return glm::dot(point-halfspace_reference, halfspace_normal);
    }
    template<glm::qualifier quality=glm::defaultp>
    bool is_point_in_halfspace(
      const glm::vec<L,scalar,quality> point, 
      const glm::vec<L,scalar,quality> halfspace_reference, 
      const glm::vec<L,scalar,quality> halfspace_normal
    ) const {
      return distance_from_plane_to_point(halfspace_reference, halfspace_normal, point) < scalar(0);
    }
    /*
    template<typename scalar, glm::qualifier quality=glm::defaultp>
    maybe_float distance_along_line_to_plane(
      const glm::vec<L,scalar,quality> line_reference, 
      const glm::vec<L,scalar,quality> line_direction, 
      const glm::vec<L,scalar,quality> halfspace_reference, 
      const glm::vec<L,scalar,quality> halfspace_normal
    ){
        return maybe_float( 
          -dot(line_reference - halfspace_reference, halfspace_normal) / 
            dot(line_direction, halfspace_normal), 
           abs(dot(line_direction, halfspace_normal)) < SMALL
        );
    }
    */
  };

  template<typename scalar>
  Planelike<scalar> planelike(const scalar pi)
  {
    return Planelike<scalar>(pi);
  }

}

