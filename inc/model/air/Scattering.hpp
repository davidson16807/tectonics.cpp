#pragma once

#include <cmath>      // sqrt, abs, etc.

#include <algorithm>  // max
#include <limits>     // numeric_limits

#include <field/poles/MonopoleScalar.hpp>
#include <field/poles/NaiveMultipole.hpp>

#include <model/light/Exposure.hpp>

namespace air
{

    template <typename scalar, glm::qualifier precision = glm::defaultp>
    class Scattering
    {

        using vec3 = glm::vec<3,scalar,precision>;

        static constexpr scalar s0 = scalar(0);
        static constexpr scalar s1 = scalar(1);
        static constexpr scalar s2 = scalar(2);
        static constexpr scalar s3 = scalar(3);
        static constexpr scalar s4 = scalar(4);
        static constexpr scalar half = scalar(0.5);

        scalar tiny;
        scalar pi;
        scalar sqrt_half_pi;
        scalar max_radii;

        /*
        GUIDE TO VARIABLE NAMES:
         "x*" distance along the ray from closest approach
         "y*" distance from the center of the world at closest approach
         "r*" distance ("radius") from the center of the world
         "*0" variable at reference point
         "*2" the square of a variable
         "Ch" a nudge we give to prevent division by zero, analogous to the "Chapman" function
        */

        /*
        `F` is a fast approximation to the integral, F = ∫ exp(r₀-√(x²+y²)) dx.
        It works by attempting to solve the integral using integration by substitution, 
          then tweaking that integral to prevent division by 0. 
        It is used internally by `Scattering` to implement `fast_air_column_density_ratio_through_atmosphere`.
        In the context of `Scattering`:
          `x` is the distance along a light ray from its closest approach to the world origin
          `y` is the distance to the world origin at closest approach
          `sqrty` is the square root of y, which is included as a parameter for performance reasons
          `r0` is the radius of the world
        where all distances are reported in scale heights.
        */
        scalar F(
            const scalar x,
            const scalar y,
            const scalar sqrty,
            const scalar r0
        ) const noexcept {
            const scalar k(0.6); // "k" is an empirically derived constant
            const scalar r = std::sqrt(x*x+y*y);
            const scalar Ch = (s1 - half/(y-x)) * sqrt_half_pi * sqrty + k*x;
            return std::exp(r0-r) / (x/r + s1/Ch);
        }

    public:

        Scattering(const scalar max_radii, const scalar pi, const scalar tiny):
            max_radii(max_radii),
            tiny(std::max(tiny, std::numeric_limits<scalar>::epsilon())),
            pi(pi),
            sqrt_half_pi(std::sqrt(half*pi))
        {}

        // Rayleigh phase function factor [-1, 1]
        scalar fraction_of_rayleigh_scattered_light_scattered_by_angle(
            const scalar cos_scatter_angle
        ) const noexcept {
            return  s3 * (s1 + cos_scatter_angle*cos_scatter_angle)
            / //------------------------
                        (scalar(16) * pi);
        }

        /*
        Henyey-Greenstein phase function factor [-1, 1]
        represents the average cosine of the scattered directions
        0 is isotropic scattering
        > 1 is forward scattering, < 1 is backwards
        */
        scalar fraction_of_mie_scattered_light_scattered_by_angle(
            const scalar cos_scatter_angle
        ) const noexcept {
            const scalar g(0.76);
            return              (s1 - g*g)
            / //---------------------------------------------
              (s4 * pi * std::pow(s1 + g*g - s2*g*cos_scatter_angle, scalar(1.5)));
        }

        /*
        Schlick's fast approximation to the Henyey-Greenstein phase function factor
        Pharr and  Humphreys [2004] equivalence to g above
        */
        scalar fast_fraction_of_mie_scattered_light_scattered_by_angle(
            const scalar cos_scatter_angle
        ) const noexcept {
            const scalar g(0.76);
            const scalar k = scalar(1.55)*g - scalar(0.55) * (g*g*g);
            return          (s1 - k*k)
            / //-------------------------------------------
              (s4 * pi * (s1 + k*cos_scatter_angle) * (s1 + k*cos_scatter_angle));
        }

