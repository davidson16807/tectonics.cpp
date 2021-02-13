#pragma once

// C libraries
#include "math.h" // fmax

// in-house libraries
#include <math/lerp.hpp> // lerp()
#include <units/si.hpp> // pressure, temperature, unitless

namespace compound{
    namespace property
    {
        /*
        NOTE: 
        The namespace here each serves as a "category of primitives" where objects are variables of primitive types.
        We may consider moving to the academic layer if we discover its implementation can be done in a general way.

        This namespace maps to and from different reduced pressures and reduced temperatures to the "compressibility factor" ("Z").
        The compressibility factor is a function of reduced temperature ("Tr") and reduced pressure ("Pr"),  
        which are the temperature and pressure of a gas or liquid compound relative to that compound's critical temperature ("Tc") and pressure ("Tc").
        The critical temperature and pressure of a compound are the points beyond which the compound forms a supercritical fluid, having properties of both liquids and gases.

        Our model has the aspirational goal to predict behavior in exotic conditions, so our estimate for Z must be valid for a wide range of p and T,
        and although we currently do not plan to run it repeatedly for every grid cell in a raster, we do not want to preclude the possibility, 
        so performance is a secondary objective. Ideally, we would also like the approximation to be easy to implement, so we want to avoid interpolation tables.

        Tackacs (1989) gives an excellent review of past approximations for Z but virtually all require a sacrafice to one of our goals.
        Fast approximations either use interpolation tables or simple functions that only perform well over small regions.
        In any case, we also tend to be megalomaniacal and would just like to see if we can do it ourselves. Therefore, we created our own approximation.
        The framework for the approximation is described under "estimate_compressibility_factor", 
        and the fitted parameter values are applied to the function within "estimate_compressibility_factor"

        Documentation for the creation of our approximation and its parameterization can be found in the tectonics-research repo, under `chemistry/compressibility/visualize.R`
        */

        namespace 
        {
            /*
            This private namespace describes functions that should only be needed within the compressibility subnamespace.
            We can decide to elevate this to a nonanonymous namespace if there is need for it. 
            */

