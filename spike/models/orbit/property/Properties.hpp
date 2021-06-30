#pragma once

#include <cmath>

#define GLM_FORCE_PURE      // disable anonymous structs so we can build with ISO C++
#define GLM_ENABLE_EXPERIMENTAL // length2
#include <glm/vec3.hpp>     // *vec3
#include <glm/gtx/norm.hpp> // length2

#include <glm/mat3x3.hpp>

/*
`Properties` is a mathematical small category implemented as a class for which methods are conceptually pure functions.
Arrows are pure functions that map orbital properties of a single orbit for which the gravitational parameter is known.
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
class Properties
{
	glm::dvec3 vernal_equinox_direction;
	glm::dvec3 north_pole_direction;
	double gravitational_parameter;
public:
	Properties(
		const glm::dvec3 vernal_equinox_direction, 
		const glm::dvec3 north_pole_direction, 
		const double gravitational_parameter
	) : 
		vernal_equinox_direction(vernal_equinox_direction),
		north_pole_direction(north_pole_direction),
		gravitational_parameter(gravitational_parameter)
	{}
	glm::dvec3 get_angular_momentum_vector_from_position_and_velocity(const glm::dvec3 position, const glm::dvec3 velocity)
	{
		return glm::cross(position, velocity);
	}
	glm::dvec3 get_node_vector_from_momentum_vector(const glm::dvec3 momentum)
	{
		return glm::cross(north_pole_direction, momentum);
	}
	glm::dvec3 get_eccentricity_vector_from_position_and_velocity(const glm::dvec3 position, const glm::dvec3 velocity)
	{
		return (
			(glm::length2(velocity) - gravitational_parameter / glm::length(position)) * position - 
			glm::dot(position, velocity) * velocity
		) / gravitational_parameter;
	}
	// "i"
	double get_inclination_from_momentum_vector(const glm::dvec3 momentum)
	{
		return std::acos(glm::dot(momentum, north_pole_direction) / glm::length(momentum));
	}
	// "Ω"
	double get_longitude_of_ascending_node_from_node_vector(const glm::dvec3 node)
	{
		return std::acos(glm::dot(node, vernal_equinox_direction) / glm::length(node));
	}
	// "ω"
	double get_argument_of_periapsis_from_node_and_eccentricity(const glm::dvec3 node, const glm::dvec3 eccentricity)
	{
		return std::acos(glm::dot(node, eccentricity) / (glm::length(node)*glm::length(eccentricity)));
	}
	// "ν₀"
	double get_true_anomaly_from_position_and_eccentricity(const glm::dvec3 position, const glm::dvec3 eccentricity)
	{
		return std::acos(glm::dot(position, eccentricity) / (glm::length(position)*glm::length(eccentricity)));
	}
	// "u₀"
	double get_argument_of_latitude_from_position_and_node(const glm::dvec3 position, const glm::dvec3 node)
	{
		return std::acos(glm::dot(position, node) / (glm::length(position)*glm::length(node)));
	}
	// "ℓ₀"
	double get_true_longitude(const double longitude_of_ascending_node, const double argument_of_latitude)
	{
		return longitude_of_ascending_node + argument_of_latitude;
	}
	// "p"
	double get_semi_latus_rectum_from_momentum_vector(const double momentum_vector)
	{
		return glm::length2(momentum_vector) / gravitational_parameter;
	}
	// "p"
	double get_semi_latus_rectum_from_semi_major_axis_and_eccentricity(const double semi_major_axis, const double eccentricity)
	{
		return semi_major_axis * (1.0 - eccentricity*eccentricity);
	}
	// "a"
	double get_semi_major_axis_from_semi_latus_rectum_and_eccentricity(const double semi_latus_rectum, const double eccentricity)
	{
		return semi_latus_rectum / (1.0 - eccentricity*eccentricity);
	}
	// "E"
	double solve_eccentric_anomaly_from_true_anomaly(const double true_anomaly, const double eccentricity, const int iterations = 10)
	{
        double e = eccentricity;
        double E = true_anomaly;
        double nu = true_anomaly;
        double nu_E(0.0);
        double dnudE(0.0);
        double error(0.0);
        double denominator(0.0);
        double numerator(0.0);
        double quotient(0.0);
        for (int i = 0; i < iterations; i++) {
            numerator = std::cos(E) - e;
            denominator = 1.0 - e*std::cos(E);
            quotient = numerator / denominator;
            nu_E   = std::acos(quotient);
            dnudE  = -(numerator*(e*std::sin(E)) - denominator*(-std::sin(E))) / 
                      (denominator * denominator * std::sqrt(1.0 - quotient*quotient));
            error = nu - nu_E;
            E = E + error/dnudE;
        }
        return E;
	}
	// "ν₀"
	double get_true_anomaly_from_eccentric_anomaly(const double eccentric_anomaly, const double eccentricity)
	{
		return std::acos((std::cos(eccentric_anomaly) - eccentricity) / (1.0 - eccentricity * std::cos(eccentric_anomaly)));
	}
	// "E"
	double solve_eccentric_anomaly_from_mean_anomaly(const double mean_anomaly, const double eccentricity, const int iterations = 10)
	{
        double e = eccentricity;
        double E = mean_anomaly;
        double M = mean_anomaly;
        double M_E_(0.0);
        double dMdE(0.0);
        double error(0.0);
        for (int i = 0; i < iterations; i++) {
            M_E_   = E-e*std::sin(E);
            dMdE  = 1.0-e*std::cos(E);
            error = M - M_E_;
            E = E + error/dMdE;
        }
        return E;
	}
	// "M"
	double get_mean_anomaly_from_eccentric_anomaly(const double eccentric_anomaly, const double eccentricity)
	{
		return eccentric_anomaly - eccentricity * std::sin(eccentric_anomaly);
	}
	
	glm::dvec3 get_perifocal_position(const double semi_latus_rectum, const double eccentricity, const double true_anomaly)
	{
		const glm::dvec3 P = vernal_equinox_direction;
		const glm::dvec3 Q = glm::cross(north_pole_direction,vernal_equinox_direction);
		const double radius = semi_latus_rectum / (1.0 + eccentricity*std::cos(true_anomaly));
		return radius * (std::cos(true_anomaly) * P + std::sin(true_anomaly) * Q);
	}
	glm::dvec3 get_perifocal_velocity(const double semi_latus_rectum, const double eccentricity, const double true_anomaly)
	{
		const glm::dvec3 P = vernal_equinox_direction;
		const glm::dvec3 Q = glm::cross(north_pole_direction,vernal_equinox_direction);
		return std::sqrt(gravitational_parameter / semi_latus_rectum) * (-std::sin(true_anomaly) * P + (eccentricity + std::cos(true_anomaly)) * Q);
	}
};


