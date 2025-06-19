#pragma once

// std libraries
#include <array>   // array

// 3rd party libraries
#include <index/adapted/symbolic/SymbolicArithmetic.hpp>
#include <index/adapted/symbolic/SymbolicOrder.hpp>
#include <index/iterated/Arithmetic.hpp>
#include <index/aggregated/Order.hpp>
#include <index/iterated/Order.hpp>
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
            const areas& area, 
            const length max_sea_depth
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
                column = (max_sea_depth - displacement[i]) * area[i];
                out += column > zero? column : zero;
            }
            return out;
        }

        /*
        `depth` finds the maximum depth of a hydrosphere using iterative numerical approximation.
        It accepts a raster that stores the height of a terrain relative to its lowest point 
        and a float that indicates the volume of water to be filled along the terrain.
        The model assumes that water will eventually be able to fill every space available 
        and achieve a globally uniform water depth.
        */
        template <typename areas, typename volume>
        length depth(
            const lengths& displacement, 
            const areas& area, 
            const volume sea_volume, 
            scalars& raster_scratch,
            scalars& bin_scratch
        ) const {
            scalars* areas_in_units = &raster_scratch;
            scalars* area_per_displacement = &bin_scratch;
            scalars* area_for_displacement = &bin_scratch;
            scalars* volume_per_displacement = &bin_scratch;
            scalars* volume_for_displacement = &bin_scratch;
            auto min_displacement = order.min(displacement);
            auto max_displacement = order.max(displacement);
            auto bin_count = bin_scratch.size();
            auto bin_increment = (max_displacement - min_displacement) / bin_count;
            arithmetic.divide(area, procedural::uniform(unit_length*unit_length), *areas_in_units);
            binning.sum(displacement, *areas_in_units, *area_per_displacement);
            preceding.sum(*area_per_displacement, *area_for_displacement);
            auto max_area = bin_scratch[bin_count-1] * unit_length * unit_length;
            arithmetic.multiply(*area_for_displacement, procedural::uniform(bin_increment/unit_length), *volume_per_displacement);
            orders.max(procedural::uniform(epsilon), *volume_per_displacement, *volume_per_displacement);
            // ^^^ ensure the function represented by `volume_for_displacement` is always increasing so it can invert, below
            preceding.sum(*volume_per_displacement, *volume_for_displacement);
            auto area_for_displacement_relation = 
                relation::get_linear_interpolation_function(si::meter3, si::meter,
                    *volume_for_displacement,
                    procedural::range(min_displacement/unit_length, max_displacement/unit_length, bin_scratch.size())
                );
            auto max_volume = bin_scratch[bin_count-1] * unit_length * unit_length * unit_length;
            auto max_sea_depth = area_for_displacement_relation(sea_volume);
            if (max_sea_depth > max_displacement) // water world, need to add depth covering tallest mountain
            {
                max_sea_depth += (sea_volume - max_volume) / max_area;
            }
            return max_sea_depth;
        }

    };

}

