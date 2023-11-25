#pragma once

// in-house libraries
#include <unit/si.hpp>

#include <model/compound/ids.hpp>
#include <model/compound/relation/compound/LatentHeatTemperatureRelation.hpp>
#include <model/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    using LatentHeatTemperatureRelation = relation::LatentHeatTemperatureRelation;
    table::PartialTable<LatentHeatTemperatureRelation> latent_heat_of_vaporization {
        {compounds::water,            
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(18.01528*si::kilogram),
                647.1f, 5.66e7f, 0.612041f, -0.625697f, 0.398804f, 
                273.160f, 647.096f)}, 
        {compounds::nitrogen,         
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(28.0134*si::kilogram),
                126.21f, 0.74905e7f, 0.40406f, -0.317f, 0.27343f, 
                63.150f, 126.200f)},
        {compounds::oxygen,           
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(31.9988*si::kilogram),
                154.59f, 0.9008e7f, 0.4542f, -0.4096f, 0.3183f,
                54.360f, 154.580f)},
        {compounds::carbon_dioxide,   
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(44.0095*si::kilogram),
                304.13f, 2.173e7f, 0.382f, -0.4339f, 0.42213f, 
                216.580f, 304.0f)},
        {compounds::methane,          
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(16.0425*si::kilogram),
                190.56f, 1.0194e7f, 0.26087f, -0.14694f, 0.22154f, 
                90.690f, 190.0f)},
        {compounds::argon,             
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(39.948*si::kilogram),
                150.87f, 0.84215e7f, 0.28333f, 0.033281f, 0.030551f, 
                83.780f, 150.860f)},    
        {compounds::helium,           
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(4.0026*si::kilogram),
                5.19f, 0.012504e7f, 1.3038f, -2.6954f, 1.7098f, 
                2.2f, 5.0f)},   
        {compounds::hydrogen,         
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(2.01588*si::kilogram),
                32.97f, 0.10127e7f, 0.698f, -1.817f, 1.447f, 
                13.950f, 32.0f)},
        {compounds::ammonia,          
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(17.03052*si::kilogram),
                405.56f, 3.1523e7f, 0.3914f, -0.2289f, 0.2309f, 
                195.410f, 405.0f)},
        {compounds::ozone,            
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(31.9988*si::kilogram),
                261.1f, 0.9008e7f, 0.4542f, -0.4096f, 0.3183f, 
                54.36f, 154.580f)},
        {compounds::nitrous_oxide,    
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(44.0128*si::kilogram),
                309.52f, 2.2724e7f, 0.22278f, 0.29352f, -0.13493f, 
                182.300f, 309.0f)},
        {compounds::sulfur_dioxide,   
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(64.0638*si::kilogram),
                430.64f, 2.846e7f, -0.24905f, 0.62158f, -0.020421f, 
                197.670f, 430.0f)},
        {compounds::nitric_oxide,     
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(30.0061*si::kilogram),
                180.150f, 0.94287e7f, -2.0627f, 3.2659f, -1.0186f,
                109.500f, 180.0f)},
        {compounds::carbon_monoxide,  
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(28.0101*si::kilogram),
                132.86f, 0.8585e7f, 0.4921f, -0.326f, 0.2231f, 
                68.130f, 132.0f)},
        {compounds::ethane,           
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(30.069*si::kilogram),
                305.36f, 2.1091e7f, 0.60646f, -0.55492f, 0.32799f, 
                90.350f, 305.0f)},
        {compounds::hydrogen_cyanide, 
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(27.02534*si::kilogram),
                456.65f, 3.3907e7f, 0.43574f, -0.56984f, 0.36017f, 
                259.830f, 456.0f)},
        {compounds::ethanol,          
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(46.06844*si::kilogram),
                513.9f, 6.5831e7f, 1.1905f, -1.7666f, 1.0012f, 
                159.050f, 513.0f)},
        {compounds::formaldehyde,     
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(30.02598*si::kilogram),
                410.3f, 2.9575e7f, 0.098296f, 0.28373f, 0.0f,
                155.150, 410.0f)},
        {compounds::formic_acid,      
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(46.0257*si::kilogram),
                588.0f, 2.3195e7f, 1.9091f, -5.0003f, 3.2641f, 
                250.000f, 588.0f)},
        {compounds::perflouromethane, 
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(88.0043*si::kilogram),
                277.54f, 1.9311e7f, 0.94983f, -1.0615f, 0.51894f, 
                89.560f, 227.510f)},
        {compounds::benzene,          
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(78.11184*si::kilogram),
                562.050f, 5.0007e7f, 0.65393f, -0.27698f, 0.029569f,
                278.680f, 562.0f)},
        {compounds::pyrimidine,       49.79*79 * si::kilojoule / (80.088*si::gram)},
        {compounds::quartz,           11770e3 * si::joule / si::kilogram},
        {compounds::halite,           40810.0 * si::calorie/(80.088 * si::gram)}, // Perry
        {compounds::corundum,         115.7 * si::kilocalorie/(101.96  * si::gram)}, // Ruff & Konshak, from Pavlushkin (1963)
        // {compounds::apatite,          },
        {compounds::carbon,           355.8*si::kilojoule/(12.011*si::gram)},
        // {compounds::calcite,          },
        // {compounds::orthoclase,       },
        // {compounds::andesine,         },
        // {compounds::augite,           },
        // {compounds::forsterite,       },
        // {compounds::goethite,         },
        // {compounds::pyrite,           },
        // {compounds::hematite,         },
        {compounds::gold,             324.0 * si::kilojoule/(196.967*si::gram)},
        {compounds::silver,           60720.0 *  si::calorie/(107.868 * si::gram)}, // Perry
        {compounds::copper,           204.8 * si::joule/si::gram},
        // {compounds::magnetite,        },
        // {compounds::chalcocite,       },
        // {compounds::chalcopyrite,     },
    };

}}