
namespace phase
{
    enum CarbonPhases
    {
        supercritical,
        vapor,
        liquid,
        graphite,
        diamond,
        COUNT
    }
    /*
    `get_carbon_phase` returns the phase of carbon at a given temperature and pressure
     Carbon has two solid phases, one of them (diamond) is economically relevant so we give special attention to it.
    */

    CarbonPhases get_carbon_phase(
        float p,  // current pressure
        float t   // current temperature
    ){
        const     float Ru(8.3144598);           // universal gas constant
        const     float M (0.012);               // molar mass, kg/mol
        const     float Lv(29650000);            // specific latent heat of vaporization (J/kg)
        const     float p0(0.2e6);               // triple point pressure (Pa)
        const     float t0(4600);                // triple point temperature (K)
        constexpr float R (Ru/M);                // individual gas constant
        constexpr float mv(-R/Lv);               // slope of clapeyron equation for vaporus
        constexpr float bv(1/t0 - mv * log(p0)); // intercept for clapeyron equation for vaporus
        const     float ml(1e-3);                // slope of clapeyron equation repurposed for liquidus, estimated from phase diagram
        const     float bl(-3.411e-5);           // intercept for clapeyron equation repurposed for liquidus, estimted from phase diagram
        if ( t > 1/(mv*log(p)+bv) )
        {
            if (t > 6810 && p > 2.2e8)
            {
                CarbonPhases::supercritical; // based on Leider (1973)
            }
            return CarbonPhases::vapor;
        }
        else if ( t < t0 && p < 1e10 )
        {
            return CarbonPhases::graphite;
        }
        else if ( t > 1/(ml*log(p)+bl) )
        {
            return CarbonPhases::liquid;
        }
        else
        {
            return CarbonPhases::diamond;
        }
    }
}
    