            /*
            "estimate_compressibility_factor" approximates the "compressibility factor" of a compound given a set of parameters describing the shape of the compounds compressibility chart.

            The function is designed based around four observations:
            1.) For high values of Pr, there is an approximately linear relationship between 1/Tr, Pr/Tr, and Z.
            2.) The estimate for high values of Pr above can be refined by adding a contribution from a sigmoidal function of 1/Tr. This is especially noticeable where Zc!=278.
            3.) The ideal gas law provides an accurate estimate at low values of P, so it is always the case that Z=1 for Pr=0.
            4.) When plotted as a function of 1/Tr and Pr/Tr, there is an approximately sigmoidal relationship between 1/Tr and the minimum value assumed by Z for a given Pr/Tr.

            From observations 1.) and 3.), we know that our approximation for Z can be expressed as an interpolation 
            between an estimate of Z for high pressures ("Zhi") and a constant Z=1 for low pressures: 1*I + (1-I)*Zhi for some interpolant "I"
            We then need only find an approximation for this interpolant, "I". 
            The minimum value for Z that's described in observation 4.) occurs near where we must interpolate between Zhi and 1,
            so it is reasonable to assume the interpolant "I" follows some similar sigmoidal relationship.
            We found for best results to define "I" as a clamped linear function of Pr/Tr. 
            The upper bound is reached where Pr/Tr=0, and the lower bound is reached at the Pr/Tr for which Zhi crosses some threshold defined by a sigmoid, which we term "Zmid".

            The function offers the following guarantees:
            * It produces reasonable behavior outside the observed data range, 
              e.g. estimates for values outside the intended range do not explode as they would using a Taylor series approximation
            * The approximation is implemented using only floating point arithmetic, 2 exponentials, and 1 exponent.
              This should place runtime in the same class as the fastest approximations reviewed by Takacs (1989), namely Papay, Burnett
            * Only 10 parameters, with straight forward interpretations, making it easy to implement and manipulate without requiring reoptimization.
            */
            constexpr float estimate_compressibility_factor(
                const float reduced_pressure,
                const float reduced_temperature,

                const float Zhi0,    // intercept of Tinv to the high pressure estimate of Z
                const float ZhiLTinv,// slope of the linear contribution of Tinv to the high pressure estimate of Z

                const float ZhiS0,   // value of Tinv at which the sigmoid contribution of Tinv to high pressure estimate is halfway to maximum
                const float ZhiSmax, // maximum sigmoid contribution of Tinv to the high pressure estimate of Z 
                const float ZhiSH,   // scale height of the sigmoid contribution of Tinv to the high pressure estimate of Z 

                const float ZhiV,    // contribution of V to the high pressure estimate of Z slope
                const float ZhiVk,   // contribution of V to the high pressure estimate of Z exponent

                const float Zmid0,   // value of Tinv indicating the halfway point of the the sigmoid that describes the value of Z at the start of interpolation between the estimates for high and low pressure
                const float Zmidmax, // maximum value that Z can assume at the start of interpolation between between the estimates for high and low pressure
                const float ZmidH    // scale height of the sigmoid that describes the value of Z at the start of interpolation between the estimates for high and low pressure
            ) {
                // SHORTHANDS
                float Tinv = 1/reduced_temperature;                
                float V    = reduced_pressure/reduced_temperature; // this is representative but not equal to volume, since volume is proportional to p/T

                float ZhiTinv = Zhi0 + ZhiLTinv*Tinv + ZhiSmax/(1+exp(ZhiSH*(Tinv-ZhiS0))); // "ZhiTinv" is the contribution of Tinv to our estimate for Zhi
                float Zhi  = pow(ZhiV*V,ZhiVk) + ZhiTinv;
                float Zmid = Zmidmax/(1+exp(ZmidH*(Tinv-Zmid0))); // "Zmid" is our handmade approximation to the sigmoidal relationship
                float Vmid = ((Zmid - ZhiTinv) / ZhiV);
                // float Vmid = pow(((Zmid - ZhiTinv) / ZhiV), 1/Zhi_Vk);
                // NOTE: we comment out the exponent above for three reasons: 
                // 1.) it should not matter at the low-Z regions where this formula starts to affect output (in order words it never does anything)
                // 2.) having it around reduces performance
                // 3.) having it around threatens us with NaNs if (S-LT1)/b happens to be negative
                float I = fmin(1, fmax(0,  1-1*V/fmax(0, Vmid) ));
                return I+(1-I)*Zhi;
            }
        }



