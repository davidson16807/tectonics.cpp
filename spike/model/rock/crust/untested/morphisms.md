// derived attributes: icosphere<Strata> -> icosphere<T>
void get_mass_pool   (const Series<StrataStore>& crust, OxygenPlanetMassPoolTypes type, const Series<float>& out)
void get_mass        (const Series<StrataStore>& crust, const Series<float>& out)
void get_volume      (const Series<StrataStore>& crust, const Series<float>& out)
void get_density     (const Series<StrataStore>& crust, const Series<float>& out)
void get_thickness   (const Series<StrataStore>& crust, const Series<float>& out)

// derived attributes: icosphere<Strata> -> icosphere<StrataValues<T>>
void get_thermal_conductivity    (const Series<StrataStore>& crust, Series<StrataValues<float>>& out)
void get_overburden_mass         (const Series<StrataStore>& crust, Series<StrataValues<float>>& out)
void get_depths                  (const Series<StrataStore>& crust, const std::array<float, mass_pool_count>& mass_pool_densities, Series<StrataValues<float>>& out)

// setters: icosphere<Strata> x icosphere<StrataValues<T>> -> icosphere<Strata>  (remember, no state modification!)
void set_max_temperature_received(const Series<StrataStore>& crust, const Series<StrataValues<float>>& max_temperature_received, const Series<StrataStore>& out)
void set_max_pressure_received   (const Series<StrataStore>& crust, const Series<StrataValues<float>>& max_pressure_received,    const Series<StrataStore>& out)

// icosphere<Strata> <-> layered<Strata>
template <typename T>
void get_blocks_from_strata(
	const Series<float>& surface_displacement, 
    const SpheroidGrid& strata_grid, const Series<StrataValues<float>>& strata_depths, const Series<StrataValues<T>>& strata_values, 
    const LayeredSpheroidGrid& block_grid, Series<T>& block_values)
template <typename T>
void get_strata_from_blocks(
	const Series<float>& surface_displacement, 
    const LayeredSpheroidGrid& block_grid, const Series<T>& block_values, 
    const SpheroidGrid& strata_grid, const Series<StrataValues<float>>& strata_depths, Series<StrataValues<T>>& strata_values)

// icosphere<Strata> -> layered<Strata>
void get_sediment(const Series<StrataStore>& crust, const Series<Stratum>& out)

// model logic
void phase_transition(
	const Series<StrataValues<float>>& pressure, const Series<StrataValues<float>>& temperature, 
	Series<StrataStore>& solidification_delta,
	std::array<MassPool, Stratum::mass_pool_count> vaporization_delta&,
	std::array<MassPool, Stratum::mass_pool_count> melting_delta&
)
void erosion(
	const Series<Stratum>& sediment, const Series<float>& surface_height, const Series<float>& precipitation, 
	const float seconds, const float surface_gravity,
	Series<Stratum>& erosion_delta, Series<Stratum>& deposited 
)
void weathering(
	const Series<StrataStore>& strata, const Series<float>& surface_temperature_range, 
	const float seconds, const float surface_gravity,
	Series<StrataStore>& weathering_delta, Series<Stratum>& deposited 
)
void cratering(
	const Series<StrataStore>& strata, const glm::vec3 impact_epicenter, const float impact_energy,
	Series<StrataStore>& excavation_delta, Series<Stratum>& deposited
)

void merge   (const std::vector<Plate>& plates, Series<StrataStore>& master)
void split   (const Series<StrataStore>& master, std::vector<Plate>& plates)
void deposit (const Series<Stratum>& master_deposited, const std::vector<Plate>& plates, std::vector<Plate>& out)
void apply   (const Series<StrataStore>& master_delta, const std::vector<Plate>& plates, std::vector<Plate>& out)
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