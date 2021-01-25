#pragma once

// C libraries
#include <math.h>       // log10 

// in-house libraries
#include <si.hpp>  // estimate_compressibility_factor

#include "compressibility.hpp"  // estimate_compressibility_factor

namespace compound{
    namespace property
    {

        /*
        NOTE: 
        `compound::property` serves as a "category of primitives" where objects are variables of primitive types.
        This namespace maps to and from critical point metrics: pressure, volume, temperature, compressibility, and acentricity


% https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZARgAYAaUgXXJADcBDAGwFcYkQBhAfQGUQAvuXSZc+QimLFKNeszYcACt2B1eAwcJAZseAkWIAmGbUat2iEAB0rAJwAWEbgBlNI3eIMBmE3PMcbByd+IXcxfUkAVl8zBUtAx24AcTdtUT0JEgA2GPkLEABrZNSdcMziAHZc-3irGBwGYtC0jwiSCmJZWPyAFRL0z0lpTtM8jgAJbjgAAh7Z7hxp5Rx+1vLjEb84kAA1F2mAWVWy73JN7o493kPjjINo87HLPaSb5tK7yRzHmpAD6Z4dGmNnqjRS7wGbUqZy6Tz+AO4QKK4K0H0GJB8hlhvyut3RxB8Xmx2yu8xWELWBnIABZifkiq4KScUMZaaNfjxGajIZkfGytvkbBAALYwADmDDxbWi-IutWwYuFkqZn2QOVlcKFoolUsyVQ1vxBDRcuqIAA4aXSAlYReLldzKSgAJyW9nbZSqZwaFX46QG7Y2BVK02SVlW54mn1Qvnh3b7HoLEMkGWxl5JiiRWMHHoJ4VJ6SZt29bgAY0Os3mACMk8ZCwKOAmy3NOUmfHW5SBGxWtXak9TyO24V25pM6EnooPfl3lGXA1hFfawqqcpPttPS+W5j2dVG9QPY+uy-8t3VjVyl+iLavi2WZ9M9k3gTbtYuWszkC7rw2N3eHxXlGOu4GLWB4bn+ABaG59EBUT7kW363hukGPoo+bkFkoEltm3DVjByA+Bh8GWI2SY5IR9bET+pZJi65Edoe97UXhZDobGv5MQ675SKxRFxiW+bGHRQ5gUhHEXlC0RCVOVFlshAnkGamFLCJsliW+qrSE6WZVvmFBabxACCRwCLIMBQGK8BEKAABmtgikgxggDgEBID4FFgCwTBMLQTAMJWMBMIoPIcEwMDWeSWi2fZiBuc5DlER5Xk+X5AVBY6ICheFIC0HA9hYFliAALTEM0UV5ogVROS5iAUBRICKDAti2AAnsQkTENlGUpYFwWWJlEU2XZ5UWlVSDSHVnC+WgMDNXZhDJf5PXpf1qRlUgLqjYgjlyol3ldYtaXMhlYUDSAa2IBtcVbdE7meXtvkHb1x1ZT5WBgPkUAQDgOBmatQ1jbVV3EP2t1JftqVPStr3vRwn3fb9pX-YgLGbW5O13QtEPLSdf3Rdxm0g+jYMPVjR0rYjePjVdhiVUT93dYdnzPSs0MfV9P1QLj5VGLQV03XTmNLWTOOs7D7MI5FSM85tBIJRj4NC0zUMgLl+UrEVJWS3jsXVTkoP0492MvRlb1s-DnMU9zOtjdtcIAKIsCWBQwPNCuM4MzNc2NN3U3rAtu5DOOW97vPVcDcvEwzgfG0wpti+bXvIyN1O1f7JOKx7yv2DADCc5YkAw6LlifSwlahZ1-lgHnXikMHyOXdVhiE08u2C+7ESe7Q2e5xwBfsEXIAl2X-cgJX1e11r5WGIDje2zUrcB0bLMgN3efgAQI+xzDxcQKX5e0GPrkT4N0VN6HDn8y38vp+3EidyreUFZrJ9Tz7jfN78ABCvDOIYnU309Ww857CnXOoYPW1NL7z2vlHJe-8447wTnXcB59kZQO2EkCA0AlSuwAXAnKj91bFWQcnMOfs4S8GztZayzVCoACkID2DAHAfQbdo6gKRoYBuY1yG-F4AwZyhUABKMAsCwCYP-WBwtjaqwKsQyeDluGIGpKnK+kdDbSOXlvM2HNE411QdCfWbC4F130ajcaacpFKxFg-NWY1TFU2qtSCxaiDak2sTHBBg9xYWwUTFRxrk57ulwAAL0apIjRHiOHRS8AEpOEc3EZw7srbR8ddEONQWYjsQiIAAHct4sLAIVIKW8xQRPcR7IBYoQF6McldWJCTjGaM6rIohz8zpIy8HUpxLjoHqIqckmxqTEHpL8V4a29dGmL2aQPOGoyX6uQmdSIJFgF54JmSbbe3ikFjImdPXiPQki8D-k0qJeiQb1PQasmBkTM5BzGRcxuqi+mJNviFIZXi5kSwWTFR5SBllTPWWc2ZPi9Fv1ch-bYzh6gsJYIVXgeAArlKSXfZWrSkDyJ+V4cFW1elxDWVYu5nitlfN8VinFAKjHTOBZsnR3yOkxIgdVLwtM4TQpgIVAA0vAHAoVbDIreX1Gx6KNamKZf8tGrjTlEq0Z80FYrUGGBWUgAltzBnErpWShl5UWWoOpJKl50r1WypJfKsZlV6m8PxTcgZqL7lYotY3A11r+koveRqtJ9LzpeFIRfXiQiGBO3qFowlHcqk1NMb65RVrrmusFffYZ2z5natckommgLQ12qyqYpR1JWWGupTK+Bpqdk-JUXqjali1VZpNZqxO5aCZ4tja89hxa6112cXq55LqW34NsXI9p51KVXU7VSoFRaQWlpTco7pEreIAEl7CiiYAgI1d9w3RPKsO5lzrehHI6mu91y8RWYunfqvVI0q22qPfWpZkLm2HqFR6kZXqkbnoJsqxAqrr1PtrZ6zmBC7HIw7X85Ru6OCLuXauwtYbgHHsIfYvx1JQPUiuSqm1brf1tv-fW8VyjL1SpgzWlpCHRVIcdf8gjBbx3GvrVG99V7MMJrlQnQDT8O30fzT2x9zGS26LY20jjeqY3obja2ydyah25u7fkTlW8SwwFyVgEsISBWALgyRoDg6kaRBnv8+9HA5NvQU0plTan0obsTmfAmkQO04usyeptHBv7OA6iZAQQA
\begin{tikzcd}
                                   &                                                                    &                                            &                                                                                                     &                                                    &                                                                        &                                                                                   &                                                                                               &                                                                                                &                                                                              & C_S                                                         & P_{vS}                                   & \rho_L \arrow[r]                                                                                        & \rho_S \arrow[l, shift left]                               &  & \rho_G                                      & k_G                                                  & \eta_G                                          \\
                                   &                                                                    &                                            &                                                                                                     &                                                    &                                                                        &                                                                                   &                                                                                               &                                                                                                &                                                                              & T \arrow[u, "Perry151"]                                     & H_s T T_t P_t \arrow[u, "Clapeyron"]     & V_L M \arrow[u] \arrow[ddd, dotted]                                                                     & V_S M \arrow[d, dotted] \arrow[u]                          &  & V_G M \arrow[u] \arrow[ddd, dotted]         & M C_v \eta_G \arrow[u, dotted] \arrow[r, shift left] & M C_v k_G \arrow[u, dotted] \arrow[l, "Eucken"] \\
                                   &                                                                    &                                            &                                                                                                     &                                                    &                                                                        &                                                                                   &                                                                                               &                                                                                                &                                                                              &                                                             &                                          &                                                                                                         & V_S                                                        &  &                                             &                                                      &                                                 \\
                                   &                                                                    &                                            &                                                                                                     &                                                    &                                                                        &                                                                                   &                                                                                               &                                                                                                &                                                                              &                                                             &                                          &                                                                                                         & V_S T_t \arrow[d] \arrow[u, dotted]                        &  &                                             &                                                      &                                                 \\
                                   & k_L                                                                & C_L                                        & \omega \arrow[rrr, no head, Rightarrow, bend left]                                                  &                                                    & \sigma \arrow[rrrrrr, no head, Rightarrow, bend left]                  & \omega \arrow[rr, no head, Rightarrow, bend left]                                 & \eta_L                                                                                        & \omega                                                                                         & P_{vL}                                                                       &                                                             & \sigma \arrow[r, shift right] \arrow[dd] & V_L \arrow[l, "BSL2"']                                                                                  & V_L T_t \arrow[l, dotted] \arrow[u, "Goodman", shift left] &  & V_G                                         &                                                      &                                                 \\
MTT_m \arrow[ru, "Sheffy-Johnson"] & T_c M T T_b \arrow[u, "Sato-Reidel", shift left] \arrow[d, dotted] & T_c T C_L \arrow[u] \arrow[r, shift right] & T_c T \omega \arrow[d, dotted] \arrow[r, "Pitzer"] \arrow[u, dotted] \arrow[l, "Rowlinson-Poling"'] & T_c T H_v \arrow[l, shift left] \arrow[ld, dotted] & T_c P_c \sigma \arrow[u, dotted] \arrow[rd, dotted] \arrow[ru, "TGS2"] & T_c P_c M T \omega \arrow[r] \arrow[u, dotted] \arrow[d, dotted]                  & T_c P_c M T \eta_L \arrow[l, "Letsou-Stiel", shift left] \arrow[u, dotted] \arrow[ld, dotted] & T_c P_c V_c T \omega \arrow[r, "Lee-Kestler", shift left] \arrow[rd, dotted] \arrow[u, dotted] & T_c P_c V_c T P_v \arrow[d, dotted] \arrow[l] \arrow[u, dotted]              &                                                             &                                          & T_c V_c Z_c T \arrow[u, "Rackett"'] \arrow[d, dotted]                                                   &                                                            &  & T_c P_c Z_c T P \arrow[u] \arrow[d, dotted] &                                                      &                                                 \\
                                   & T_cMT_b \arrow[ddd, dotted] \arrow[rr, dotted]                     &                                            & T_c \arrow[ll, shift left]                                                                          &                                                    &                                                                        & T_c P_c \arrow[lll, dotted] \arrow[rrr, "Ihmels"'] \arrow[lu, "TGS1", shift left] &                                                                                               &                                                                                                & T_c P_c V_c \arrow[rrrdd] \arrow[r, dotted] \arrow[lll, dotted, shift right] & P_c V_c \arrow[l, "Ihmels"', shift right] \arrow[r, dotted] & V_c \arrow[uu, "BSL1", shift left]       & T_c V_c Z_c \arrow[dd, shift left]                                                                      &                                                            &  & T_c P_c Z_c \arrow[llldd]                   &                                                      &                                                 \\
                                   &                                                                    &                                            &                                                                                                     &                                                    &                                                                        &                                                                                   &                                                                                               &                                                                                                &                                                                              &                                                             &                                          &                                                                                                         &                                                            &  &                                             &                                                      &                                                 \\
                                   &                                                                    &                                            &                                                                                                     &                                                    &                                                                        &                                                                                   &                                                                                               &                                                                                                &                                                                              &                                                             &                                          & T_c P_c V_c Z_c \arrow[llluu, dotted, shift right] \arrow[rrruu, dotted, shift left] \arrow[uu, dotted] &                                                            &  &                                             &                                                      &                                                 \\
                                   & M T_b \arrow[uuu, "Klincewicz"', shift right]                      &                                            &                                                                                                     &                                                    &                                                                        &                                                                                   &                                                                                               &                                                                                                &                                                                              & AM \arrow[uuu, "Klincewicz"']                               &                                          &                                                                                                         &                                                            &  &                                             &                                                      &                                                
\end{tikzcd}


        `vetted.hpp` describes mappings within this category which are vetted by peer review. 
        Other mappings within this category describe custom correlations which were created exclusively for use within the model.
        These more speculative correlations are largely created out of desperation, 
        and should only be used when there are no other peer-review correlations available to derive a property.
        The speculative correlations are included in a separate file, `speculative.hpp`, 
        so that we can clearly delineatly what is or is not peer-reviewed,
        and we append them with the prefix `guess_*` (as opposed to `estimate_*`) to warn about their limited use.
        */

