#pragma once

// in-house libraries
#include <model/compound/correlation/published.hpp>
#include <model/compound/table/FullTable.hpp>
#include <model/compound/table/PartialTable.hpp>

namespace compound{
namespace estimated{

    /*
    `SamplePointHandbook` serves the role of a chemical handbook 
    that contains properties regarding the molecules of compounds 
    that must be inferred from other properties
    */
    struct SamplePointHandbook
    {
        table::FullTable<point<double>> solid_point;
        table::FullTable<point<double>> liquid_point;
        table::FullTable<point<double>> gas_point;
        table::FullTable<point<double>> freezing_point;
        table::FullTable<point<double>> boiling_point;
        table::FullTable<si::temperature<double>> freezing_point_temperature;
        table::FullTable<si::pressure<double>> freezing_point_pressure;
        table::FullTable<si::temperature<double>> boiling_point_temperature;
        table::FullTable<si::pressure<double>> boiling_point_pressure;

        SamplePointHandbook(
            const table::FullTable<point<double>>& freezing_point0,
            const table::PartialTable<point<double>>& boiling_point0,
            const CriticalPropertiesHandbook& critical,
            const BasicHandbook& basic
        ): 
            freezing_point(freezing_point0)
        {

            freezing_point_temperature = 
                table::derive<si::temperature<double>>(
                    [](point<double> pT){ return pT.temperature; }, 
                    freezing_point);

            freezing_point_pressure = 
                table::derive<si::pressure<double>>(
                    [](point<double> pT){ return pT.pressure; }, 
                    freezing_point);

            boiling_point = 
                table::complete(
                    boiling_point0,
                    table::derive<point<double>>(
                        [](si::molar_mass<double> M, si::temperature<double> Tc, si::temperature<double> Tf){ 
                            return point<double>(si::atmosphere, si::clamp(correlation::estimate_normal_boiling_point_from_klincewicz(M,Tc), Tf, Tc));
                        }, 
                        basic.molar_mass,
                        critical.temperature,
                        freezing_point_temperature
                    )
                );

            boiling_point_temperature = 
                table::derive<si::temperature<double>>(
                    [](point<double> pT){ return pT.temperature; }, 
                    boiling_point);

            boiling_point_pressure = 
                table::derive<si::pressure<double>>(
                    [](point<double> pT){ return pT.pressure; }, 
                    boiling_point);

            point<double> standard = point<double>(si::standard_pressure, si::standard_temperature);

            solid_point = 
                table::derive<point<double>>(
                    [standard](point<double> freezing){ return  standard.temperature < freezing.temperature? standard : freezing; }, 
                    freezing_point);

            liquid_point = 
                table::derive<point<double>>(
                    [standard](point<double> freezing, point<double> boiling){ 
                        return freezing.temperature < standard.temperature && standard.temperature < boiling.temperature? 
                            standard : (freezing + boiling)/2.0; }, 
                    freezing_point, 
                    boiling_point);

            gas_point = 
                table::derive<point<double>>(
                    [standard](point<double> boiling){ return boiling.temperature < standard.temperature? standard : boiling; }, 
                    boiling_point);

        }
    };

}}

