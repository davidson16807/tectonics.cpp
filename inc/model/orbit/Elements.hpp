#pragma once

namespace orbit {

	template <typename scalar>
	struct Elements
	{

		scalar semi_major_axis;
		scalar eccentricity;
		scalar inclination;
		scalar longitude_of_ascending_node;
		scalar argument_of_periapsis;
		scalar mean_anomaly;

		constexpr Elements(
			const scalar semi_major_axis,
			const scalar eccentricity,
			const scalar inclination,
			const scalar longitude_of_ascending_node,
			const scalar argument_of_periapsis,
			const scalar mean_anomaly
		) noexcept:
			semi_major_axis(semi_major_axis),
			eccentricity(eccentricity),
			inclination(inclination),
			longitude_of_ascending_node(longitude_of_ascending_node),
			argument_of_periapsis(argument_of_periapsis),
			mean_anomaly(mean_anomaly)
		{}

        constexpr Elements<scalar> advance(const scalar anomaly_offset) const noexcept {
	        return Elements(
	            semi_major_axis, 
	            eccentricity, 
	            inclination, 
	            longitude_of_ascending_node, 
	            argument_of_periapsis, 
	            mean_anomaly + anomaly_offset
	        );
        }
	};

}

