// derived attributes: icosphere<Strata> -> icosphere<T>
void get_mass_pool   (const series<StrataStore>& crust, OxygenPlanetMassPoolTypes type, const series<float>& out)
void get_mass        (const series<StrataStore>& crust, const series<float>& out)
void get_volume      (const series<StrataStore>& crust, const series<float>& out)
void get_density     (const series<StrataStore>& crust, const series<float>& out)
void get_thickness   (const series<StrataStore>& crust, const series<float>& out)

// derived attributes: icosphere<Strata> -> icosphere<StrataValues<T>>
void get_thermal_conductivity    (const series<StrataStore>& crust, series<StrataValues<float>>& out)
void get_overburden_mass         (const series<StrataStore>& crust, series<StrataValues<float>>& out)
void get_depths                  (const series<StrataStore>& crust, const std::array<float, mass_pool_count>& mass_pool_densities, series<StrataValues<float>>& out)

// setters: icosphere<Strata> x icosphere<StrataValues<T>> -> icosphere<Strata>  (remember, no state modification!)
void set_max_temperature_received(const series<StrataStore>& crust, const series<StrataValues<float>>& max_temperature_received, const series<StrataStore>& out)
void set_max_pressure_received   (const series<StrataStore>& crust, const series<StrataValues<float>>& max_pressure_received,    const series<StrataStore>& out)

// icosphere<Strata> <-> layered<Strata>
template <typename T>
void get_blocks_from_strata(
	const series<float>& surface_displacement, 
    const SpheroidGrid& strata_grid, const series<StrataValues<float>>& strata_depths, const series<StrataValues<T>>& strata_values, 
    const LayeredSpheroidGrid& block_grid, series<T>& block_values)
template <typename T>
void get_strata_from_blocks(
	const series<float>& surface_displacement, 
    const LayeredSpheroidGrid& block_grid, const series<T>& block_values, 
    const SpheroidGrid& strata_grid, const series<StrataValues<float>>& strata_depths, series<StrataValues<T>>& strata_values)

// icosphere<Strata> -> layered<Strata>
void get_sediment(const series<StrataStore>& crust, const series<Stratum>& out)

// model logic
void phase_transition(
	const series<StrataValues<float>>& pressure, const series<StrataValues<float>>& temperature, 
	series<StrataStore>& solidification_delta,
	std::array<MassPool, Stratum::mass_pool_count> vaporization_delta&,
	std::array<MassPool, Stratum::mass_pool_count> melting_delta&
)
void erosion(
	const series<Stratum>& sediment, const series<float>& surface_height, const series<float>& precipitation, 
	const float seconds, const float surface_gravity,
	series<Stratum>& erosion_delta, series<Stratum>& deposited 
)
void weathering(
	const series<StrataStore>& strata, const series<float>& surface_temperature_range, 
	const float seconds, const float surface_gravity,
	series<StrataStore>& weathering_delta, series<Stratum>& deposited 
)
void cratering(
	const series<StrataStore>& strata, const glm::vec3 impact_epicenter, const float impact_energy,
	series<StrataStore>& excavation_delta, series<Stratum>& deposited
)

void merge   (const std::vector<Plate>& plates, series<StrataStore>& master)
void split   (const series<StrataStore>& master, std::vector<Plate>& plates)
void deposit (const series<Stratum>& master_deposited, const std::vector<Plate>& plates, std::vector<Plate>& out)
void apply   (const series<StrataStore>& master_delta, const std::vector<Plate>& plates, std::vector<Plate>& out)
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