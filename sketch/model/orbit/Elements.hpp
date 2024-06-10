#pragma once

namespace orbit {

	template <typename Tfloat>
	struct Elements
	{
		const Tfloat semi_major_axis;
		const Tfloat eccentricity;
		const Tfloat inclination;
		const Tfloat longitude_of_ascending_node;
		const Tfloat argument_of_periapsis;
		Tfloat mean_anomaly;
		Elements(
			const Tfloat semi_major_axis,
			const Tfloat eccentricity,
			const Tfloat inclination,
			const Tfloat longitude_of_ascending_node,
			const Tfloat argument_of_periapsis,
			const Tfloat mean_anomaly
		):
			semi_major_axis(semi_major_axis),
			eccentricity(eccentricity),
			inclination(inclination),
			longitude_of_ascending_node(longitude_of_ascending_node),
			argument_of_periapsis(argument_of_periapsis),
			mean_anomaly(mean_anomaly)
		{}
	};

}