        // taken from https://eng.libretexts.org/Bookshelves/Chemical_Engineering/Book%3A_Distillation_Science_(Coleman)/03%3A_Critical_Properties_and_Acentric_Factor
        constexpr double get_critical_compressibility(const si::pressure critical_pressure, const si::temperature critical_temperature, const si::molar_volume critical_volume)
        {
            return si::unitless(critical_pressure * critical_volume / (si::universal_gas_constant * critical_temperature));
        }
        constexpr si::molar_volume get_critical_volume(const si::pressure critical_pressure, const si::temperature critical_temperature, const double critical_compressibility)
        {
            return critical_compressibility * si::universal_gas_constant * critical_temperature / critical_pressure;
        }
        constexpr si::pressure get_critical_pressure(const si::temperature critical_temperature, const si::molar_volume critical_volume, const double critical_compressibility)
        {
            return critical_compressibility * si::universal_gas_constant * critical_temperature / critical_volume;
        }
        constexpr si::temperature get_critical_temperature(const si::pressure critical_pressure, const si::molar_volume critical_volume, const double critical_compressibility)
        {
            return critical_pressure * critical_volume/(critical_compressibility*si::universal_gas_constant);
        }
        // from definition of the acentric factor: https://en.wikipedia.org/wiki/Acentric_factor
        constexpr double get_acentric_factor(const si::pressure liquid_saturated_vapor_pressure_at_reduced_temperature_of_0_7, const si::pressure critical_pressure)
        {
            return -log10(si::unitless(liquid_saturated_vapor_pressure_at_reduced_temperature_of_0_7/critical_pressure)) - 1.0;
        }

