#pragma once

// in-house libraries
#include <units/si.hpp>

#include <models/compound/ids.hpp>
#include <models/compound/table/PartialTable.hpp>
#include "latent_heat_of_vaporization.hpp"
#include "latent_heat_of_fusion.hpp"

namespace compound { 
namespace published { 

    table::PartialTable<si::specific_energy<double>> latent_heat_of_sublimation = latent_heat_of_fusion + latent_heat_of_vaporization;

}}