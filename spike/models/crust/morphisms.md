// derived attributes: icosphere<Strata> -> icosphere<T>
void get_mass_pool   (const tmany<StrataStore>& crust, OxygenPlanetMassPoolTypes type, const tmany<float>& out)
void get_mass        (const tmany<StrataStore>& crust, const tmany<float>& out)
void get_volume      (const tmany<StrataStore>& crust, const tmany<float>& out)
void get_density     (const tmany<StrataStore>& crust, const tmany<float>& out)
void get_thickness   (const tmany<StrataStore>& crust, const tmany<float>& out)

// derived attributes: icosphere<Strata> -> icosphere<StrataValues<T>>
void get_thermal_conductivity    (const tmany<StrataStore>& crust, tmany<StrataValues<float>>& out)
void get_overburden_mass         (const tmany<StrataStore>& crust, tmany<StrataValues<float>>& out)
void get_depths                  (const tmany<StrataStore>& crust, const std::array<float, mass_pool_count>& mass_pool_densities, tmany<StrataValues<float>>& out)

// setters: icosphere<Strata> x icosphere<StrataValues<T>> -> icosphere<Strata>  (remember, no state modification!)
void set_max_temperature_received(const tmany<StrataStore>& crust, const tmany<StrataValues<float>>& max_temperature_received, const tmany<StrataStore>& out)
void set_max_pressure_received   (const tmany<StrataStore>& crust, const tmany<StrataValues<float>>& max_pressure_received,    const tmany<StrataStore>& out)

// icosphere<Strata> <-> layered<Strata>
template <typename T>
void get_blocks_from_strata(
	const tmany<float>& surface_displacement, 
    const SpheroidGrid& strata_grid, const tmany<StrataValues<float>>& strata_depths, const tmany<StrataValues<T>>& strata_values, 
    const LayeredSpheroidGrid& block_grid, tmany<T>& block_values)
template <typename T>
void get_strata_from_blocks(
	const tmany<float>& surface_displacement, 
    const LayeredSpheroidGrid& block_grid, const tmany<T>& block_values, 
    const SpheroidGrid& strata_grid, const tmany<StrataValues<float>>& strata_depths, tmany<StrataValues<T>>& strata_values)

// icosphere<Strata> -> layered<Strata>
void get_sediment(const tmany<StrataStore>& crust, const tmany<Stratum>& out)

// model logic
void phase_transition(
	const tmany<StrataValues<float>>& pressure, const tmany<StrataValues<float>>& temperature, 
	tmany<StrataStore>& solidification_delta,
	std::array<MassPool, Stratum::mass_pool_count> vaporization_delta&,
	std::array<MassPool, Stratum::mass_pool_count> melting_delta&
)
void erosion(
	const tmany<Stratum>& sediment, const tmany<float>& surface_height, const tmany<float>& precipitation, 
	const float seconds, const float surface_gravity,
	tmany<Stratum>& erosion_delta, tmany<Stratum>& deposited 
)
void weathering(
	const tmany<StrataStore>& strata, const tmany<float>& surface_temperature_range, 
	const float seconds, const float surface_gravity,
	tmany<StrataStore>& weathering_delta, tmany<Stratum>& deposited 
)
void cratering(
	const tmany<StrataStore>& strata, const glm::vec3 impact_epicenter, const float impact_energy,
	tmany<StrataStore>& excavation_delta, tmany<Stratum>& deposited
)

void merge   (const std::vector<Plate>& plates, tmany<StrataStore>& master)
void split   (const tmany<StrataStore>& master, std::vector<Plate>& plates)
void deposit (const tmany<Stratum>& master_deposited, const std::vector<Plate>& plates, std::vector<Plate>& out)
void apply   (const tmany<StrataStore>& master_delta, const std::vector<Plate>& plates, std::vector<Plate>& out)
void simplify(const std::vector<Plate>& plates, std::vector<Plate>& out)
void rift    (const std::vector<Plate>& plates, std::vector<Plate>& master)
void subduct (const std::vector<Plate>& plates, std::vector<Strata>& master)

/*
fix_delta = function(crust_delta, crust, scratch) 
is_conserved_delta = function(crust_delta, threshold) 
is_conserved_transport_delta = function(crust_delta, threshold) 
is_conserved_reaction_delta = function(crust_delta, threshold, scratch) 

// extensions to functions over Stratum
deposit_to_ids = function(crust, id_raster, value_crust, result_crust)
overlap = function(crust1, crust2, crust2_exists, crust2_on_top, result_crust)

// derived attributes
get_thickness = function(crust, material_density, thickness) {
get_buoyancy = function (density, material_density, surface_gravity, buoyancy) {
get_isostatic_displacements = function(thickness, density, material_density, displacement)
get_fluid_velocities = function(pressure, velocity)

// model logic
model_lithification = function(
        surface_height, seconds,
        material_density, surface_gravity,
        top_crust, crust_delta, crust_scratch)      // NOTE: no longer needed, subsumed by max_temperature/pressure_received
model_metamorphosis = function(
        surface_height, seconds,
        material_density, surface_gravity,
        top_crust, crust_delta, crust_scratch)      // NOTE: no longer needed, subsumed by max_temperature/pressure_received
model_weathering = function(
        surface_height, seconds,
        material_density, surface_gravity,
        top_crust, crust_delta, crust_scratch)
model_erosion = function(
        surface_height, seconds,
        material_density, surface_gravity,
        top_crust, crust_delta, crust_scratch)

merge_plates_to_master(plates, master)
update_rifting(world, plates)
update_subducted(world, plates)
calculate_deltas(lithosphere, seconds)
integrate_deltas(world, plates, seconds)
*/