        // Klincewicz method (1982): https://en.wikipedia.org/wiki/Klincewicz_method
        constexpr si::temperature estimate_critical_temperature_from_klincewicz(const si::molar_mass molar_mass, const si::temperature normal_boiling_point)
        {
            double molar_mass_in_grams = si::unitless(molar_mass / (si::gram/si::mole));
            double normal_boiling_point_in_kelvin = si::unitless(normal_boiling_point / si::kelvin);
            double temperature_in_kelvin = 50.2 - 0.16 * molar_mass_in_grams + 1.41 * normal_boiling_point_in_kelvin;
            return temperature_in_kelvin * si::kelvin; 
        }
        // Klincewicz method (1982): https://en.wikipedia.org/wiki/Klincewicz_method
        constexpr si::pressure estimate_critical_pressure_from_klincewicz(const si::molar_mass molar_mass, const int atom_count)
        {
            double molar_mass_in_grams = si::unitless(molar_mass / (si::gram / si::mole));
            double Y = 0.335 + 0.009 * molar_mass_in_grams + 0.019 * atom_count;
            double critical_pressure_in_bars = molar_mass_in_grams/Y;
            return critical_pressure_in_bars * si::bar;
        }
        // Klincewicz method (1982): https://en.wikipedia.org/wiki/Klincewicz_method
        constexpr si::molar_volume estimate_critical_molar_volume_from_klincewicz(const si::molar_mass molar_mass, const int atom_count)
        {
            double molar_mass_in_grams = si::unitless(molar_mass / (si::gram / si::mole));
            double critical_molar_volume_in_cm3 = 20.1 + 0.88 * molar_mass_in_grams + 13.4 * atom_count;
            return critical_molar_volume_in_cm3 * (si::centimeter3/si::mole);
        }

