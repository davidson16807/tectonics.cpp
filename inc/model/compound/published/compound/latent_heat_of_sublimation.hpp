#pragma once

// in-house libraries
#include <units/si.hpp>

#include <model/compound/ids.hpp>
#include <model/compound/table/PartialTable.hpp>
#include "latent_heat_of_vaporization.hpp"
#include "latent_heat_of_fusion.hpp"

namespace compound { 
namespace published { 

    table::PartialTable<LatentHeatTemperatureRelation> latent_heat_of_sublimation = latent_heat_of_fusion + latent_heat_of_vaporization;

}}