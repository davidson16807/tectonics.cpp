#include <Stratum>

namespace air
{


    enum AtmosphericMassPools
    {
        N2,  
        O2,  
        CO2, 
        H2O, 
        CH4, 
        CF4, // perflouromethane, recommended by Zubrin (1996) for terraforming Mars
        C2H6,
        Ar,  
        He,  
        H2,  
        // CO 
        // NH3
        // NOx
        // SOx
        COUNT
    };

    /*
    "Atmosphere" is a component that holds the mass pools of an atmosphere.
    It possesses only extensive orthogonal state variables and side-effect-free methods.
    It encapsulates variables and functions that are probably not useful elsewhere,
     but it intentionally does not encapsulate member attributes since there is no need for it. 
    It has no concept of climate, since that would require nonorthogonal state variables. 
    That responsibility is left to the "Climate" component.
    for more information about the Entity-Component-System pattern, 
     see https://en.wikipedia.org/wiki/Entity_component_system
    */
    class Atmosphere
    {
        /*
        "get_molecular_refraction_of_refractive_index_at_stp" returns molecular refraction 
        from refractive index at standard temperature and pressure
        see description from https://encyclopedia2.thefreedictionary.com/Molecular+Refraction
        or https://goldbook.iupac.org/terms/view/M03979 for the related concept, "molar refraction"
        see trying-to-find-molecular-refraction.md for how this was derived
        */
        float get_molecular_refraction_of_refractive_index_at_stp = function(float n) {
            return ((n*n-1)/(n*n+2)) * units::standard_molar_volume / units::mole;
        }

        // TODO: hoist these constants out of atmosphere as need arises
        static constexpr std::array<float, AtmosphericMassPools::COUNT> molecular_masses = {
            28.034  * units::dalton, // N2  
            31.9988 * units::dalton, // O2  
            44.01   * units::dalton, // CO2 
            18.02   * units::dalton, // H2O 
            16.043  * units::dalton, // CH4 
            88.004  * units::dalton, // CF4  
            30.070  * units::dalton, // C2H6
            39.948  * units::dalton, // Ar  
            4.0026  * units::dalton, // He  
            2.016   * units::dalton  // H2  
        };
        /*
        NOTE: used to determine specific heat capacity
        from http://hyperphysics.phy-astr.gsu.edu/hbase/Tables/heatcap.html
        */
        static constexpr std::array<float, AtmosphericMassPools::COUNT> molecular_degrees_of_freedom = {
            5.0, // N2  
            5.1, // O2  
            6.8, // CO2 
            6.0, // H2O 
            6.0, // CH4 
            6.0, // C2H6
            6.0, // CF4 
            3.0, // Ar  
            3.0, // He  
            4.9  // H2  
        };
        // from https://en.wikipedia.org/wiki/Kinetic_diameter
        static constexpr std::array<float, AtmosphericMassPools::COUNT> molecular_diameters = {
            365 * units::picometer, // N2  
            346 * units::picometer, // O2  
            330 * units::picometer, // CO2 
            265 * units::picometer, // H2O 
            380 * units::picometer, // CH4 
            470 * units::picometer, // CF4 
            443 * units::picometer, // C2H6
            340 * units::picometer, // Ar  
            260 * units::picometer, // He  
            289 * units::picometer  // H2  
        };

        /*
        "molecular_refractions" can be thought of as a refractive tendency when applied to a volume per molecule
        see description from https://encyclopedia2.thefreedictionary.com/Molecular+Refraction
        or https://goldbook.iupac.org/terms/view/M03979 for the related concept, "molar refraction"
        derived using refractive indices from https://www.engineeringtoolbox.com/refractive-index-d_1264.html
        as well as https://refractiveindex.info/?shelf=organic&book=ethane&page=Loria
        */
        static constexpr std::array<float, AtmosphericMassPools::COUNT> molecular_refractions = {
            get_molecular_refraction_of_refractive_index_at_stp(1.000298), // N2  
            get_molecular_refraction_of_refractive_index_at_stp(1.000271), // O2  
            get_molecular_refraction_of_refractive_index_at_stp(1.000449), // CO2 
            get_molecular_refraction_of_refractive_index_at_stp(1.000261), // H2O 
            get_molecular_refraction_of_refractive_index_at_stp(1.000444), // CH4 
            get_molecular_refraction_of_refractive_index_at_stp(1.000752), // C2H6
            get_molecular_refraction_of_refractive_index_at_stp(1.000281), // Ar  
            get_molecular_refraction_of_refractive_index_at_stp(1.000035), // He  
            get_molecular_refraction_of_refractive_index_at_stp(1.000132)  // H2  
        }