        /*
        "fast_air_column_density_ratio_through_atmosphere" 
          calculates the distance you would need to travel 
          along the surface to encounter the same number of particles in the column. 
        All distances are recorded in scale heights.
        "a" and "b" are distances along the ray from closest approach.
          The ray is fired in the positive direction.
          If there is no intersection with the planet, 
          a and b are distances from the closest approach to the upper bound.
        "y2" is the square of the closest distance from the ray to the center of the world.
        "r0" is the radius of the world.
        */
        scalar fast_air_column_density_ratio_through_atmosphere(
            const scalar a,
            const scalar b,
            const scalar y2,
            const scalar r0
        ) const noexcept {
            const scalar x0 = std::sqrt(std::max(r0*r0 - y2, tiny));
            const scalar y = std::sqrt(y2);
            const scalar sqrty = std::sqrt(y);
            const scalar F0 = F(x0,y,sqrty,r0);
            return std::max( glm::sign(b)*(F0-F(b,y,sqrty,r0)) - glm::sign(a)*(F0-F(a,y,sqrty,r0)), s0 );
        }

        template<typename spectrum>
        spectrum fraction_of_light_transmitted_through_atmosphere(
            const vec3 view_origin, const vec3 view_direction, const scalar view_start_length, const scalar view_stop_length,
            const vec3 world_position, const scalar world_radius, const scalar atmosphere_scale_height,
            const spectrum beta_sum // sum of betas for attenuation effects (chiefly rayleigh, mie, and absorption)
        ) const noexcept {
            scalar h = atmosphere_scale_height;
            scalar r = world_radius / h;
            vec3 V0 = (view_origin + view_direction * view_start_length - world_position) / h;
            vec3 V1 = (view_origin + view_direction * view_stop_length - world_position) / h;
            vec3 V = view_direction; // unit vector pointing to pixel being viewed
            scalar v0 = glm::dot(V0,V);
            scalar v1 = glm::dot(V1,V);
            scalar zv2 = glm::dot(V0,V0) - v0*v0;
            scalar sigma = fast_air_column_density_ratio_through_atmosphere(v0,v1,zv2,r);
            return glm::exp(-sigma * beta_sum * h);
        }

