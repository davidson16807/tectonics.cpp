
import math

from pyglm import glm

from State import State, sape, state_distance
from Properties import Properties
from ElementsAndState import ElementsAndState
from Elements import Elements
from Universals import Universals
from UniversalPropagator import UniversalPropagator

G = 6.6743015e-11 # m³kg⁻¹s⁻²
mass_of_didymos_dimorphos = 5.4e11 # kg
mass_of_earth_moon = 6.0457e24 # kg
mass_of_jupiter = 1.898e27 # kg
mass_of_saturn = 5.683e26 # kg
mass_of_uranus = 8.681e25 # kg
mass_of_neptune = 1.024e26 # kg
mass_of_pluto_charon = 1.3e22 + 1.5e21 # kg
mass_of_sun = 1.32712440018e20/G # kg
mass_of_galaxy = 1.262e41 # kg, back calculated to achieve period of 250 million years
au = 1.5e11

propagator = UniversalPropagator(G, max_refinement_count=100, laguerre_method_n = 5)
properties = Properties(glm.vec3(1,0,0), glm.vec3(0,0,1), G)
converter = ElementsAndState(properties)

pi = 3.141592653589793238462643383279
degrees = 2*pi/360

escape_velocities = [
	# parent mass (kg),   radius (m),    escape velocity (m/s)
	# (mass_of_earth_moon,  12756.2/2,     11.186e3), 
	# (mass_of_saturn,      120536/2,      36.09e3),
	# (mass_of_jupiter,     142984/2,      60.20e3),
	# (mass_of_sun,         432300/2,      617.5e3),
	# (mass_of_galaxy,      24000*9.4e15,  594e3),
]

elliptic_periapses = [
	# TODO: add didymos/dimorphos
	(mass_of_didymos_dimorphos, Elements(1.144e3, 0.0247, 169.3*degrees, 0.0, 0.0, 0.0)), # Didymos, post-impact
	(mass_of_earth_moon, Elements(384e6,  0.0549, 0.0*degrees, 0.0, 0.0, 0.0)), # Moon
	(mass_of_saturn, Elements(186e6,    0.0196, 1.53*degrees, 0.0, 0.0, 0.0)), # Mimas
	(mass_of_saturn, Elements(238.4e6,  0.0047, 0.02*degrees, 0.0, 0.0, 0.0)), # Enceladus
	(mass_of_saturn, Elements(295e6,    0.0001, 1.09*degrees, 0.0, 0.0, 0.0)), # Tethys
	(mass_of_saturn, Elements(377.7e6,  0.0022, 0.02*degrees, 0.0, 0.0, 0.0)), # Dione
	(mass_of_saturn, Elements(527.2e6,  0.001,  0.35*degrees, 0.0, 0.0, 0.0)), # Rhea
	(mass_of_saturn, Elements(1221.9e6, 0.0288, 0.33*degrees, 0.0, 0.0, 0.0)), # Titan
	(mass_of_saturn, Elements(1481.5e6, 0.0274, 0.43*degrees, 0.0, 0.0, 0.0)), # Hyperion
	(mass_of_saturn, Elements(3561.7e6, 0.0283, 157*degrees,  0.0, 0.0, 0.0)), # Iapetus
	(mass_of_jupiter, Elements(421.8e6,   0.0041, 0.050*degrees, 0.0, 0.0, 0.0)), # Io
	(mass_of_jupiter, Elements(671.1e6,   0.0090, 0.470*degrees, 0.0, 0.0, 0.0)), # Europa
	(mass_of_jupiter, Elements(1070.4e6,  0.0013, 0.200*degrees, 0.0, 0.0, 0.0)), # Ganymede
	(mass_of_jupiter, Elements(1882.7e6,  0.0074, 0.192*degrees, 0.0, 0.0, 0.0)), # Callisto
    (mass_of_sun, Elements(5.790905e10,   0.2056, 7.005*degrees, 0.0, 0.0, 0.0)), # Mercury
    (mass_of_sun, Elements(1.082080e11,   0.0068, 3.3947*degrees, 0.0, 0.0, 0.0)), # Venus
    (mass_of_sun, Elements(1.49598023e11, 0.0167, 0.000*degrees, 0.0, 0.0, 0.0)), # Earth
    (mass_of_sun, Elements(2.279392e11,   0.0934, 1.851*degrees, 0.0, 0.0, 0.0)), # Mars
    (mass_of_sun, Elements(7.78570e11,    0.0489, 1.305*degrees, 0.0, 0.0, 0.0)), # Jupiter
    (mass_of_sun, Elements(1.43353e12,    0.0565, 2.484*degrees, 0.0, 0.0, 0.0)), # Saturn
    (mass_of_sun, Elements(2.87246e12,    0.046,  0.770*degrees, 0.0, 0.0, 0.0)), # Uranus
    (mass_of_sun, Elements(4.49506e12,    0.009,  1.769*degrees, 0.0, 0.0, 0.0)), # Neptune
    (mass_of_sun, Elements(39.482*au,     0.2488, 17.14*degrees, 0.0, 0.0, 0.0)), # Pluto
    # mass_of_galaxy, Elements(), # sun around galaxy
]