        /*
        DERIVED SCALARS, PRIVATE
        these are here to prevent saturating the user with functions that they will likely never use
        */

        /*
        "mean_molecular_refraction_between_diameters" returns the average molecular refraction 
        for particles whose diameter falls within a range
        */
        float mean_molecular_refraction_between_diameters(float lo, float hi) 
        {
            float sum_molecular_refraction(0);
            float sum_molecule_count(0);
            for (unsigned int i = 0, li = AtmosphericMassPools::COUNT; i < li; i++) {
                float diameter = molecular_diameters[i];
                float molecule_count = mass_pools[i] / molecular_masses[i];
                if (lo < diameter && diameter < hi) {
                    sum_molecular_refraction += molecular_refractions[i] * molecule_count;
                    sum_molecule_count += molecule_count;
                }
            };
            return sum_molecular_refraction / sum_molecule_count;
        }

        /*
        "mean_molecular_refraction_between_diameters" returns the average molecular diameter 
        for particles whose diameter falls within a range
        */
        float mean_molecular_diameter_between_diameters(float lo, float hi) 
        {
            var sum_molecular_diameter(0);
            var sum_molecule_count(0);
            for (unsigned int i = 0, li = AtmosphericMassPools::COUNT; i < li; i++) {
                var diameter = molecular_diameters[pool];
                var molecule_count = mass_pools[pool] / molecular_masses[pool];
                if (lo < diameter && diameter < hi) {
                    sum_molecular_diameter += diameter * molecule_count;
                    sum_molecule_count += molecule_count;
                }
            };
            return sum_molecular_diameter / sum_molecule_count;
        }


    public:
        // ORTHOGONAL, EXTRINSIC, STATE:
        // such state is always public and can be modified at any time
        std::array<float, AtmosphericMassPools::COUNT> mass_pools;


        // DERIVED SCALARS, PUBLIC
        float total_mass() {
            float sum_mass(0);
            for (unsigned int i = 0, li = AtmosphericMassPools::COUNT; i < li; i++) {
                sum_mass += mass_pools[i];
            }
        }
        float molecule_count() {
            float sum_count(0);
            for (unsigned int i = 0, li = AtmosphericMassPools::COUNT; i < li; i++) {
                sum_count += mass_pools[i] / molecular_masses[i];
            }
        }
        float mean_molecular_mass() {
            return total_mass() / molecule_count();
        }
        float specific_heat_capacity() 
        {
            float sum_degrees_of_freedom(0);
            float sum_molecule_count(0);
            for (unsigned int i = 0, li = AtmosphericMassPools::COUNT; i < li; i++) {
                float molecule_count = mass_pools[pool] / molecular_masses[pool];
                float degrees_of_freedom = molecular_degrees_of_freedom[pool] / 2 + 1;
                sum_degrees_of_freedom += degrees_of_freedom * molecule_count;
                sum_molecule_count += molecule_count;
            };
            return (sum_degrees_of_freedom / sum_molecule_count) * (physics::boltzmann_constant / mean_molecular_mass());
        }

        float scale_height(const float gravity, const float temperature) 
        {
            return physics::boltzmann_constant * temperature / (mean_molecular_mass() * gravity);
        }
        float surface_pressure(const float gravity, const float surface_area) 
        {
            return total_mass() * gravity / surface_area;
        }
        float surface_density(const float gravity, const float surface_area, const float temperature) 
        {
            float total_mass = total_mass();
            float surface_pressure = total_mass * gravity / surface_area;
            return total_mass * surface_pressure / (molecule_count() * physics::boltzmann_constant * temperature);
        }
        float surface_molecular_density(const float gravity, const float surface_area, const float temperature) 
        {
            return surface_density(gravity, surface_area, temperature) / mean_molecular_mass();
        }
        float lapse_rate(const float gravity) 
        {
            return gravity / specific_heat_capacity();
        }




