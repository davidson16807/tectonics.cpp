#pragma once

#include <unit/si.hpp>

#include <model/compound/point.hpp>

namespace compound {
namespace phase {

    struct LinearPhaseBoundary
    {
        int source_mineral_id;
        int target_mineral_id;
        /*
        NOTE: order of points doesn't matter, transitioned_fraction returns the same thing 
        as long as gradient_source_point is closer to the source phase than target.
        */
        glm::vec2 point_in_pascal_kelvin_point0;  
        glm::vec2 point_in_pascal_kelvin_point1;
        // constant constructor
        constexpr LinearPhaseBoundary(const si::temperature<float> temperature):
            pascal_to_kelvin(float(temperature/si::temperature<float>))
        {

        }
        /*
        `transitioned_fraction` returns the fraction of mass that transitions from source to to target
        in a therobaric gradient through a mass of incompressible inmiscible matter.
        */
        transitioned_fraction(
            const point<float> gradient_source_point, 
            const point<float> gradient_target_point
        ) const {
            glm::vec2 gradient_source(gradient_source_point.pressure/si::pascal, gradient_source_point.temperature/si::kelvin);
            glm::vec2 gradient_target(gradient_target_point.pressure/si::pascal, gradient_target_point.temperature/si::kelvin);
            glm::vec2 boundary0(point_in_pascal_kelvin_point0.pressure, point_in_pascal_kelvin_point0.temperaturel);
            glm::vec2 boundary1(point_in_pascal_kelvin_point1.pressure, point_in_pascal_kelvin_point1.temperaturel);
            return geometric::get_distance_along_2d_line_segment_to_line_segment(
                        gradient_source, gradient_target,
                        boundary0, boundary1) / 
                    glm::length(gradient_target-gradient_source);
        }
    };

}}