all_periapses = [
	*[(mass, Universals.from_state(mass, converter.get_state_from_elements(elements, mass)))
		for mass, elements in elliptic_periapses], # elliptics
	*[(mass, Universals(mass, 0, glm.vec3(radius,0,0), glm.vec3(0,velocity,0)))
		for mass, radius, velocity in escape_velocities], # parabolics
	*[(mass, Universals(mass, 0, glm.vec3(radius,0,0), glm.vec3(0,velocity*1.5,0)))
		for mass, radius, velocity in escape_velocities], # hyperbolics
]

def test_nth_period_congruence_modulo_n(): 
	# also checks Stepanov's "circular orbit" property, determinism, and an identity
	n = 10 # samples of orbit
	for k in range(n): # starting position
		for (mass, elements) in elliptic_periapses:
			elements = elements.advance(2*pi*k/n)
			for i in range(n+1):
				T = properties.period_from_semi_major_axis(elements.semi_major_axis, mass)
				universals = Universals.from_state(mass, converter.get_state_from_elements(elements, mass))
				for j in range(0,3+1):
					assert state_distance(
						propagator.state(universals,T*(i/n)),
						propagator.state(universals,T*(i/n+j)), 
					) < 3e-5

def test_apsis_extrema(): 
	# for universals in all_periapses:
	# 	e = 1e2
	# 	for sign in [1,-1]:
	# 		# periapsis position
	# 		assert (
	# 			glm.length(propagator.state(universals,0).position) <
	# 			glm.length(propagator.state(universals,e).position)
	# 		)
	# 		# periapsis velocity
	# 		assert (
	# 			glm.length(propagator.state(universals,0).velocity) >
	# 			glm.length(propagator.state(universals,e).velocity)
	# 		)
	for (mass, elements) in elliptic_periapses:
		T = properties.period_from_semi_major_axis(elements.semi_major_axis, mass)
		universals = Universals.from_state(mass, converter.get_state_from_elements(elements, mass))
		e = 0.03
		for sign in [1,-1]:
			# apoapsis position
			assert (
				glm.length(propagator.state(universals,(T/2)).position) >=
				glm.length(propagator.state(universals,(T/2)*(1+sign*e)).position)
			)
			# apoapsis velocity
			assert (
				glm.length(propagator.state(universals,(T/2)).velocity) <=
				glm.length(propagator.state(universals,(T/2)*(1+sign*e)).velocity)
			)
			# periapsis position
			assert (
				glm.length(propagator.state(universals,0).position) <=
				glm.length(propagator.state(universals,sign*T*e).position)
			)
			# periapsis velocity
			assert (
				glm.length(propagator.state(universals,0).velocity) >=
				glm.length(propagator.state(universals,sign*T*e).velocity)
			)
		elements.mean_anomaly = pi # apoapsis
		universals = Universals.from_state(mass, converter.get_state_from_elements(elements, mass))
		e = 0.03
		for sign in [1,-1]:
			# periapsis position
			assert (
				glm.length(propagator.state(universals,(T/2)).position) <=
				glm.length(propagator.state(universals,(T/2)*(1+sign*e)).position)
			)
			# periapsis velocity
			assert (
				glm.length(propagator.state(universals,(T/2)).velocity) >=
				glm.length(propagator.state(universals,(T/2)*(1+sign*e)).velocity)
			)
			# apoapsis position
			assert (
				glm.length(propagator.state(universals,0).position) >=
				glm.length(propagator.state(universals,sign*T*e).position)
			)
			# apoapsis velocity
			assert (
				glm.length(propagator.state(universals,0).velocity) <=
				glm.length(propagator.state(universals,sign*T*e).velocity)
			)