        /*
        "rayleigh_scattering_cross_section" indicates the rayleigh scattering cross section for surface air.
        This is the cross sectional area of a single particle that can scatter a ray of light of given wavelength.
        Multiply it by surface_molecular_density() to get a scattering coefficient, 
         then use the scattering coefficient to find the fraction of intensity lost to rayleigh scattering via Beer's law. 
        */
        float rayleigh_scattering_cross_section(const float wavelength) 
        {
            float R = mean_molecular_refraction_between_diameters(0, wavelength);
            /*
            see Platt (2007)
            Platt states σ = 24*π^3/λ^4 * ((n*n-1)/(n*n+2))^2 / (count/volume)^2
            and R = (n*n-1)/(n*n+2) * volume / count
            So that simplifies to σ = 24*π^3/λ^4 * R^2
            */
            return 24.0f*M_PI*M_PI*M_PI*R*R / (wavelength*wavelength*wavelength*wavelength);
        }
        /*
        "mie_scattering_cross_section" indicates the mie scattering cross section for surface air.
        This is the cross sectional area of a single particle that can scatter with a ray of light of given wavelength.
        Multiply it by surface_molecular_density() to get a scattering coefficient, 
         then use the scattering coefficient to find the fraction of intensity lost to mie scattering via Beer's law.
        */
        float mie_scattering_cross_section(const float wavelength) 
        {
            float r = mean_molecular_diameter_between_diameters(wavelength, Infinity) / 2.0f;
            /*
            We actually model mie scattering cross section as the optical scattering cross section,
             which is closely related but is meant more for particles that are much larger than the wavelength
            This makes it easier since the absorption cross section is equivalent to the particle cross section.
            See http://www.mike-willis.com/Tutorial/rainscatter.htm
             or http://www.radartutorial.eu/01.basics/Rayleigh-%20versus%20Mie-Scattering.en.html
             for an introduction to the different scattering regimes.
            One last thing: we check for NaNs in case there are no particles large enough to cause mie scattering
             if this  this case we return a very small number.
            */
            return !isNaN(r)? M_PI*r*r : 1e-35;
        }
        /*
        "absorption_cross_section" indicates the absorption cross section for surface air.
        This is the cross sectional area of a single particle that can scatter with a ray of light of given wavelength.
        Multiply it by surface_molecular_density() to get an absorption coefficient, 
         then use the absorption coefficient to find the fraction of intensity lost to absorption via Beer's law.
        */
        float absorption_cross_section(const float wavelength_lo, const float wavelength_hi) {
            float sum_cross_section(0);
            float sum_molecule_count(0);

            // convert from wavelength to wavenumber
            float lo = 1.0f/wavelength_hi;
            float hi = 1.0f/wavelength_lo;
            float range = hi - lo;
            float molecule_count;

            /*
            molecular absorption cross sections can be treated as a set of functions.
            Each function accepts a range of the electromagnetic spectrum 
             (expressed as a range of wavenumbers, in units of waves per meter, A.K.A. meter^-1)
             and returns a crude approximation for the effective cross section 
             of a particle in that mass pool exposed to that range of the spectrum.
             (expressed in meter^2 per particle)
            We want to capture a large range of effects
             (greenhouse gas effect, ozone layer, color of plants, physically based rendering, etc.)
             however the model must be performant and its output need only be representational, 
             so we prefer crude approximations that cover a broad range of the spectrum.
            Unlike virtually every other variable involving light, this variable uses wavenumber instead of wavelength.
            This is because wavenumber scales with energy, 
             and since we assume the user mostly cares about processes related to energy,
             we can take an average cross section across a range of wavenumbers and it should have some physical meaning. 
            */
            molecule_count = mass_pools[AtmosphericMassPools::N2] / molecular_masses[AtmosphericMassPools::N2];
            sum_cross_section += pow(10, lerp::integral_of_lerp(
                [6.5e6, 1e7,   8e7,   1e9], 
                [-35,   -20.5, -21.5, -23], lo, hi) / range) * molecule_count;
            sum_molecule_count += molecule_count;

            /*
            NOTE: we assume that if UV light and O2 is present, then there must also be ozone,
             so we treat O2 as ozone within UV wavelengths.
            TODO: revisit this, try to model O2/O3 conversion, and separate out their cross sections
            */
            molecule_count = mass_pools[AtmosphericMassPools::O2] / molecular_masses[AtmosphericMassPools::O2];
            sum_cross_section += pow(10, lerp::integral_of_lerp(
                [0,  2e5,7e5,9e5,1.6e6,3e6, 3.5e6, 4.8e6, 6.1e6, 7.3e6, 8.1e6, 9.6e6, 1.2e7], 
                [-28,-26,-31,-28,-35,  -35, -26.8, -26.5, -21.3, -20.8, -22.3, -23.3, -22.0], lo, hi) / range) * molecule_count;
            sum_molecule_count += molecule_count;


            molecule_count = mass_pools[AtmosphericMassPools::CO2] / molecular_masses[AtmosphericMassPools::CO2];
            sum_cross_section += pow(10, lerp::integral_of_lerp(
                [ 0,  1.5e6,5e6,  6.6e6,7.5e6,9e6,  4.7e7,1.5e8,5e8,2e9], 
                [-26,-34,  -30.5,-22.5,-22,   -20.5,-21,  -22,  -23,-26], lo, hi) / range) * molecule_count;
            sum_molecule_count += molecule_count;


            molecule_count = mass_pools[AtmosphericMassPools::H2O] / molecular_masses[AtmosphericMassPools::H2O];
            sum_cross_section += pow(10, lerp::integral_of_lerp(
                [0,  100, 2.1e6, 2.7e6, 6e6,   1.3e7,  2e8  ], 
                [-26,-24, -31.5, -29.5, -21.5, -20.5,  -22.5], lo, hi) / range) * molecule_count;
            sum_molecule_count += molecule_count;


            molecule_count = mass_pools[AtmosphericMassPools::CH4] / molecular_masses[AtmosphericMassPools::CH4];
            sum_cross_section += pow(10, lerp::integral_of_lerp(
                [3.7e4, 2.9e5, 1.9e6, 2.3e6, 2.4e6, 6.2e6, 7.6e6, 1e7,   7e7  ],
                [-31,   -25,   -31,   -31,   -31,   -27,   -21,   -20.3, -22.5], lo, hi) / range) * molecule_count;
            sum_molecule_count += molecule_count;


            molecule_count = mass_pools[AtmosphericMassPools::C2H6] / molecular_masses[AtmosphericMassPools::C2H6];
            sum_cross_section += pow(10, lerp::integral_of_lerp(
                [5.6e6, 7.6e6, 1.2e7, 5.3e7, 1.9e8],
                [-35,   -20.6, -20,   -21.5, -22.6], lo, hi) / range) * molecule_count;
            sum_molecule_count += molecule_count;


            molecule_count = mass_pools[AtmosphericMassPools::Ar] / molecular_masses[AtmosphericMassPools::Ar];
            sum_cross_section += 1e-35 * molecule_count;
            sum_molecule_count += molecule_count;


            molecule_count = mass_pools[AtmosphericMassPools::He] / molecular_masses[AtmosphericMassPools::He];
            sum_cross_section += 1e-35 * molecule_count;
            sum_molecule_count += molecule_count;


            molecule_count = mass_pools[AtmosphericMassPools::H2] / molecular_masses[AtmosphericMassPools::H2];
            sum_cross_section += pow(10, lerp::integral_of_lerp(
                [5e6,  1e7,   2.6e7, 5.7e7], 
                [-35,  -20.6, -21.6, -22.6], lo, hi) / range) * molecule_count;
            sum_molecule_count += molecule_count;


            // NOTE: the remaining cross section functions here are for possible future use
            molecule_count = mass_pools[AtmosphericMassPools::O3] / molecular_masses[AtmosphericMassPools::O3];
            sum_cross_section += pow(10, lerp::integral_of_lerp(
                [0,  2e5,7e5,9e5,1.6e6,2e6,2.5e6,2.8e6,3e6,3.5e6,4.6e6,6e6,7.7e6,1.2e7], 
                [-28,-26,-31,-28,-24,  -25,-27,  -24.5,-23,-21,  -22.5,-22,-21,  -21  ], lo, hi) / range) * molecule_count;
            sum_molecule_count += molecule_count;


            // NOTE: the remaining cross section functions here are for possible future use
            molecule_count = mass_pools[AtmosphericMassPools::N2O] / molecular_masses[AtmosphericMassPools::N2O];
            sum_cross_section += pow(10, lerp::integral_of_lerp(
                [5e4, 6e4, 2.6e5, 7.7e5, 2.7e6, 7.6e6, 7.8e7, 2.1e8, 4.4e8], 
                [-35, -25, -24.4, -29,   -35,   -20.4, -21.4, -22.4, -21.8], lo, hi) / range) * molecule_count;
            sum_molecule_count += molecule_count;

            return sum_cross_section / sum_molecule_count;
        }
    };
}