        // Ihmels (2010)
        constexpr si::temperature estimate_critical_temperature_from_ihmels(const si::pressure critical_pressure, const si::molar_volume critical_molar_volume)
        {
            double critical_volume_in_m3_per_mole = si::unitless(critical_molar_volume / (si::meter3/si::mole));
            double critical_pressure_in_pascal = si::unitless(critical_pressure / si::pascal);
            double critical_temperature_in_kelvin = (critical_pressure_in_pascal + 0.025f) * critical_volume_in_m3_per_mole / 2.215f;
            return critical_temperature_in_kelvin * si::kelvin;
        }
        // Ihmels (2010)
        constexpr si::molar_volume estimate_critical_molar_volume_from_ihmels(const si::temperature critical_temperature, const si::pressure critical_pressure)
        {
            double critical_pressure_in_pascal = si::unitless(critical_pressure / si::pascal);
            double critical_temperature_in_kelvin = si::unitless(critical_temperature / si::kelvin);
            double critical_volume_in_m3_per_mole = critical_temperature_in_kelvin * 2.215 / (critical_pressure_in_pascal + 0.025f);
            return critical_volume_in_m3_per_mole * (si::meter3/si::mole);
        }
        // Ihmels (2010)
        constexpr si::pressure estimate_critical_pressure_from_ihmels(const si::temperature critical_temperature, const si::molar_volume critical_molar_volume)
        {
            double critical_temperature_in_kelvin = si::unitless(critical_temperature / si::kelvin);
            double critical_volume_in_m3_per_mole = si::unitless(critical_molar_volume / (si::meter3/si::mole));
            double critical_pressure_in_pascal = 2.215 * critical_temperature_in_kelvin / critical_volume_in_m3_per_mole - 0.025f;
            return critical_pressure_in_pascal * si::pascal;
        }

        // Sheffy Johnson: https://chemicals.readthedocs.io/en/latest/chemicals.thermal_conductivity.html#pure-low-pressure-liquid-correlations
        constexpr si::thermal_conductivity estimate_thermal_conductivity_as_liquid_from_sheffy_johnson(
            const si::molar_mass molar_mass,
            const si::temperature temperature,
            const si::temperature normal_melting_point
        ){
            double molar_mass_in_grams = si::unitless(molar_mass / (si::gram / si::mole));
            double temperature_in_kelvin = si::unitless(temperature / si::kelvin);
            double standard_melting_point_in_kelvin = si::unitless(normal_melting_point / si::kelvin);
            double liquid_thermal_conductivity_in_watts_per_meter_kelvin = 
                1.951 * (1.0 - 0.00126 * (temperature_in_kelvin - standard_melting_point_in_kelvin)) / 
                         (pow(standard_melting_point_in_kelvin, 0.216f)*pow(molar_mass_in_grams, 0.3f));
            return liquid_thermal_conductivity_in_watts_per_meter_kelvin * si::watt / (si::meter*si::kelvin);
        }

        // Sato-Riedel method: https://chemicals.readthedocs.io/en/latest/chemicals.thermal_conductivity.html#pure-low-pressure-liquid-correlations
        constexpr si::thermal_conductivity estimate_thermal_conductivity_as_liquid_from_sato_riedel(
            const si::molar_mass molar_mass,  
            const si::temperature temperature, 
            const si::temperature normal_boiling_point, 
            const si::temperature critical_temperature 
        ){
            double molar_mass_in_grams = si::unitless(molar_mass / (si::gram / si::mole));
            double liquid_thermal_conductivity_in_watts_per_meter_kelvin = 
                (1.1053 / sqrt(molar_mass_in_grams)) * (3 + 20.0 * pow(1.0 - si::unitless(         temperature / critical_temperature), 2.0/3.0)) / 
                                                        (3 + 20.0 * pow(1.0 - si::unitless(normal_boiling_point / critical_temperature), 2.0/3.0));
            return liquid_thermal_conductivity_in_watts_per_meter_kelvin * si::watt / (si::meter*si::kelvin);
        }

