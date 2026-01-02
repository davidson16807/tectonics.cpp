#pragma once

#include <cmath>

#include <glm/vec3.hpp>     // *vec3
#include <glm/geometric.hpp>// cross, normalize, dot, length
#include <glm/gtx/norm.hpp> // length2
#include <glm/mat3x3.hpp>

namespace orbit {
	/*
	`Properties` is a mathematical small category implemented as a class for which methods are conceptually pure functions.
	Arrows in the category map orbital properties of a single orbit for which the gravitational parameter is known.
	Objects in the category are orbital properties expressed as primitive data types and glm vectors.

	It implements the following commutative diagram:
	```
	% https://q.uiver.app/?q=WzAsMzAsWzMsNSwiXFxib2xke2h9Il0sWzYsNSwiXFxib2xke3IgXFx0aW1lcyB2fSJdLFs2LDAsIlxcYm9sZHtlfSJdLFsyLDUsIlxcYm9sZHtufSJdLFszLDYsImkiXSxbMSw1LCJcXE9tZWdhIl0sWzIsMCwiXFxib2xke2UgXFx0aW1lcyBufSJdLFsxLDAsIlxcb21lZ2EiXSxbOCwzLCJcXGJvbGR7ciBcXHRpbWVzIGV9Il0sWzgsMSwiXFxudV8wIl0sWzIsNywiXFxib2xke3IgXFx0aW1lcyBufSJdLFs2LDcsIlxcYm9sZHtyfSJdLFsxLDcsInVfMCJdLFsxLDYsIlxcT21lZ2EgXFx0aW1lcyB1XzAiXSxbMCw2LCJsXzAiXSxbOSwxLCJFIl0sWzMsNCwicCJdLFszLDIsImUgXFx0aW1lcyBwIl0sWzMsMSwiZVxcdGltZXMgYSJdLFs4LDAsInZfMFxcdGltZXMgZSJdLFs5LDAsIkUgXFx0aW1lcyBlIl0sWzEwLDAsIk1cXHRpbWVzIGUiXSxbMTAsMSwiTSJdLFs1LDIsImUiXSxbNywwLCJlIl0sWzUsNCwiXFx2YXJlcHNpbG9uIl0sWzQsNCwiaCJdLFs1LDMsIlxcdmFyZXBzaWxvbiBcXHRpbWVzIGgiXSxbNCwzLCJoIFxcdGltZXMgZSJdLFs0LDEsImEiXSxbMSwwXSxbMSwyXSxbMCwzXSxbMCw0XSxbMyw1XSxbNiwyXSxbNiwzXSxbNiw3XSxbOCwyXSxbOCw5XSxbMTAsM10sWzEsMTFdLFsxMCwxMV0sWzgsMTFdLFsxMCwxMl0sWzEzLDEyXSxbMTMsNV0sWzEzLDE0XSxbMCwxNl0sWzE3LDE2XSxbMTcsMTgsIiIsMSx7InN0eWxlIjp7InRhaWwiOnsibmFtZSI6ImFycm93aGVhZCJ9fX1dLFsxOSw5XSxbMTksMjAsIiIsMCx7InN0eWxlIjp7InRhaWwiOnsibmFtZSI6ImFycm93aGVhZCJ9fX1dLFsyMSwyMCwiIiwyLHsic3R5bGUiOnsidGFpbCI6eyJuYW1lIjoiYXJyb3doZWFkIn19fV0sWzIxLDIyXSxbMjAsMTVdLFsyLDIzXSxbMiwyNF0sWzE5LDI0XSxbMSwyNV0sWzAsMjZdLFsyNywyNV0sWzI3LDIzXSxbMjgsMjcsIiIsMCx7InN0eWxlIjp7InRhaWwiOnsibmFtZSI6ImFycm93aGVhZCJ9fX1dLFsyOCwyNl0sWzI4LDIzXSxbMTcsMjNdLFsxOCwyOV1d
	\[\begin{tikzcd}
		& \omega & {\bold{e \times n}} &&&& {\bold{e}} & e & {v_0\times e} & {E \times e} & {M\times e} \\
		&&& {e\times a} & a &&&& {\nu_0} & E & M \\
		&&& {e \times p} && e \\
		&&&& {h \times e} & {\varepsilon \times h} &&& {\bold{r \times e}} \\
		&&& p & h & \varepsilon \\
		& \Omega & {\bold{n}} & {\bold{h}} &&& {\bold{r \times v}} \\
		{l_0} & {\Omega \times u_0} && i \\
		& {u_0} & {\bold{r \times n}} &&&& {\bold{r}}
		\arrow[from=6-7, to=6-4]
		\arrow[from=6-7, to=1-7]
		\arrow[from=6-4, to=6-3]
		\arrow[from=6-4, to=7-4]
		\arrow[from=6-3, to=6-2]
		\arrow[from=1-3, to=1-7]
		\arrow[from=1-3, to=6-3]
		\arrow[from=1-3, to=1-2]
		\arrow[from=4-9, to=1-7]
		\arrow[from=4-9, to=2-9]
		\arrow[from=8-3, to=6-3]
		\arrow[from=6-7, to=8-7]
		\arrow[from=8-3, to=8-7]
		\arrow[from=4-9, to=8-7]
		\arrow[from=8-3, to=8-2]
		\arrow[from=7-2, to=8-2]
		\arrow[from=7-2, to=6-2]
		\arrow[from=7-2, to=7-1]
		\arrow[from=6-4, to=5-4]
		\arrow[from=3-4, to=5-4]
		\arrow[tail reversed, from=3-4, to=2-4]
		\arrow[from=1-9, to=2-9]
		\arrow[tail reversed, from=1-9, to=1-10]
		\arrow[tail reversed, from=1-11, to=1-10]
		\arrow[from=1-11, to=2-11]
		\arrow[from=1-10, to=2-10]
		\arrow[from=1-7, to=3-6]
		\arrow[from=1-7, to=1-8]
		\arrow[from=1-9, to=1-8]
		\arrow[from=6-7, to=5-6]
		\arrow[from=6-4, to=5-5]
		\arrow[from=4-6, to=5-6]
		\arrow[from=4-6, to=3-6]
		\arrow[tail reversed, from=4-5, to=4-6]
		\arrow[from=4-5, to=5-5]
		\arrow[from=4-5, to=3-6]
		\arrow[from=3-4, to=3-6]
		\arrow[from=2-4, to=2-5]
	\end{tikzcd}\]
	```
	*/
    template<typename scalar, glm::qualifier precision=glm::defaultp>
	struct Properties
	{
		using vec3 = glm::vec<3,scalar,precision>;