def test_circular_orbit_radius_conservation():
	for (mass, elements) in elliptic_periapses:
		elements = elements.copy()
		elements.eccentricity = 0 # circular orbits only
		T = properties.period_from_semi_major_axis(elements.semi_major_axis, mass)
		universals = Universals.from_state(mass, converter.get_state_from_elements(elements, mass))
		n = 10 # orbit samples
		for i in range(n+1):
			assert abs(glm.length(propagator.state(universals,T*i/n).position) / glm.length(universals.initial_position) - 1) < 1e-6
			assert abs(glm.length(propagator.state(universals,T*i/n).velocity) / glm.length(universals.initial_velocity) - 1) < 1e-6

# def test_specific_orbital_energy_conservation(): 
# 	for m, elements in elliptic_periapses:
# 		for e in [1, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7]:
# 			for sign in [1,-1]:
# 				assert abs(
# 						properties.specific_orbial_energy(
# 							converter.get_elements_from_state(
# 								propagator.state(
# 									Universals.from_state(m, 
# 										converter.get_state_from_elements(elements, m)), 
# 										e), 
# 								m).semi_major_axis, m
# 						)
# 					) < 1e-6

def test_continuity(): 
	for m, o in all_periapses:
		for e in [1, 1e1, 1e2, 1e3]:
			for sign in [1,-1]:
				# periapsis position
				assert (
					state_distance(propagator.state(o,0), propagator.state(o,sign*e)) <
					state_distance(propagator.state(o,0), propagator.state(o,sign*e*2))
				)

def test_angular_momentum_conservation(): 
	for m, o in all_periapses:
		for e in [1, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7]:
			for sign in [1,-1]:
				# periapsis position
				assert sape(o.angular_momentum_vector(), 
					Universals.from_state(m, propagator.state(o,e)).angular_momentum_vector()) < 1e-6

def test_group(): 
	t = 1e3
	for m, a in all_periapses:
		assert state_distance(
			State.from_universals(a), 
			propagator.state(a,0)
			# propagator.state(Universals.from_state(m, propagator.state(a,t)),-t)
		) < 1e-10
		for t1 in [1e2, 1e3, 1e4]:
			assert state_distance(
				State.from_universals(a), 
				propagator.state(Universals.from_state(m, propagator.state(a,t)),-t)
			) < 1e-6
			for t2 in [1e2, 1e3, 1e4]:
				assert state_distance(
					propagator.state(Universals.from_state(m, propagator.state(a,t2)),t1),
					propagator.state(Universals.from_state(m, propagator.state(a,t1)),t2)
				) < 1e-6
				assert state_distance(
					propagator.state(a,t1+t2),
					propagator.state(Universals.from_state(m, propagator.state(a,t1)),t2)
				) < 1e-6

'''
for any orbit:

	group:
x		advance(o,0) = o
x		advance(advance(o,a),b) = advance(advance(o,b),a) 
x		advance(advance(o,a),-a) = o
	commutativity:
x		advance(advance(o,a),b) = advance(o,a+b)

	continuity:
x		d(state(o,ε), state(o,0)) < d(state(o,2ε), state(o,0))

	tare idempotence:
		tare(tare(o)) = tare(o)

	energy is conserved:
x		energy(advance(o,t)) == energy(o)
	angular momentum is conserved
x		angular momentum(advance(o,t)) == angular momentum(o)

x	t=0 minimizes o↦glm.length(state(periapsis,t).position)
x	t=0 maximizes o↦glm.length(state(periapsis,t).velocity)

for known elliptic orbits:
x	congruence:
		state(o,n*T) = o for any n∈ℕ
x	orbit/involute/identity: 
		advance(advance(advance(o, T/n), T/n), T/n) = o etc.
x	t=T/2 maximizes o↦glm.length(state(periapsis,t).position)
x	t=T/2 minimizes o↦glm.length(state(periapsis,t).velocity)
x	t=0 maximizes o↦glm.length(state(apoapsis,t).position)
x	t=0 minimizes o↦glm.length(state(apoapsis,t).velocity)
x	t=T/2 minimizes o↦glm.length(state(apoapsis,t).position)
x	t=T/2 maximizes o↦glm.length(state(apoapsis,t).velocity)

for circular orbits:
x	glm.length(advance(o,t).position) is constant for any t

given: 
	o 	orbit
	T 	period
	d 	distance()

'''