        /*
        "estimate_compressibility_factor" approximates the "compressibility factor" of a compound given a the compound's critical compressibility, "Zc".
        If critical compressibility is not known or provided, the critical compressibility of air is assumed (Zc=0.278)

        "estimate_compressibility_factor" was constructed by running an optimization algorithm to fit a custom function to observed values from two data sources.
        The first data source was created by digitizing well known charts from Nelson and Obert (1955). 
        The script we used to digitize them can be found under 'digitize.R'.
        The second data source was created using OCR on tables from a more extensive, more recent, but less well known paper by Varsanyi (1986).
        The generation of the Varsanyi dataset from the pdf is documented within 'varsanyi1986.R'.
        Varsanyi's dataset appears ideal in that it provides the actual tables that underly his charts, 
        there are over 5000 data points, and the values are reported to 3 decimal places,
        but it is also valuable in that he provides estimates for different compounds that vary by their critical compressibility, 'Zc'.
        We calculate estimates for the three values of Zc he gives and then interpolate between them. 
        
        The fitted function offers the following guarantees:
        * It is applicable for use from 0.6<Tr<15 and 0<pr<40.  This exceeds the range of virtually all approximations reviewed by Takacs (1989).
        * It produces reasonable behavior outside the observed data range, 
          e.g. estimates for values outside the intended range do not explode as they would using a Taylor series approximation
        * The approximation has a mean absolute percentage error of 0.35% over the intended range for Zc=0.278 
          This surpasses all but 3 of the approximations reviewed by Takacs (1989).
          The remaining 3 approximations that surpassed this one either made use of large memory footprints (Gray-Sims) 
          or were only intended for use over narrower ranges (Carlile-Gillett, Dranchuk-A. Kassem).
          Note that this metric is not applicable where Zc!=0.278. 
          Since the observed values for other Zc are so much lower, percentage error is less meaningful.
        * mean absolute error is not to exceed 0.015 for Zc=0.278, 0.030 for Zc=0.244, or 0.030 for 
        * The max error of the function is not to exceed 0.26 over the intended range
          The highest error is observed to occur at regions where 2.0<T<2.5 and p>35.
        * The approximation is implemented using only floating point arithmetic, 3*2 exponentials, and 3 exponents.
          This should place runtime in the same class as the fastest approximations reviewed by Takacs (1989), namely Papay, Burnett
        */
        constexpr float estimate_compressibility_factor(
            const si::pressure pressure,
            const si::temperature temperature,
            const si::pressure critical_pressure,
            const si::temperature critical_temperature,
            const float critical_compressibility = 0.278f
        ){
            float reduced_pressure = (pressure/critical_pressure);
            float reduced_temperature = (temperature/critical_temperature);

            const auto Zc = std::array<float,3>{ 0.244f, 0.278f, 0.316f };
            auto Z = std::array<float,3>{
                estimate_compressibility_factor(
                    reduced_pressure, reduced_temperature,
                    0.139f, -0.103f,       // intercept and slope for the linear contribution of inverse reduced temperature to the high pressure estimate of compressibility
                    5.0f, 0.95f, 0.6f,     // scale height, max, and midpoint for the sigmoid contribution of inverse reduced temperature to the high pressure estimate of compressibility
                    0.103f, 0.92f,         // slope and exponent for the contribution of the pressure/reduced temperature ratio to the high pressure estimate of compressibility
                    8.0f, 1.0f, 0.8f       // scale height, max, and midpoint for the sigmoid that describes the value of Z at which we start interpolation between the estimates for high and low pressure
                ),
                estimate_compressibility_factor(
                    reduced_pressure, reduced_temperature,
                    0.153f, -0.143f,       // intercept and slope for the linear contribution of inverse reduced temperature to the high pressure estimate of compressibility
                    8.988f, 0.672f, 0.773f,// scale height, max, and midpoint for the sigmoid contribution of inverse reduced temperature to the high pressure estimate of compressibility
                    0.145f, 0.9f,          // slope and exponent for the contribution of the pressure/reduced temperature ratio to the high pressure estimate of compressibility
                    8.38f, 0.833f, 1.11f   // scale height, max, and midpoint for the sigmoid that describes the value of Z at which we start interpolation between the estimates for high and low pressure
                ),
                estimate_compressibility_factor(
                    reduced_pressure, reduced_temperature,
                    1.229f, -0.967f,       // intercept and slope for the linear contribution of inverse reduced temperature to the high pressure estimate of compressibility
                    -4.89f, -0.209f, 1.087f,// scale height, max, and midpoint for the sigmoid contribution of inverse reduced temperature to the high pressure estimate of compressibility
                    0.07f, 1.129f,         // slope and exponent for the contribution of the pressure/reduced temperature ratio to the high pressure estimate of compressibility
                    5.387f, 1.248f, 0.772f // scale height, max, and midpoint for the sigmoid that describes the value of Z at which we start interpolation between the estimates for high and low pressure
                )
            };

            return math::lerp(Zc, Z, critical_compressibility);
        }
    }
}