        constexpr si::molar_volume get_molar_volume_as_ideal_gas(const si::temperature temperature, const si::pressure pressure)
        {
            return si::universal_gas_constant * temperature / pressure;
        }

        constexpr si::specific_heat_capacity get_constant_pressure_heat_capacity_as_gas(
            si::molar_mass molar_mass,
            double degrees_of_freedom
        ){
            return degrees_of_freedom * (si::universal_gas_constant / molar_mass);   
        }

        // Rowlinson-Poling: https://chemicals.readthedocs.io/en/latest/chemicals.heat_capacity.html?highlight=rowlinson#chemicals.heat_capacity.Rowlinson_Poling
        constexpr si::molar_heat_capacity estimate_constant_pressure_heat_capacity_as_liquid_from_rowlinson_poling(
            const si::temperature critical_temperature,
            const si::temperature temperature,
            const double accentric_factor,
            const si::molar_heat_capacity constant_pressure_heat_capacity_as_gas
        ){
            double reduced_temperature = si::unitless(temperature / critical_temperature);
            double heat_capacity_phase_difference_versus_gas_constant = 1.586 + 
                0.49f/(1.0-reduced_temperature) + 
                accentric_factor * (4.2775 + 6.3f*pow(1.0 - reduced_temperature, 1.0/3.0)/reduced_temperature + 0.4355f/(1.0 - reduced_temperature));
            return heat_capacity_phase_difference_versus_gas_constant * si::universal_gas_constant + constant_pressure_heat_capacity_as_gas;
        }

        // Rowlinson-Poling: https://chemicals.readthedocs.io/en/latest/chemicals.heat_capacity.html?highlight=rowlinson#chemicals.heat_capacity.Rowlinson_Poling
        constexpr si::molar_heat_capacity estimate_constant_pressure_heat_capacity_as_gas_from_rowlinson_poling(
            const si::temperature critical_temperature,
            const si::temperature temperature,
            const double accentric_factor,
            const si::molar_heat_capacity constant_pressure_heat_capacity_as_liquid
        ){
            double reduced_temperature = si::unitless(temperature / critical_temperature);
            double heat_capacity_phase_difference_versus_gas_constant = 1.586 + 
                0.49f/(1.0-reduced_temperature) + 
                accentric_factor * (4.2775 + 6.3f*pow(1.0 - reduced_temperature, 1.0/3.0)/reduced_temperature + 0.4355f/(1.0 - reduced_temperature));
            return constant_pressure_heat_capacity_as_liquid - heat_capacity_phase_difference_versus_gas_constant * si::universal_gas_constant;
        }

        constexpr double estimate_acentric_factor_from_rowlinson_poling(
            const si::temperature critical_temperature,
            const si::temperature temperature,
            const si::molar_heat_capacity constant_pressure_heat_capacity_as_liquid,
            const si::molar_heat_capacity constant_pressure_heat_capacity_as_gas
        ){
            double reduced_temperature = si::unitless(temperature / critical_temperature);
            double heat_capacity_phase_difference_versus_gas_constant = si::unitless((constant_pressure_heat_capacity_as_liquid - constant_pressure_heat_capacity_as_gas) / si::universal_gas_constant);
            return (heat_capacity_phase_difference_versus_gas_constant - 1.586 - 0.49f/(1.0-reduced_temperature)) / (4.2775 + 6.3f*pow(1.0 - reduced_temperature, 1.0/3.0)/reduced_temperature + 0.4355f/(1.0 - reduced_temperature));
        }

        // Pitzer model: https://chemicals.readthedocs.io/en/latest/chemicals.phase_change.html#heat-of-vaporization-at-tb-correlations
        constexpr double estimate_accentric_factor_from_pitzer(
            const si::specific_energy latent_heat_of_vaporization,
            const si::molar_mass molar_mass,  
            const si::temperature temperature,
            const si::temperature critical_temperature
        ){
            double reduced_temperature = si::unitless(temperature / critical_temperature);
            return (si::unitless(latent_heat_of_vaporization * molar_mass / (si::universal_gas_constant * critical_temperature)) - 7.08 * pow(1.0 - reduced_temperature, 0.354)) / (10.95 * pow(1.0 - reduced_temperature, 0.456));
        }
        // Pitzer model: https://chemicals.readthedocs.io/en/latest/chemicals.phase_change.html#heat-of-vaporization-at-tb-correlations
        constexpr si::specific_energy estimate_latent_heat_of_vaporization_from_pitzer(
            const double accentric_factor,
            const si::molar_mass molar_mass,  
            const si::temperature temperature,
            const si::temperature critical_temperature
        ){
            double reduced_temperature = si::unitless(temperature / critical_temperature);
            return (7.08 * pow(1.0 - reduced_temperature, 0.354) + 10.95 * accentric_factor * pow(1.0 - reduced_temperature, 0.456)) * si::universal_gas_constant * critical_temperature / molar_mass;
        }

