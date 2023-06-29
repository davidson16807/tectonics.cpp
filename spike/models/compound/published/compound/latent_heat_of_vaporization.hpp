#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/ids.hpp>
#include <models/compound/relation/compound/LatentHeatTemperatureRelation.hpp>
#include <models/compound/table/PartialTable.hpp>

namespace compound { 
namespace published { 

    using LatentHeatTemperatureRelation = relation::LatentHeatTemperatureRelation;
    table::PartialTable<LatentHeatTemperatureRelation> latent_heat_of_vaporization {
        {ids::water,            
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(18.01528*si::kilogram),
                647.1f, 5.66e7f, 0.612041f, -0.625697f, 0.398804f, 
                273.160f, 647.096f)}, 
        {ids::nitrogen,         
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(28.0134*si::kilogram),
                126.21f, 0.74905e7f, 0.40406f, -0.317f, 0.27343f, 
                63.150f, 126.200f)},
        {ids::oxygen,           
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(31.9988*si::kilogram),
                154.59f, 0.9008e7f, 0.4542f, -0.4096f, 0.3183f,
                54.360f, 154.580f)},
        {ids::carbon_dioxide,   
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(44.0095*si::kilogram),
                304.13f, 2.173e7f, 0.382f, -0.4339f, 0.42213f, 
                216.580f, 304.0f)},
        {ids::methane,          
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(16.0425*si::kilogram),
                190.56f, 1.0194e7f, 0.26087f, -0.14694f, 0.22154f, 
                90.690f, 190.0f)},
        {ids::argon,             
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(39.948*si::kilogram),
                150.87f, 0.84215e7f, 0.28333f, 0.033281f, 0.030551f, 
                83.780f, 150.860f)},    
        {ids::helium,           
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(4.0026*si::kilogram),
                5.19f, 0.012504e7f, 1.3038f, -2.6954f, 1.7098f, 
                2.2f, 5.0f)},   
        {ids::hydrogen,         
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(2.01588*si::kilogram),
                32.97f, 0.10127e7f, 0.698f, -1.817f, 1.447f, 
                13.950f, 32.0f)},
        {ids::ammonia,          
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(17.03052*si::kilogram),
                405.56f, 3.1523e7f, 0.3914f, -0.2289f, 0.2309f, 
                195.410f, 405.0f)},
        {ids::ozone,            
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(31.9988*si::kilogram),
                261.1f, 0.9008e7f, 0.4542f, -0.4096f, 0.3183f, 
                54.36f, 154.580f)},
        {ids::nitrous_oxide,    
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(44.0128*si::kilogram),
                309.52f, 2.2724e7f, 0.22278f, 0.29352f, -0.13493f, 
                182.300f, 309.0f)},
        {ids::sulfur_dioxide,   
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(64.0638*si::kilogram),
                430.64f, 2.846e7f, -0.24905f, 0.62158f, -0.020421f, 
                197.670f, 430.0f)},
        {ids::nitric_oxide,     
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(30.0061*si::kilogram),
                180.150f, 0.94287e7f, -2.0627f, 3.2659f, -1.0186f,
                109.500f, 180.0f)},
        {ids::carbon_monoxide,  
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(28.0101*si::kilogram),
                132.86f, 0.8585e7f, 0.4921f, -0.326f, 0.2231f, 
                68.130f, 132.0f)},
        {ids::ethane,           
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(30.069*si::kilogram),
                305.36f, 2.1091e7f, 0.60646f, -0.55492f, 0.32799f, 
                90.350f, 305.0f)},
        {ids::hydrogen_cyanide, 
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(27.02534*si::kilogram),
                456.65f, 3.3907e7f, 0.43574f, -0.56984f, 0.36017f, 
                259.830f, 456.0f)},
        {ids::ethanol,          
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(46.06844*si::kilogram),
                513.9f, 6.5831e7f, 1.1905f, -1.7666f, 1.0012f, 
                159.050f, 513.0f)},
        {ids::formaldehyde,     
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(30.02598*si::kilogram),
                410.3f, 2.9575e7f, 0.098296f, 0.28373f, 0.0f,
                155.150, 410.0f)},
        {ids::formic_acid,      
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(46.0257*si::kilogram),
                588.0f, 2.3195e7f, 1.9091f, -5.0003f, 3.2641f, 
                250.000f, 588.0f)},
        {ids::perflouromethane, 
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(88.0043*si::kilogram),
                277.54f, 1.9311e7f, 0.94983f, -1.0615f, 0.51894f, 
                89.560f, 227.510f)},
        {ids::benzene,          
            relation::get_dippr_temperature_relation_106(
                si::kelvin, si::joule/(78.11184*si::kilogram),
                562.050f, 5.0007e7f, 0.65393f, -0.27698f, 0.029569f,
                278.680f, 562.0f)},
        {ids::pyrimidine,       49.79*79 * si::kilojoule / (80.088*si::gram)},
        {ids::quartz,           11770e3 * si::joule / si::kilogram},
        {ids::halite,           40810.0 * si::calorie/(80.088 * si::gram)}, // Perry
        // {ids::corundum,         },
        // {ids::apatite,          },
        {ids::carbon,           355.8*si::kilojoule/(12.011*si::gram)},
        // {ids::calcite,          },
        // {ids::orthoclase,       },
        // {ids::andesine,         },
        // {ids::augite,           },
        // {ids::forsterite,       },
        // {ids::goethite,         },
        // {ids::pyrite,           },
        // {ids::hematite,         },
        {ids::gold,             324.0 * si::kilojoule/(196.967*si::gram)},
        {ids::silver,           60720.0 *  si::calorie/(107.868 * si::gram)}, // Perry
        {ids::copper,           204.8 * si::joule/si::gram},
        // {ids::magnetite,        },
        // {ids::chalcocite,       },
        // {ids::chalcopyrite,     },
    };

}}