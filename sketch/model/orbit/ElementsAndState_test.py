import math

import pytest
from pyglm import glm

from State import State
from Elements import Elements
from Properties import Properties
from ElementsAndState import ElementsAndState

# from orbit import Elements, State, Properties, ElementsAndState
# from si import solar_radius, astronomical_unit, gravitational_constant, solar_mass, meter, meter3, second2

class si:
    solar_radius = 6.957e8          # meters
    astronomical_unit = 1.496e11    # meters
    gravitational_constant = 6.67430e-11
    solar_mass = 1.9885e30
    meter = 1.0
    meter3 = 1.0
    second2 = 1.0

# Utility mix function
def mix(a, b, t):
    """Linear interpolation between a and b."""
    return a * (1 - t) + b * t

# Helper approximate comparison functions
def elements_equal(e1, e2, tol=0.0001):
    assert math.isclose(e1.semi_major_axis, e2.semi_major_axis, rel_tol=tol)
    assert math.isclose(e1.eccentricity, e2.eccentricity, rel_tol=tol)
    assert math.isclose(e1.inclination, e2.inclination, rel_tol=tol)
    assert math.isclose(e1.longitude_of_ascending_node, e2.longitude_of_ascending_node, rel_tol=tol)
    assert math.isclose(e1.argument_of_periapsis, e2.argument_of_periapsis, rel_tol=tol)
    assert math.isclose(e1.mean_anomaly, e2.mean_anomaly, rel_tol=tol)

def state_equal(s1, s2, tol=1e-4):
    assert glm.length(s1.position - s2.position) / max(glm.length(s1.position), glm.length(s2.position)) < tol
    assert glm.length(s1.velocity - s2.velocity) / max(glm.length(s1.velocity), glm.length(s2.velocity)) < tol

# get_elements_from_state()/get_state_from_elements() invertibility
def test_get_elements_from_state_invertibility():
    properties = Properties(
        glm.vec3(1, 0, 0),
        glm.vec3(0, 0, 1),
        si.gravitational_constant
    )
    conversion = ElementsAndState(properties)

    m = si.solar_mass / (si.meter3 / si.second2)
    max_samples = 3.0
    pi = math.pi

    for ai in range(int(max_samples)):
        for ei in range(int(max_samples)):
            for ii in range(int(max_samples)):
                for Oi in range(int(max_samples)):
                    for wi in range(int(max_samples)):
                        for Mi in range(int(max_samples)):
                            a = mix(si.solar_radius / si.meter, si.astronomical_unit / si.meter, ai / max_samples)
                            e = mix(0.1, 1.2, ei / max_samples)
                            i = mix(0.1, 0.9 * pi, ii / max_samples)
                            O = mix(0.1, 0.9 * pi, Oi / max_samples)
                            w = mix(0.1, 0.9 * pi, wi / max_samples)
                            M = mix(0.1, 0.9 * pi, Mi / max_samples)

                            elements = Elements(a, e, i, O, w, M)
                            reproduced = conversion.get_elements_from_state(
                                conversion.get_state_from_elements(elements, m),
                                m
                            )
                            elements_equal(reproduced, elements)

# get_state_from_elements() inclination congruence
def test_get_state_from_elements_inclination_congruence():
    properties = Properties(
        glm.vec3(1, 0, 0),
        glm.vec3(0, 0, 1),
        si.gravitational_constant
    )
    conversion = ElementsAndState(properties)

    m = si.solar_mass / (si.meter3 / si.second2)
    max_samples = 3.0
    pi = math.pi

    for ai in range(int(max_samples)):
        for ei in range(int(max_samples)):
            for ii in range(int(max_samples)):
                for Oi in range(int(max_samples)):
                    for wi in range(int(max_samples)):
                        for Mi in range(int(max_samples)):
                            a = mix(si.solar_radius / si.meter, si.astronomical_unit / si.meter, ai / max_samples)
                            e = mix(0.1, 1.2, ei / max_samples)
                            i = mix(0.1, 0.9 * pi, ii / max_samples)
                            O = mix(0.1, 0.9 * pi, Oi / max_samples)
                            w = mix(0.1, 0.9 * pi, wi / max_samples)
                            M = mix(0.1, 0.9 * pi, Mi / max_samples)

                            baseline = conversion.get_state_from_elements(Elements(a, e, i, O, w, M), m)

                            state_equal(baseline, conversion.get_state_from_elements(Elements(a, e, i + 2 * pi, O, w, M), m))
                            state_equal(baseline, conversion.get_state_from_elements(Elements(a, e, i, O + 2 * pi, w, M), m))
                            state_equal(baseline, conversion.get_state_from_elements(Elements(a, e, i, O, w + 2 * pi, M), m))
                            state_equal(baseline, conversion.get_state_from_elements(Elements(a, e, i, O, w, M + 2 * pi), m))
