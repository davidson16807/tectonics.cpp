#pragma once

#include <glm/vec3.hpp>      // *vec3
#include <glm/geometric.hpp> // length

namespace geometric
{

  /*
  `Halfspace` addresses any geometry that is defined as an offset from a plane 
  by a vector of known direction (the "surface normal"). 
  This can be useful when dealing with planes as well.
  These geometries are defined the same way for any number of dimensions, L.
  */
  template<int L, typename scalar>
  struct Halfspace
  {

    using vector = glm::vec<N,scalar,quality>;

    Halfspace()
    {}

    template<glm::qualifier quality=glm::defaultp>
    inline scalar surface_distance_to_point(
      const vector halfspace_reference, 
      const vector halfspace_normal,
      const vector point
    ) const {
      return glm::dot(point-halfspace_reference, halfspace_normal);
    }

    template<glm::qualifier quality=glm::defaultp>
    bool is_point_inside(
      const vector point, 
      const vector halfspace_reference, 
      const vector halfspace_normal
    ) const {
      return surface_distance_to_point(halfspace_reference, halfspace_normal, point) < scalar(0);
    }

    /*
    template<typename scalar, glm::qualifier quality=glm::defaultp>
    maybe_float distance_along_line_to_plane(
      const vector line_reference, 
      const vector line_direction, 
      const vector halfspace_reference, 
      const vector halfspace_normal
    ){
        return maybe_float( 
          -dot(line_reference - halfspace_reference, halfspace_normal) / 
            dot(line_direction, halfspace_normal), 
           abs(dot(line_direction, halfspace_normal)) < SMALL
        );
    }
    */

  };

}

