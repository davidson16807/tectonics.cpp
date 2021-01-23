#pragma once

namespace phase
{
    enum struct BasicPhases
    {
        supercritical,
        vapor,
        liquid,
        solid,
        COUNT
    }
    /*
    `get_basic_phase` returns the phase of a basic phase diagram given a set of parameters 
    that should be applicable to virtually any solid/liquid/vapor phase diagram.
    Models boiling/sublimation points using the Clapeyron equation,
    and melting points using the empirical Simon-Glatzel equation.
    */

    BasicPhases get_basic_phase(
        float p,  // current pressure
        float t,  // current temperature
        float p0, // triple point pressure
        float t0, // triple point temperature
        float pc, // critical point pressure
        float tc, // critical point temperature
        float pf, // freezing point pressure
        float tf, // freezing point temperature
        float L,  // latent heat of vaporization at boiling point
        float M,  // molar mass
        float a,  // simon glatzel slope
        float c,  // simon glatzel exponent
        bool has_simon_glatzel_parameters,
        bool has_critical_point
    ){
        const float Ru(8.3144598);
        float R = Ru/M;
        float m = -(R/L);
        float b = has_critical_point? (1/tc - m * log(pc)) : (1/t0 - m * log(p0));
        float A = (tf-t0) / (pf-p0);
        if (t > tc && p > pc && has_critical_point)
        {
            return BasicPhases::supercritical;
        }
        else if ( t > 1/(m*log(p)+b) )
        {
            return BasicPhases::vapor;
        }
        else if ( t > ( t0 * max((p-p0)/a + 1, 0)^(1/c) ) && has_simon_glatzel_parameters )
        {
            return BasicPhases::liquid;
        }
        else if ( t > t0 + A*p ) // linear relation, for when simon glatzel exponent can't be determined
        {
            return BasicPhases::liquid;
        }
        else
        {
            return BasicPhases::solid;
        }
    }
}
    