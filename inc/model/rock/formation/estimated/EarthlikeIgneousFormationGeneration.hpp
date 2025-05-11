#pragma once

#include <math/analytic/Sum.hpp>
#include <math/analytic/Gaussian.hpp>
#include <math/inspected/InverseByNewtonsMethod.hpp>

#include <relation/PolynomialRailyardRelation.hpp>
#include <relation/ScalarRelation.hpp>

#include <field/Compose.hpp>                        // Compose
#include <field/noise/RankedFractalBrownianNoise.hpp>// field::ranked_fractal_brownian_noise

#include <model/rock/stratum/StratumForElevation.hpp>  // StratumForElevation
#include <model/rock/formation/FormationGeneration.hpp>  // FormationGeneration

namespace rock
{

    template<typename Grid, typename length, typename scalar, typename id>
    class EarthlikeIgneousFormationGeneration
    {

        const Grid& grid;
        const length max_wavelength;
        const scalar hurst_exponent;
        const id octave_count;

    public:
        EarthlikeIgneousFormationGeneration(
            const Grid& grid, 
            const length max_wavelength, 
            const scalar hurst_exponent, 
            const id octave_count
        ):
          grid(grid),
          max_wavelength(max_wavelength),
          hurst_exponent(hurst_exponent),
          octave_count(octave_count)
        {}

        auto operator() (const scalar seed1, const scalar seed2) const
        {

            length meter(1);

            scalar min_elevation(-16000.0f);
            scalar max_elevation( 16000.0f);

            analytic::Sum<float,analytic::Gaussian<float>> hypsometry_pdf_unscaled {
                analytic::Gaussian(-4019.0f, 1113.0f, 0.232f),
                analytic::Gaussian(  797.0f, 1169.0f, 0.209f)
            };
            auto hypsometry_cdf_unscaled = analytic::integral(hypsometry_pdf_unscaled);
            // auto hypsometry_pdf_ddx = analytic::derivative(hypsometry_pdf_unscaled);
            auto hypsometry_cdf_unscaled_range = hypsometry_cdf_unscaled(max_elevation) - hypsometry_cdf_unscaled(min_elevation);
            auto hypsometry_cdf = hypsometry_cdf_unscaled / hypsometry_cdf_unscaled_range;
            auto hypsometry_pdf = hypsometry_pdf_unscaled / hypsometry_cdf_unscaled_range;
            auto hypsometry_cdfi = inspected::inverse_by_newtons_method(hypsometry_cdf, hypsometry_pdf, 0.5f, 30);
            auto hypsometry_cdfi_meters = relation::ScalarRelation(1.0f, meter, hypsometry_cdfi);

            rock::StratumForElevation stratum_per_area_for_elevation {
              // displacements are from Charette & Smith 2010 (shallow ocean), enclopedia britannica (shelf bottom"continental slope"), 
              // wikipedia (shelf top), and Sverdrup & Fleming 1942 (land)
              // Funck et al. (2003) suggests a sudden transition from felsic to mafic occuring at ~4km depth or 8km thickness
              relation::get_linear_interpolation_function(si::meter, si::megayear, 
                {-11000.0, -5000.0, -4500.0, -2000.0, -900.0},
                {250.0,    100.0,   0.0,       100.0, 1000.0}),
              relation::get_linear_interpolation_function(si::meter, si::kilogram, // per unit of area as defined by `grid`
                {-5000.0,              -4500.0},
                {3300.0 * 7100.0, 2890.0 * 0.0}),
              relation::get_linear_interpolation_function(si::meter, 2600.0 * si::kilogram, // per unit of area as defined by `grid`
                {-5000.0, -4500.0,  -950.0,  840.0,    8848.0},
                {0.0,      7100.0, 28300.0, 36900.0, 70000.0}),
              relation::get_linear_interpolation_function(si::meter, 1.0, 
                {-1500.0, 8848.0},
                {0.25,      0.25}), // from Gillis (2013)
              relation::get_linear_interpolation_function(si::meter, 1.0, 
                {-1500.0, 8848.0},
                {0.15,      0.15}) // based on estimate from Wikipedia
            };

            return rock::FormationGeneration(
                grid, 
                field::compose(
                    stratum_per_area_for_elevation,
                    field::compose(
                        hypsometry_cdfi_meters,
                        field::ranked_fractal_brownian_noise<3>(octave_count, hurst_exponent, meter/max_wavelength, seed1, seed2)
                    )
                )
            );

        }

    };

}

