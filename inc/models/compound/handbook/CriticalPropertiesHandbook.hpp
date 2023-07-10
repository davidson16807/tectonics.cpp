#pragma once

// in-house libraries
#include <models/compound/correlation/published.hpp>
#include <models/compound/table/FullTable.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound{
namespace estimated{


    /*
    `CriticalPropertiesHandbook` serves the role of a chemical handbook 
    that contains the critical properties of chemical compounds
    */
    struct CriticalPropertiesHandbook
    {
        table::FullTable<si::pressure<double>> pressure;
        table::FullTable<si::molar_volume<double>> volume;
        table::FullTable<si::temperature<double>> temperature;
        table::FullTable<double> compressibility;
        table::FullTable<point<double>> point_;

        CriticalPropertiesHandbook(
            const table::PartialTable<si::pressure<double>> Pc, // critical pressure
            const table::PartialTable<si::molar_volume<double>> Vc, // critical volume
            const table::PartialTable<si::temperature<double>> Tc, // critical temperature
            const table::PartialTable<double> Zc, // critical compressibility
            const table::FullTable<unsigned int> A, // atoms per molecule
            const table::FullTable<si::molar_mass<double>> M // molar mass
        ){

            // via Klincewicz
            pressure = table::complete(Pc, 
                table::derive<si::pressure<double>>(correlation::estimate_critical_pressure_from_klincewicz, M, A));

            // via Klincewicz
            volume = table::complete(Vc, 
                table::derive<si::molar_volume<double>>(correlation::estimate_critical_molar_volume_from_klincewicz, M, A));

            // via Klincewicz ⟶ Ihmels
            temperature = table::complete(Tc, 
                table::derive<si::temperature<double>>(correlation::estimate_critical_temperature_from_ihmels, pressure, volume)); 

            // via Klincewicz ⟶ Ihmels ⟶ definition of compressibility
            compressibility = table::complete(Zc, 
                table::derive<double>(correlation::get_critical_compressibility, pressure, temperature, volume));

            point_ = table::derive<point<double>>(
                [](si::pressure<double> p, si::temperature<double> T){ return point<double>(p,T); }, 
                pressure, temperature);

        }
    };
    

}}