        // Letsou-Stiel method: https://chemicals.readthedocs.io/en/latest/chemicals.viscosity.html?highlight=letsou%20stiel#chemicals.viscosity.Letsou_Stiel
        constexpr si::dynamic_viscosity estimate_viscosity_as_liquid_from_letsou_stiel(
            const double acentric_factor,
            const si::molar_mass molar_mass,  
            const si::temperature temperature, 
            const si::temperature critical_temperature, 
            const si::pressure critical_pressure
        ){
            double molar_mass_in_grams = si::unitless(molar_mass / (si::gram / si::mole));
            double reduced_temperature = si::unitless(temperature / critical_temperature);
            double critical_temperature_in_kelvin = si::unitless(critical_temperature / si::kelvin);
            double critical_pressure_in_pascal =  si::unitless(critical_pressure / si::pascal);
            double eta_zeta_0 = (1.5174 - 2.135 * reduced_temperature + 0.75 * reduced_temperature * reduced_temperature) * 1e-5;
            double eta_zeta_1 = (4.2552 - 7.674 * reduced_temperature + 3.40 * reduced_temperature * reduced_temperature) * 1e-5;
            double zeta = 2173.424 * pow(critical_temperature_in_kelvin, 1.0/6.0)  /  (sqrt(molar_mass_in_grams) * pow(critical_pressure_in_pascal, 2.0/3.0));
            double eta_zeta = eta_zeta_0 + acentric_factor * eta_zeta_1;
            double eta = eta_zeta / zeta;
            return eta * si::pascal * si::second;
        }

        // Letsou-Stiel method: https://chemicals.readthedocs.io/en/latest/chemicals.viscosity.html?highlight=letsou%20stiel#chemicals.viscosity.Letsou_Stiel
        constexpr double estimate_acentric_factor_from_letsou_stiel(
            const si::dynamic_viscosity viscosity_as_liquid,
            const si::molar_mass molar_mass,  
            const si::temperature temperature, 
            const si::temperature critical_temperature, 
            const si::pressure critical_pressure
        ){
            double molar_mass_in_grams = si::unitless(molar_mass / (si::gram / si::mole));
            double reduced_temperature = si::unitless(temperature / critical_temperature);
            double critical_temperature_in_kelvin = si::unitless(critical_temperature / si::kelvin);
            double critical_pressure_in_atm =  si::unitless(critical_pressure / si::pascal);
            double eta_zeta_0 = (1.5174 - 2.135 * reduced_temperature + 0.75 * reduced_temperature * reduced_temperature) * 1e-5;
            double eta_zeta_1 = (4.2552 - 7.674 * reduced_temperature + 3.40 * reduced_temperature * reduced_temperature) * 1e-5;
            double zeta = 2173.424 * pow(critical_temperature_in_kelvin, 1.0/6.0)  /  (sqrt(molar_mass_in_grams) * pow(critical_pressure_in_atm, 2.0/3.0));
            return (si::unitless(viscosity_as_liquid * zeta / (si::pascal * si::second)) - eta_zeta_0) / eta_zeta_1;
        }

        constexpr si::length estimate_molecular_diameter_from_tee_gotoh_steward(
            const si::temperature critical_temperature, 
            const si::pressure critical_pressure
        ){
            double critical_pressure_in_atm = si::unitless(critical_pressure / si::standard_pressure);
            double critical_temperature_in_kelvin = si::unitless(critical_temperature / si::kelvin);
            double molecular_diameter_in_angstrom = 2.3647 * pow(critical_temperature_in_kelvin / critical_pressure_in_atm, 1.0/3.0);
            return molecular_diameter_in_angstrom * si::angstrom;
        }
        
        /*
        // NOTE: We're commenting this out due to poor accuracy, it's off by almost an order of magnitude 
        // for a property that doesn't vary much and might be better predicted by simply assuming a constant value.
        constexpr double estimate_acentric_factor_from_tee_gotoh_steward(
            const si::length molecular_diameter,
            const si::temperature critical_temperature, 
            const si::pressure critical_pressure
        ){
            double critical_pressure_in_atm = si::unitless(critical_pressure / si::standard_pressure);
            double critical_temperature_in_kelvin = si::unitless(critical_temperature / si::kelvin);
            double molecular_diameter_in_angstrom = si::unitless(molecular_diameter / si::angstrom);
            return -(molecular_diameter_in_angstrom * pow(critical_pressure_in_atm/critical_temperature_in_kelvin, 1.0/3.0) - 2.3551) / 0.0874;
        }
        */