        template<typename spectrum>
        vec3 rgb_fraction_of_distant_light_scattered_by_atmosphere(
            const vec3 view_origin, const vec3 view_direction, const scalar view_start_length, const scalar view_stop_length,
            const vec3 world_position, const scalar world_radius,
            const vec3 light_direction, const scalar atmosphere_scale_height,
            const spectrum beta_ray, const spectrum beta_mie, const spectrum beta_abs, 
            const scalar step_count
        ) const noexcept {
            /*
            For an excellent introduction to what we're try to do here, see Alan Zucconi: 
              https://www.alanzucconi.com/2017/10/10/atmospheric-scattering-3/
            We will be using most of the same terminology and variable names.
            GUIDE TO VARIABLE NAMES:
             Uppercase letters indicate vectors.
             Lowercase letters indicate scalars.
             Going for terseness because I tried longhand names and trust me, you can't read them.
             "*v*"    property of the view ray, the ray cast from the viewer to the object being viewed
             "*l*"    property of the light ray, the ray cast from the object to the light source
             "y*"     distance from the center of the world to the plane shared by view and light ray
             "z*"     distance from the center of the world to along the plane shared by the view and light ray 
             "r*"     a distance ("radius") from the center of the world
             "h*"     the atmospheric scale height, the distance at which air density reduces by a factor of e
             "*2"     the square of a variable
             "*0"     property at the start of the raymarch
             "*1"     property at the end of the raymarch
             "*i"     property during an iteration of the raymarch
             "d*"     the change in a property across iterations of the raymarch
             "beta*"  a scattering coefficient, the number of e-foldings in light intensity per unit distance
             "gamma*" a phase factor, the fraction of light that's scattered in a certain direction
             "sigma*" a column density ratio, the density of a column of air relative to surface density
             "F*"     fraction of source light that reaches the viewer due to scattering for each color channel
             "*_ray"  property of rayleigh scattering
             "*_mie"  property of mie scattering
             "*_abs"  property of absorption
            setup variable shorthands
            express all distances in scale heights 
            express all positions relative to world origin
            */
            scalar h = atmosphere_scale_height;
            scalar r = world_radius / h;
            vec3 V0 = (view_origin + view_direction * view_start_length - world_position) / h;
            vec3 V1 = (view_origin + view_direction * view_stop_length - world_position) / h;
            vec3 V = view_direction; // unit vector pointing to pixel being viewed
            scalar v0 = glm::dot(V0,V);
            scalar v1 = glm::dot(V1,V);
            vec3 L = light_direction; // unit vector pointing to light source
            scalar VL = glm::dot(V,L);
            // "gamma_*" indicates the fraction of scattered sunlight that scatters to a given angle (indicated by its cosine, A.K.A. "VL").
            // It only accounts for a portion of the sunlight that's lost during the scatter, which is irrespective of wavelength or density
            scalar gamma_ray = fraction_of_rayleigh_scattered_light_scattered_by_angle(VL);
            scalar gamma_mie = fast_fraction_of_mie_scattered_light_scattered_by_angle(VL);
            // "beta_*" indicates the rest of the fractional loss.
            // it is dependant on wavelength, and the density ratio, which is dependant on height
            // So all together, the fraction of sunlight that scatters to a given angle is: beta(wavelength) * gamma(angle) * density_ratio(height)
            spectrum beta_sum = h*(beta_ray + beta_mie + beta_abs);
            spectrum beta_gamma = h*(beta_ray * gamma_ray + beta_mie * gamma_mie);
            // number of iterations within the raymarch
            scalar dv = (v1 - v0) / step_count;
            scalar dl = dv*VL;
            scalar l0 = glm::dot(V0,L);
            scalar y  = glm::dot(V0,glm::normalize(glm::cross(V,L)));
            scalar y2 = y*y;
            scalar zv2 = glm::dot(V0,V0) - y2 - v0*v0;
            scalar zl2(0);
            scalar vi = v0;
            scalar li = l0;
            scalar sigma; // columnar density encountered along the entire path, relative to surface density, effectively the distance along the surface needed to obtain a similar column density
            vec3 F(0); // total intensity for each color channel, found as the sum of light intensities for each path from the light source to the camera
            for (scalar i = s0; i < step_count; ++i)
            {
                vi = dv*i + v0;
                li = VL*(vi-v0) + l0;
                zl2 = vi*vi + zv2 - li*li;
                sigma = fast_air_column_density_ratio_through_atmosphere(v0, vi, y2+zv2, r )
                      + fast_air_column_density_ratio_through_atmosphere(li, max_radii*r, y2+zl2, r );
                F += glm::exp(r-std::sqrt(vi*vi+y2+zv2) - beta_sum*sigma) * beta_gamma * dv;
                /*
                NOTE: the above is equivalent to the incoming fraction multiplied by the outgoing fraction:
                incoming fraction: the fraction of light that scatters towards camera
                  exp(r-sqrt(vi*vi+y2+zv2)) * beta_gamma * dv
                outgoing fraction: the fraction of light that scatters away from camera
                * exp(-beta_sum * sigma);
                */
            }
            return F;
        }

    };

}

/*
tests:
* F approximates the integral ∫ exp(r₀-√(x²+y²)) dx
* phase functions are finite and non-negative
* read Schlick (1990?) for properties used to derive phase factor approximations
* phase functions are smallest at a right angle and largest when parallel
*/