		const vec3 vernal_equinox_direction;
		const vec3 north_pole_direction;
		const scalar standard_gravitational_parameter;
		const scalar pi;

		Properties(
			const vec3 vernal_equinox_direction, 
			const vec3 north_pole_direction, 
			const scalar standard_gravitational_parameter,
			const scalar pi
		) noexcept : 
			vernal_equinox_direction(glm::normalize(vernal_equinox_direction)),
			north_pole_direction(glm::normalize(north_pole_direction)),
			standard_gravitational_parameter(standard_gravitational_parameter),
			pi(pi)
		{}
		scalar period_from_semi_major_axis(const scalar semi_major_axis, const scalar combined_mass) const noexcept
		{
			const scalar a = semi_major_axis;
			const scalar mu = standard_gravitational_parameter * combined_mass;
        	return 2*pi*std::sqrt(a*a*a/mu);
		}
		vec3 angular_momentum_vector_from_position_and_velocity(const vec3 position, const vec3 velocity) const noexcept
		{
			return glm::cross(position, velocity);
		}
		vec3 node_vector_from_momentum_vector(const vec3 momentum) const noexcept
		{
			return glm::cross(north_pole_direction, momentum);
		}
		vec3 eccentricity_vector_from_position_and_velocity(
			const vec3 position, const vec3 velocity, const scalar combined_mass
		) const noexcept {
			const scalar mu = standard_gravitational_parameter * combined_mass;
			return (
				(glm::length2(velocity) - mu / glm::length(position)) * position - 
				glm::dot(position, velocity) * velocity
			) / mu;
		}
		// "i"
		scalar inclination_from_momentum_vector(const vec3 momentum) const noexcept
		{
			return std::acos(glm::dot(momentum, north_pole_direction) / glm::length(momentum));
		}
		// "Ω"
		scalar longitude_of_ascending_node_from_node_vector(const vec3 node) const noexcept
		{
			return std::acos(glm::dot(node, vernal_equinox_direction) / glm::length(node));
		}
		// "ω"
		scalar argument_of_periapsis_from_node_and_eccentricity(const vec3 node, const vec3 eccentricity) const noexcept
		{
			return std::acos(glm::dot(node, eccentricity) / (glm::length(node)*glm::length(eccentricity)));
		}
		// "ν₀"
		scalar true_anomaly_from_position_and_eccentricity(const vec3 position, const vec3 eccentricity) const noexcept
		{
			return std::acos(glm::dot(position, eccentricity) / (glm::length(position)*glm::length(eccentricity)));
		}
		// "u₀"
		scalar argument_of_latitude_from_position_and_node(const vec3 position, const vec3 node) const noexcept
		{
			return std::acos(glm::dot(position, node) / (glm::length(position)*glm::length(node)));
		}
		// "ℓ₀"
		scalar true_longitude(const scalar longitude_of_ascending_node, const scalar argument_of_latitude) const noexcept
		{
			return longitude_of_ascending_node + argument_of_latitude;
		}
		// "p"
		scalar semi_latus_rectum_from_momentum_vector(const vec3 momentum_vector, const scalar combined_mass) const noexcept
		{
			const scalar mu = standard_gravitational_parameter * combined_mass;
			return glm::length2(momentum_vector) / mu;
		}
		// "p"
		scalar semi_latus_rectum_from_semi_major_axis_and_eccentricity(const scalar semi_major_axis, const scalar eccentricity) const noexcept
		{
			return semi_major_axis * (1.0 - eccentricity*eccentricity);
		}
		// "a"
		scalar semi_major_axis_from_semi_latus_rectum_and_eccentricity(const scalar semi_latus_rectum, const scalar eccentricity) const noexcept
		{
			return semi_latus_rectum / (1.0 - eccentricity*eccentricity);
		}
		// "E"
		scalar eccentric_anomaly_from_true_anomaly(const scalar true_anomaly, const scalar eccentricity, const int iterations = 10) const noexcept
		{
			return std::acos(-(std::cos(pi - true_anomaly) - eccentricity) / (1.0 - eccentricity * std::cos(pi - true_anomaly)));
		}
		// "ν₀"
		scalar true_anomaly_from_eccentric_anomaly(const scalar eccentric_anomaly, const scalar eccentricity) const noexcept
		{
			return std::acos((std::cos(eccentric_anomaly) - eccentricity) / (1.0 - eccentricity * std::cos(eccentric_anomaly)));
		}
		// "E"
		scalar eccentric_anomaly_from_mean_anomaly(const scalar mean_anomaly, const scalar eccentricity, const int iterations = 10) const noexcept
		{
	        scalar e = eccentricity;
	        scalar E = mean_anomaly;
	        scalar M = mean_anomaly;
	        scalar M_E_(0.0);
	        scalar dMdE(0.0);
	        scalar error(0.0);
	        // solve using Newton's method
	        for (int i = 0; i < iterations; i++) {
	            M_E_   = E-e*std::sin(E);
	            dMdE  = 1.0-e*std::cos(E);
	            error = M - M_E_;
	            E = E + error/dMdE;
	        }
	        return E;
		}
		// "M"
		scalar mean_anomaly_from_eccentric_anomaly(const scalar eccentric_anomaly, const scalar eccentricity) const noexcept
		{
			return eccentric_anomaly - eccentricity * std::sin(eccentric_anomaly);
		}
		
		vec3 perifocal_position(const scalar semi_latus_rectum, const scalar eccentricity, const scalar true_anomaly) const noexcept
		{
			const vec3 P = vernal_equinox_direction;
			const vec3 Q = glm::cross(north_pole_direction,vernal_equinox_direction);
			const scalar radius = semi_latus_rectum / (1.0 + eccentricity*std::cos(true_anomaly));
			return radius * (std::cos(true_anomaly) * P + std::sin(true_anomaly) * Q);
		}
		vec3 perifocal_velocity(
			const scalar semi_latus_rectum, const scalar eccentricity, const scalar true_anomaly, const scalar combined_mass
		) const noexcept {
			const vec3 P = vernal_equinox_direction;
			const vec3 Q = glm::cross(north_pole_direction,vernal_equinox_direction);
			const scalar mu = standard_gravitational_parameter * combined_mass;
			const scalar noneccentric_speed = std::sqrt(mu / semi_latus_rectum);
			return noneccentric_speed * (-std::sin(true_anomaly) * P + (eccentricity + std::cos(true_anomaly)) * Q);
		}
	};

}