        namespace {
            /*
            NOTE: 
            this namespace describes subfunctions of the Lee-Kesler method: https://en.wikipedia.org/wiki/Lee%E2%80%93Kesler_method
            */
            constexpr double f0(const double reduced_temperature) {
                return 5.92714 - 6.09648f/reduced_temperature - 1.28862f*log(reduced_temperature) + 0.169347f*reduced_temperature*reduced_temperature*reduced_temperature*reduced_temperature*reduced_temperature*reduced_temperature;
            }
            constexpr double f1(const double reduced_temperature) {
                return 15.2518 - 15.6875f/reduced_temperature - 13.4721f*log(reduced_temperature) + 0.43577 *reduced_temperature*reduced_temperature*reduced_temperature*reduced_temperature*reduced_temperature*reduced_temperature;
            }
        }

        // Lee Kesler method (1975): https://en.wikipedia.org/wiki/Lee%E2%80%93Kesler_method
        constexpr double estimate_accentric_factor_from_lee_kesler(
            const si::pressure liquid_saturated_vapor_pressure, 
            const si::temperature temperature, 
            const si::temperature critical_temperature,
            const si::pressure critical_pressure
        ){
            double reduced_pressure    = si::unitless(liquid_saturated_vapor_pressure / critical_pressure);
            double reduced_temperature = si::unitless(temperature / critical_temperature);
            return (log(reduced_pressure) - f0(reduced_temperature)) / f1(reduced_temperature);
        }
        // Lee Kesler method: https://en.wikipedia.org/wiki/Lee%E2%80%93Kesler_method
        constexpr si::pressure estimate_vapor_pressure_as_liquid_from_lee_kesler(
            const double acentric_factor, 
            const si::temperature temperature, 
            const si::temperature critical_temperature, 
            const si::pressure critical_pressure
        ){
            double reduced_temperature = si::unitless(temperature / critical_temperature);
            double reduced_pressure    = exp(acentric_factor * f1(reduced_temperature) + f0(reduced_temperature));
            return reduced_pressure * critical_pressure;
        }

        // Bird-Steward-Lightfoot: https://chemicals.readthedocs.io/en/latest/chemicals.lennard_jones.html#molecular-diameter-correlations
        constexpr si::length estimate_molecular_diameter_from_bird_steward_lightfoot_1(const si::molar_volume critical_molar_volume)
        {
            double critical_molar_volume_in_meter3_per_mole = si::unitless(critical_molar_volume / (si::milliliter/si::mole));
            double molecular_diameter_in_angstrom = 0.841 * pow(critical_molar_volume_in_meter3_per_mole, 1.0/3.0);
            return molecular_diameter_in_angstrom * si::angstrom;
        }

        // Bird-Steward-Lightfoot: https://chemicals.readthedocs.io/en/latest/chemicals.lennard_jones.html#molecular-diameter-correlations
        constexpr si::molar_volume estimate_critical_molar_volume_from_bird_steward_lightfoot_1(const si::length molecular_diameter)
        {
            double molecular_diameter_in_angstrom = si::unitless(molecular_diameter / si::angstrom);
            double critical_molar_volume_in_meter3_per_mole = pow(molecular_diameter_in_angstrom / 0.841f, 3.0);
            return critical_molar_volume_in_meter3_per_mole * (si::milliliter/si::mole);
        }

        // Bird-Steward-Lightfoot: https://chemicals.readthedocs.io/en/latest/chemicals.lennard_jones.html#molecular-diameter-correlations
        constexpr si::length estimate_molecular_diameter_from_bird_steward_lightfoot_2(const si::molar_volume liquid_molar_volume_at_melting_point)
        {
            double liquid_molar_volume_in_meter3_per_mole = si::unitless(liquid_molar_volume_at_melting_point / (si::milliliter/si::mole));
            double molecular_diameter_in_angstrom = 1.222 * pow(liquid_molar_volume_in_meter3_per_mole, 1.0/3.0);
            return molecular_diameter_in_angstrom * si::angstrom;
        }

        // Bird-Steward-Lightfoot: https://chemicals.readthedocs.io/en/latest/chemicals.lennard_jones.html#molecular-diameter-correlations
        constexpr si::molar_volume estimate_molar_volume_as_liquid_from_bird_steward_lightfoot_2(const si::length molecular_diameter)
        {
            double molecular_diameter_in_angstrom = si::unitless(molecular_diameter / si::angstrom);
            double liquid_molar_volume_in_meter3_per_mole = pow(molecular_diameter_in_angstrom / 1.222f, 3.0);
            return liquid_molar_volume_in_meter3_per_mole * (si::milliliter/si::mole);
        }

        // Goodman: https://chemicals.readthedocs.io/en/latest/chemicals.volume.html#pure-solid-correlations
        constexpr si::molar_volume estimate_molar_volume_as_solid_from_goodman(
            const si::molar_volume liquid_molar_volume_at_melting_point, 
            const si::temperature temperature, 
            const si::temperature triple_point_temperature
        ){
            double temperature_in_kelvin = si::unitless(temperature / si::kelvin);
            double triple_point_temperature_in_kelvin = si::unitless(triple_point_temperature / si::kelvin);
            double liquid_molar_volume_in_meter3_per_mole = si::unitless(liquid_molar_volume_at_melting_point / (si::meter3/si::mole));
            double solid_molar_volume_in_meter3_per_mole = liquid_molar_volume_in_meter3_per_mole / (1.28 - 0.16 * temperature_in_kelvin / triple_point_temperature_in_kelvin);
            return solid_molar_volume_in_meter3_per_mole * (si::meter3/si::mole);
        }

