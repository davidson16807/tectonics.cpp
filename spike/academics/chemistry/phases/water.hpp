#pragma once

namespace phase
{
    enum struct WaterPhases
    {
        //fascies            //   origin T    origin P    radius T
        gas,
        liquid,
        ice1h,
        ice1c,
        ice2,
        ice3,
        ice4,
        ice5,
        ice6,
        ice7,
        ice8,
        ice9,
        ice10,
        ice11_hexagonal,
        ice11_orthorhombic,
        ice12,
        ice13,
        ice14,
        ice15,
        COUNT
    }
    /*
     `get_water_phase` returns the phase of water at a given temperature and pressure.
     Water occurs often in the universe, its one of the simplest compounds that can be made 
     from two of the most abundant elements in the universe
     It has many kinds of solid phases. We're used to ice1h, but ice1c occurs in clouds on earth,
     and ice 7 is common on other planets and moons, so we give its phase diagram special attention.
     Since it takes a long time to go back and revisit phase diagrams if an alteration is needed,
     We will attempt to represent all phases that are currently known to science.
     We don't expect we'll be using these, we just want to make sure we don't have to go through this again.
     Nevertheless, some phases are not very common so they will be represented only in a crude way. 
    */
    WaterPhases get_water_phase(
        float p, // current pressure
        float t  // current temperature
    ){
        const     float Ru(8.3144598);          // universal gas constant
        const     float M (0.0180153);          // molar mass, kg/mol
        const     float L (22.6e5);             // specific latent heat of vaporization (J/kg)
        const     float p0(0.6116e3);           // triple point pressure (Pa)
        const     float t0(273.15);             // triple point temperature (K)
        constexpr float R (Ru/M);               // individual gas constant
        constexpr float m (-R/L);               // slope of clapeyron equation for vaporus
        constexpr float b (1/t0 - m * log(p0)); // intercept for clapeyron equation for vaporus
        const     float tc(647.096);            // critical point temperature (K) 
        const     float pc(22.064e6);           // critical point pressure (Pa)
        const     float a3(7070e5);             // slope of clapeyron equation repurposed for liquidus, estimated from phase diagram
        const     float c3(4.46);               // intercept for clapeyron equation repurposed for liquidus, estimted from phase diagram
        if ( t > 1.0f/(m*log(p)+b) && p<1e9f)
        {
            if (t > tc && p > pc)
            {
                return WaterPhases::supercritical;
            }
            else
            {
                return WaterPhases::gas;
            }
        }
        else if ( t > ( t0 * max((p-p0)/a3 + 1.0f, 0.0f)^(1.0f/c3) ) )
        {
            if (t > tc && p > pc)
            {
                return WaterPhases::supercritical;
            }
            else
            {
                return WaterPhases::liquid;
            }
        }
        else if (p < 209.9e6f && t < 73.15f)
        {
            return WaterPhases::ice11_orthorhombic; //11
        }
        else if (p < 209.9e6f && t < 173.15f)
        {
            return WaterPhases::ice1c; //1c
        }
        else if (p < 209.9e6f)
        {
            return WaterPhases::ice1h; //1h
        }
        else if (p < 632.4e6f && t < 170.0f)
        {
            return WaterPhases::ice9; //9
        }
        else if (p < 350.1e6f && t > 238.5f)
        {
            return WaterPhases::ice3; //3
        }
        else if (350.1e6f < p && p < 632.4e6f && t > 218.0f)
        {
            return WaterPhases::ice5; //5
        }
        else if (p < 632.4e6f)
        {
            return WaterPhases::ice2; //2
        }
        else if (p < 2.216e9f && t < 130.0f)
        {
            return WaterPhases::ice15; //15
        }
        else if (p < 2.216e9f)
        {
            return WaterPhases::ice6; //6
        }
        else if (p < 62e9f && (t-100.0f)/(278.0f-100.0f) < (p-62e9f)/(2.1e9f-62e9f))
        {
            return WaterPhases::ice8; //8
        }
        else if (p < 62e9f)
        {
            return WaterPhases::ice7; //7
        }
        else if (p < 350e9f)
        {
            return WaterPhases::ice10; //10
        }
        else 
        {
            return WaterPhases::ice11_hexagonal;
        }
    }
}
    