#pragma once

// in-house libraries
#include <units/si.hpp>
#include <model/compound/point.hpp>
#include <model/compound/phase/PhaseDiagram.hpp>
#include <model/compound/table/PartialTable.hpp>

namespace compound {
namespace published {

table::PartialTable<PhaseDiagram> phase{
    {compounds::water, 
        [](){
            const int supercritical = -3;
            const int gas = -2;
            const int liquid = -1;
            const int ice1h = 0;
            const int ice1c = 1;
            const int ice2 = 2;
            const int ice3 = 3;
            // const int ice4 = 4; // unused
            const int ice5 = 5;
            const int ice6 = 6;
            const int ice7 = 7;
            const int ice8 = 8;
            const int ice9 = 9;
            const int ice10 = 10;
            const int ice11h = 11;
            const int ice11o = 12;
            // const int ice12 = 13; // unused
            // const int ice13 = 14; // unused
            // const int ice14 = 15; // unused
            const int ice15 = 16;
            const auto critical = point<double>(647.096 * si::kelvin, 22.064e6 * si::pascal);
            const auto triple   = point<double>(273.16 * si::kelvin, 0.6116e3 * si::pascal);
            const auto M  = 18.0153 * si::gram/si::mole;
            const auto L  = 22.6e5 * si::joule/si::kilogram; // latent heat of vaporization
            const auto a = 620.0 * si::bar;                  // simon glatzel slope
            const auto c = 60.0;                             // simon glatzel inverse exponent
            return PhaseDiagram(
                supercriticality(critical),
                basic_vaporus(L, M, critical),
                simon_glatzel_liquidus(triple, a, c),
                {
                    LinearPhaseRegion(ice11o,
                        unbounded_low_temperature(), 73.15*si::kelvin, 
                        unbounded_low_pressure(),    209.9e6*si::pascal ),
                    LinearPhaseRegion(ice1c,
                        unbounded_low_temperature(), 173.15*si::kelvin, 
                        unbounded_low_pressure(),    209.9e6*si::pascal ),
                    LinearPhaseRegion(ice1h,
                        unbounded_low_temperature(), unbounded_high_temperature(), 
                        unbounded_low_pressure(),    209.9e6*si::pascal ),
                    LinearPhaseRegion(ice9,
                        unbounded_low_temperature(), 170.0*si::kelvin, 
                        unbounded_low_pressure(),    632.4e6*si::pascal ),
                    LinearPhaseRegion(ice3,
                        238.5*si::kelvin,            unbounded_high_temperature(), 
                        unbounded_low_pressure(),    350.1e6*si::pascal ),
                    LinearPhaseRegion(ice5,
                        218.0*si::kelvin,            unbounded_high_temperature(), 
                        350.1e6*si::pascal,          632.4e6*si::pascal ),
                    LinearPhaseRegion(ice2,
                        unbounded_low_temperature(), unbounded_high_temperature(), 
                        unbounded_low_pressure(),    632.4e6*si::pascal ),
                    LinearPhaseRegion(ice15,
                        unbounded_low_temperature(), 130.0*si::kelvin, 
                        unbounded_low_pressure(),    2.216e9*si::pascal ),
                    LinearPhaseRegion(ice6,
                        unbounded_low_temperature(), unbounded_high_temperature(), 
                        unbounded_low_pressure(),    2.216e9*si::pascal ),
                    LinearPhaseRegion(ice8,
                        unbounded_low_temperature(), 
                        temperature_lerp(
                            point<double>(62e9*si::pascal, 100.0*si::kelvin), 
                            point<double>(2.1e9*si::pascal,278.0*si::kelvin)), 
                        unbounded_low_pressure(),    62e9*si::pascal ),
                    LinearPhaseRegion(ice7,
                        unbounded_low_temperature(), unbounded_high_temperature(), 
                        unbounded_low_pressure(),    62e9*si::pascal ),
                    LinearPhaseRegion(ice10,
                        unbounded_low_temperature(), unbounded_high_temperature(), 
                        unbounded_low_pressure(),    350e9*si::pascal ),
                },
                ice11h
            );
        }()},

    {compounds::nitrogen, 
        [](){
            const int beta = 0;
            const int alpha = 1;
            const int gamma = 2;
            // triple and critical points
            const point<double> slg_triple(12.463 * si::kilopascal, 63.15 * si::kelvin);
            const point<double> abg_triple(4650 * si::atmosphere,   44.5 * si::kelvin);
            const point<double> critical  (3.39 * si::megapascal,   126.21 * si::kelvin);
            // latent heat of vaporization and molar mass
            const si::specific_energy<double> L  = 198.8 * si::joule/si::gram;
            const si::molar_mass<double>      M  = 28.013  * si::gram/si::mole;
            // Simon-Glatzel constants, from Manzhelii (1997)
            const si::pressure<double>        a  = 1606.56 * si::bar;
            const double                      c  = 1.791;
            return PhaseDiagram(
                supercriticality(critical),
                basic_vaporus(L, M, critical),
                simon_glatzel_liquidus(slg_triple, a, c),
                {
                    LinearPhaseRegion(beta,
                        unbounded_low_temperature(), 
                        unbounded_high_temperature(), 
                        unbounded_low_pressure(),    
                        pressure_lerp(slg_triple, abg_triple)),
                    LinearPhaseRegion(alpha,
                        unbounded_low_temperature(), unbounded_high_temperature(), 
                        abg_triple.pressure,         unbounded_high_pressure())
                },
                gamma
            );
        }()},


    {compounds::oxygen, 
        [](){
            const int beta = 0;
            const int alpha = 1;
            const int gamma = 2;
            // triple and critical points
            const point<double> critical  (5.043 * si::megapascal,   154.59 * si::kelvin);
            const point<double> slg_triple(0.14633 * si::kilopascal, 54.35 * si::kelvin);
            const point<double> ab0_sample(0.0 * si::kilobar,        24.0 * si::kelvin);
            const point<double> ab1_sample(9.0 * si::kilobar,        52.0 * si::kelvin);
            const point<double> bg0_sample(0.0 * si::kilobar,        44.0 * si::kelvin);
            const point<double> bg1_sample(2.0 * si::kilobar,        55.0 * si::kelvin);
            // latent heat of vaporization and molar mass
            const si::specific_energy<double> L  = 213.1 * si::joule/si::gram;
            const si::molar_mass<double>      M  = 31.9988 * si::gram/si::mole;
            // Simon-Glatzel constants
            const si::pressure<double>        a  = 2733e5 * si::pascal;
            const si::pressure<double>        b  = 0.0 * si::pascal;
            const double                      c  = 1.7425;
            const auto Ru = si::universal_gas_constant;
            auto R = Ru/M;
            auto m = -(R/L);
            auto b = 1/Tc - m * std::log(pc/si::pascal);
            return PhaseDiagram(
                supercriticality(critical),
                basic_vaporus(L, M, critical),
                simon_glatzel_liquidus(slg_triple, a, c),
                {
                    LinearPhaseRegion(alpha,
                        unbounded_low_temperature(), 
                        unbounded_high_temperature(),  
                        pressure_lerp(ab0_sample, ab1_sample), 
                        unbounded_high_pressure()),
                    LinearPhaseRegion(beta,
                        unbounded_low_temperature(), 
                        unbounded_high_temperature(),  
                        pressure_lerp(bg0_sample, bg1_sample), 
                        unbounded_high_pressure())
                },
                gamma
            );
        }()},

    {compounds::methane, 
        [](){
            const int beta = 0;
            const int alpha = 1;
            // triple and critical points
            const point<double> critical (4.60 * si::megapascal,   190.56 * si::kelvin);
            const point<double> triple   (11.696 * si::kilopascal, 90.694 * si::kelvin);
            // latent heat of vaporization and molar mass
            const si::specific_energy<double> L  = 510.83 * si::joule/si::gram;
            const si::molar_mass<double>      M  = 16.043 * si::gram/si::mole;
            // Simon-Glatzel constants
            const si::pressure<double>        a  = 2080e5 * si::pascal;
            const double                      c  = 1.698;
            return PhaseDiagram(
                supercriticality(critical),
                basic_vaporus(L, M, critical),
                simon_glatzel_liquidus(triple, a, c),
                {
                    LinearPhaseRegion(alpha,
                        20.0 * si::kelvin,           unbounded_high_temperature(), 
                        unbounded_low_pressure(),    unbounded_high_pressure())
                },
                beta
            );
        }()},

    
    {compounds::helium, 
        [](){
            // const int supercritical = -3;
            // const int gas = -2;
            // const int liquid = -1;
            const int hcp = 0;
            const int fcc = 1;
            // triple point between solid, liquid, and gas
            const point<double> triple     (5.048e3 * si::pascal, 2.1768 * si::kelvin);
            const point<double> critical   (0.227 * si::megapascal, 5.19 * si::kelvin);
            const point<double> ab0_sample (1.1 * si::kilobar, 15.0 * si::kelvin);
            const point<double> ab1_sample (3.25 * si::kilobar, 20.0 * si::kelvin);
            // latent heat of vaporization and molar mass
            const si::specific_energy<double> L  = 20.7 * si::joule / si::gram;
            const si::molar_mass<double>      M  = 4.0026 * si::gram/si::mole;
            // Simon-Glatzel constants
            const si::pressure<double>        a  = 50.96e5 * si::pascal;
            const double                      c  = 1.5602;
            return PhaseDiagram(
                supercriticality(critical),
                basic_vaporus(L, M, critical),
                simon_glatzel_liquidus(triple, a, c),
                {
                    LinearPhaseRegion(hcp,
                        unbounded_low_temperature(), 
                        unbounded_high_temperature(),  
                        pressure_lerp(ab0_sample, ab1_sample), 
                        unbounded_high_pressure()),
                },
                fcc
            );
        }()},
    


    {compounds::carbon, 
        [](){
            // const int supercritical = -3;
            // const int gas = -2;
            // const int liquid = -1;
            const int beta = 0;
            const int alpha = 1;
            const point<double> triple     (15.35 * si::kilopascal, 68.15 * si::kelvin); // PubChem
            const point<double> critical   (3.494 *  si::megapascal, 132.86 * si::kelvin);
            const point<double> ab0_sample (1.0 * si::atmosphere, 62.0 * si::kelvin);
            const point<double> ab1_sample (3.5 * si::gigapascal, 150.0 * si::kelvin);
            // latent heat of vaporization and molar mass
            const si::specific_energy<double> L  = 6.04 * si::kilojoule/(28.010*si::gram);
            const si::molar_mass<double>      M  = 28.010 * si::gram/si::mole;
            return PhaseDiagram(
                supercriticality(critical),
                basic_vaporus(L, M, critical),
                basic_liquidus(triple, freezing),
                {
                    LinearPhaseRegion(beta,
                        unbounded_low_temperature(), 
                        unbounded_high_temperature(),  
                        pressure_lerp(ab0_sample, ab1_sample), 
                        unbounded_high_pressure()),
                },
                alpha
            );
        }()},



    {compounds::quartz, 
        [](){
            // const int supercritical = -3;
            // const int gas = -2;
            // const int liquid = -1;
            const int beta = 0;
            const int alpha = 1;
            const int cristobalite_alpha = 2;
            const int cristobalite_beta = 3;
            const int coesite = 4;
            const int stishovite = 5; // unused
    
            // critical point
            const point<double> critical   (1.7e8 * si::pascal, 5300.0* si::kelvin);
            // triple point between solid, liquid, and gas
            const point<double> slg_triple (0.0003 * si::pascal, 1983.0 * si::kelvin);
            // triple point between alpha, beta, and coesite
            const point<double> abc_triple (3.1 * si::gigapascal, 1190.0 * si::celcius);
            // sample point between solid and liquid
            const point<double> freezing   (si::atmosphere, 1425.0 * si::celcius);
            // sample point between stishovite and coesite
            const point<double> sc0_sample (7.75 * si::gigapascal, 0.0 * si::celcius);
            // sample point between stishovite and coesite
            const point<double> sc1_sample (11.0 * si::gigapascal, 1400.0 * si::celcius);
            // sample point between alpha and coesite
            const point<double> ac_sample  (1.8 * si::gigapascal, 0.0 * si::celcius);
            // sample point between beta and coesite
            const point<double> bc_sample  (4.3 * si::gigapascal, 2200.0 * si::celcius);
            // sample point between alpha and beta
            const point<double> ab_sample  (0.0 * si::gigapascal, 573.0 * si::celcius);
            // sample point between beta and cristobalite alpha
            const point<double> bca_sample (0.0 * si::gigapascal, 870.0 * si::celcius);
            // sample point between beta and cristobalite beta
            const point<double> bcb_sample (1.0 * si::gigapascal, 1705.0 * si::celcius);
            // sample point between cristobalite alpha and cristobalite beta
            const si::temperature<double>     Tcacb = 1470.0 * si::celcius;
            // latent heat of vaporization and molar mass
            const si::specific_energy<double> L  = 11770e3 * si::joule / si::kilogram;
            const si::molar_mass<double>      M  = 60.08 * si::gram/si::mole;
    
            return PhaseDiagram(
                supercriticality(critical),
                basic_vaporus(L, M, critical),
                basic_liquidus(slg_triple, freezing),
                {
                    LinearPhaseRegion(stishovite,
                        unbounded_low_temperature(), 
                        unbounded_high_temperature(),  
                        pressure_lerp(sc0_sample, sc1_sample), 
                        unbounded_high_pressure()),
                    LinearPhaseRegion(coesite,
                        unbounded_low_temperature(), 
                        unbounded_high_temperature(),  
                        pressure_lerp(ac_sample, bc_sample), 
                        unbounded_high_pressure()),
                    LinearPhaseRegion(alpha,
                        unbounded_low_temperature(), 
                        unbounded_high_temperature(),  
                        pressure_lerp(ab_sample, abc_triple), 
                        unbounded_high_pressure()),
                    LinearPhaseRegion(beta,
                        unbounded_low_temperature(), 
                        unbounded_high_temperature(),  
                        pressure_lerp(bca_sample, bcb_triple), 
                        unbounded_high_pressure()),
                    LinearPhaseRegion(cristobalite_alpha,
                        unbounded_low_temperature(), Tcacb,  
                        unbounded_low_pressure(),    unbounded_high_pressure()),
                },
                cristobalite_alpha
            );
        }()},

    {compounds::carbon, 
        [](){
            const double supercritical = -3;
            const double vapor = -2;
            const double liquid = -1;
            const double graphite = 0;
            const double diamond = 1;
            const point<double> carbon   (2200.0 * si::atmosphere, 6810.0 * si::kelvin); // Leider (1973)
            const point<double> triple   (107e5 * si::pascal,      4765.0 * si::kelvin);
            const point<double> freezing (si::atmosphere,          3500.0 *si::celcius); // Perry, lower bound
            const     double M (0.012);               // molar mass, kg/mol
            const     double L (355.8*si::kilojoule/(12.011*si::gram) /(si::joule/si::kilogram)); // specific latent heat of vaporization (J/kg)
            constexpr double bv(1/t0 - mv * std::log(p0)); // intercept for clapeyron equation for vaporus
            const     double ml(1e-3);                // slope of clapeyron equation repurposed for liquidus, estimated from phase diagram
            const     double bl(-3.411e-5);           // intercept for clapeyron equation repurposed for liquidus, estimted from phase diagram
            return PhaseDiagram(
                supercriticality(critical),
                basic_vaporus(L, M, critical),
                basic_liquidus(triple, freezing),
                {
                    LinearPhaseRegion(graphite,
                        unbounded_low_temperature(), triple.temperature,  
                        unbounded_low_pressure(),    1e10*si::pascal),
                },
                diamond
            );
        }()},
    }
}}