        // Goodman: https://chemicals.readthedocs.io/en/latest/chemicals.volume.html#pure-solid-correlations
        constexpr si::molar_volume estimate_molar_volume_as_liquid_from_goodman(
            const si::molar_volume solid_molar_volume, 
            const si::temperature temperature, 
            const si::temperature triple_point_temperature
        ){
            double temperature_in_kelvin = si::unitless(temperature / si::kelvin);
            double triple_point_temperature_in_kelvin = si::unitless(triple_point_temperature / si::kelvin);
            double solid_molar_volume_in_meter3_per_mole = si::unitless(solid_molar_volume / (si::meter3/si::mole));
            double liquid_molar_volume_in_meter3_per_mole = solid_molar_volume_in_meter3_per_mole * (1.28 - 0.16 * temperature_in_kelvin / triple_point_temperature_in_kelvin);
            return liquid_molar_volume_in_meter3_per_mole * (si::meter3/si::mole);
        }

        // Rackett equation
        constexpr si::molar_volume estimate_molar_volume_as_liquid_from_rackett(const si::temperature temperature, 
            const si::temperature critical_temperature, 
            const si::pressure critical_pressure, 
            const double critical_compressibility
        ){
            double reduced_temperature = si::unitless(temperature / critical_temperature);
            return si::universal_gas_constant * critical_temperature / critical_pressure * pow(critical_compressibility, 1.0 + pow(1.0 - reduced_temperature, 2.0/7.0));
        }

        constexpr si::molar_volume estimate_molar_volume_as_gas(
            const si::pressure pressure, 
            const si::pressure critical_pressure, 
            const si::temperature temperature, 
            const si::temperature critical_temperature, 
            const double critical_compressibility
        ){
            double compressibility = estimate_compressibility_factor(pressure, temperature, critical_pressure, critical_temperature, critical_compressibility);
            return compressibility * si::universal_gas_constant * temperature / pressure;
        }

        constexpr si::density get_density_from_molar_volume(const si::molar_volume molar_volume, const si::molar_mass molar_mass)
        {
            return molar_mass/molar_volume;
        }

        // Eucken: https://chemicals.readthedocs.io/en/latest/chemicals.thermal_conductivity.html#pure-low-pressure-liquid-correlations
        constexpr si::thermal_conductivity estimate_thermal_conductivity_as_gas_from_eucken(
            const si::dynamic_viscosity viscosity_as_gas,
            const si::molar_mass molar_mass,
            const si::specific_heat_capacity constant_volume_specific_heat_capacity_as_gas
        ){
            si::molar_heat_capacity constant_molar_volume_heat_capacity_as_gas = constant_volume_specific_heat_capacity_as_gas * molar_mass;
            return (1.0 + (9.0/4.0) / si::unitless(constant_molar_volume_heat_capacity_as_gas/si::universal_gas_constant))
                  * viscosity_as_gas * constant_molar_volume_heat_capacity_as_gas / molar_mass;
        }

        // Eucken: https://chemicals.readthedocs.io/en/latest/chemicals.thermal_conductivity.html#pure-low-pressure-liquid-correlations
        constexpr si::dynamic_viscosity estimate_viscosity_as_gas_from_eucken(
            const si::thermal_conductivity thermal_conductivity_as_gas,
            const si::molar_mass molar_mass,
            const si::specific_heat_capacity constant_volume_specific_heat_capacity_as_gas
        ){
            si::molar_heat_capacity constant_molar_volume_heat_capacity_as_gas = constant_volume_specific_heat_capacity_as_gas * molar_mass;
            return thermal_conductivity_as_gas *molar_mass / ((1.0 + (9.0/4.0) / si::unitless(constant_molar_volume_heat_capacity_as_gas/si::universal_gas_constant))*constant_molar_volume_heat_capacity_as_gas);
        }

        // Clapeyron: https://chemicals.readthedocs.io/en/latest/chemicals.vapor_pressure.html#sublimation-pressure-estimation-correlations
        constexpr si::pressure estimate_vapor_pressure_as_solid_from_clapeyron(
            const si::molar_mass molar_mass,
            const si::specific_energy latent_heat_of_sublimation,
            const si::temperature temperature,
            const si::temperature triple_point_temperature,
            const si::pressure triple_point_pressure
        ){
            return triple_point_pressure * exp(-si::unitless((latent_heat_of_sublimation*molar_mass / si::universal_gas_constant) * (1.0/temperature - 1.0/triple_point_temperature)));
        }
    }
}