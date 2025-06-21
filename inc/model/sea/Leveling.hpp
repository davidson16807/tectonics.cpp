#pragma once

// std libraries
#include <array>   // array

// 3rd party libraries
#include <index/adapted/symbolic/SymbolicArithmetic.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/iterated/Order.hpp>
#include <index/aggregated/Order.hpp>
#include <index/aggregated/Reduction.hpp>
#include <index/binned/Statistics.hpp>
#include <index/preceded/Statistics.hpp>
#include <index/procedural/Uniform.hpp>
#include <index/procedural/Range.hpp>

namespace sea
{

    /*
    `sea::Leveling` represents the following commutative diagram: 

        displacement×area → depth ⇆ volume

    where the above represents a 
    lithosphere with `displacement` and `area`,
    and a hydrosphere with a `depth` and `volume`
    */
    template<typename length, typename scalar>
    class Leveling
    {

        using lengths = std::vector<length>;
        using scalars = std::vector<scalar>;

        const iterated::Arithmetic<adapted::SymbolicArithmetic> arithmetic;
        const aggregated::Order<adapted::SymbolicOrder> order;
        const iterated::Order<adapted::SymbolicOrder> orders;
        const binned::Statistics<aggregated::Order<adapted::SymbolicOrder>, adapted::SymbolicArithmetic> binning;
        const preceded::Statistics<adapted::SymbolicArithmetic> preceding;
        const aggregated::Reduction<adapted::SymbolicArithmetic> total;

        const length unit_length;
        const scalar epsilon;

    public:

        Leveling(
            const length unit_length,
            const scalar epsilon
        ):
            arithmetic(),
            order(),
            orders(),
            binning(),
            preceding(),
            unit_length(unit_length),
            epsilon(epsilon)
        {}

        /*
        `volume` finds the total volume of a hydrosphere
        */
        template <typename areas>
        auto volume(
            const lengths& displacement,
            const areas& vertex_areas, 
            const length max_sea_depth
        ) const {
            return volume(displacement, vertex_areas, max_sea_depth, order.min(displacement));
        }

        /*
        `volume` finds the total volume of a hydrosphere
        */
        template <typename areas>
        auto volume(
            const lengths& displacement,
            const areas& vertex_areas, 
            const length max_sea_depth,
            const length min_displacement
        ) const {
            /*
            We use a regular for loop instead of functionality under `index/` 
            since the alternative requires storing to multiple temporary rasters.
            */
            auto zero = scalar(0)*unit_length*unit_length*unit_length;
            auto out = zero;
            auto column = zero;
            for (unsigned int i = 0; i < displacement.size(); ++i)
            {
                column = (max_sea_depth - (displacement[i]-min_displacement)) * vertex_areas[i];
                out += column > zero? column : zero;
            }
            return out;
        }


        /*
        "depth" finds the maximum depth of a hydrosphere using iterative numerical approximation.
        It accepts a raster that stores the height of a terrain relative to its lowest point 
        and a float that indicates the volume of water to be filled along the terrain.
        The model assumes that water has enough time to fill every space available 
        and achieve a globally uniform water depth.
        */
        template <typename length2, typename length3>
        length depth(
            const lengths& displacement, 
            const length2& vertex_areas, 
            const length3 sea_volume, 
            const unsigned int iterations = 16
        ){

            using area = typename length2::value_type;
            length min_displacement = order.min(displacement);
            area global_surface_area(total.sum(vertex_areas));
            // the depth of the ocean if the entire globe is at a single height
            length uniform_ocean_depth(sea_volume / global_surface_area);
            // lowest possible value - the value sealevel takes if all but an infinitesimal point on the globe is at the lowest height observed
            length sealevel_min(min_displacement + uniform_ocean_depth);
            // highest possible value - the value sealevel takes if all but an infinitesimal point on the globe is at the highest height observed
            length sealevel_max(order.max(displacement) + uniform_ocean_depth);
            // our current guess for sealevel, which we improve iteratively
            length sealevel_guess(0.0f);
            // the volume of the sea, presuming our guess were correct
            length3 sea_volume_of_guess(0.0f);

            scalar half(0.5);

            for (unsigned int i = 0; i < iterations; i++) 
            {
                sealevel_guess = (sealevel_max + sealevel_min) * half;
                sea_volume_of_guess = volume(displacement, vertex_areas, sealevel_guess, min_displacement);
                sealevel_min = sea_volume_of_guess < sea_volume? sealevel_guess : sealevel_min;
                sealevel_max = sea_volume_of_guess > sea_volume? sealevel_guess : sealevel_max;
            }

            return sealevel_guess;

        }

    };

}

