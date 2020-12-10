from math import *
import random
import numpy as np

from optimize import OptimizationBasis, genetic_algorithm


def seq(start, end, step_count = None, step_size = None):
    assert step_count or step_size, "either step_count or step_size must be defined"
    step_size = step_size or (end-start) / step_count
    step_count = step_count or int(abs(end - start) / step_size)
    for i in range(0,step_count):
        yield start + i*step_size
def integrate(f, start, end, step_count = None, step_size = None):
    step_size = step_size or (end-start) / step_count
    step_count = step_count or int(abs(end - start) / step_size)
    return sum([f(x) for x in seq(start, end, step_count, step_size)]) * step_size

def rho(x,y,r): return exp( sqrt(x*x + y*y) - r )
def I(x,y,r):   return integrate(lambda xi: rho(xi,y,r), 0, x, step_count=100)
def F(x,y,r):   return rho(x,y,r)/I(x,y,r) - x/sqrt(x**2+y**2) - 1/x
def x0(x,r):    return sqrt(max(r*r - x*x,0))

SAMPLE_COUNT = 30
R = 10
CACHE = [
    ((x, y), F(x,y,R)) 
    for x in seq(-R,R,SAMPLE_COUNT)
    for y in seq(-x0(x,R), x0(x,R), SAMPLE_COUNT)
    if x != 0
]


numerator_basis = [
    '1', 
    'x', 
    #'y', 
    #'x**2', 
    #'x*y', 
    'y**2', 
    'x**3', 
    #'x**2*y',
    'x*y**2',
    #'y**3',
    #'x**4',
    #'x**3*y',
    #'x**2*y**2',
    #'x*y**3',
    #'y**4',
    #'sqrt(x*x+y*y)',
    #'sqrt(x)',
    #'sqrt(y)',
]
denominator_basis = [
    '1', 
    'x', 
    # 'y', 
    'x**2', 
    #'x*y', 
    'y**2', 
    'x**3', 
    #'x**2*y',
    'x*y**2',
    #'y**3',
    #'x**4',
    #'x**3*y',
    #'x**2*y**2',
    #'x*y**3',
    #'y**4',
    #'sqrt(x*x+y*y)',
    #'sqrt(x)',
    #'sqrt(y)',
]
def F2(input,params):
    (x,y) = input
    numerator_basis = [
        1, 
        x, 
        #y, 
        #x**2, 
        #x*y, 
        y**2, 
        x**3, 
        #x**2*y,
        x*y**2,
        #y**3,
        #x**4,
        #x**3*y,
        #x**2*y**2,
        #x*y**3,
        #y**4,
        #sqrt(x*x+y*y),
        #sqrt(x),
        #sqrt(y),
    ]
    denominator_basis = [
        1, 
        x, 
        # y, 
        x**2, 
        #x*y, 
        y**2, 
        x**3, 
        #x**2*y,
        x*y**2,
        #y**3,
        #x**4,
        #x**3*y,
        #x**2*y**2,
        #x*y**3,
        #y**4,
        #sqrt(x*x+y*y),
        #sqrt(x),
        #sqrt(y),
    ]
    a = params[0:len(numerator_basis)]
    b = params[len(numerator_basis):(len(numerator_basis)+len(denominator_basis))]
    return sum([a[i]*numerator_basis[i] for i in range(len(numerator_basis))]) /      \
           sum([exp(b[i])*denominator_basis[i] for i in range(len(denominator_basis))])


def code(params):
    a = params[0:len(numerator_basis)]
    b = params[len(numerator_basis):(len(numerator_basis)+len(denominator_basis))]
    numerator = '+'.join(f'{a[i]:.3f}*{numerator_basis[i]}' for i in range(len(a)))
    denominator = '+'.join(f'{exp(b[i]):.3f}*{denominator_basis[i]}' for i in range(len(b)))
    return f'({numerator}) / ({denominator})'
def pretty_basis(basis):
    return basis.replace('**3','³').replace('**2','²').replace('*','').replace('sqrt(','√').replace(')','')
def pretty(params):
    a = params[0:len(numerator_basis)]
    b = params[len(numerator_basis):(len(numerator_basis)+len(denominator_basis))]
    numerator = ' '.join(f'{a[i]:+.2f}{pretty_basis(numerator_basis[i])}' for i in range(len(a)))
    denominator = ' '.join(f'{exp(b[i]):+.2f}{pretty_basis(denominator_basis[i])}' for i in range(len(b)))
    return \
f"""({numerator}) 
{'-'*max(len(numerator),len(denominator))}
({denominator})"""


basis = OptimizationBasis(F2, pretty)


solutions = [np.array([random.gauss(0,1) for j in range(len(numerator_basis)+len(denominator_basis))]) for i in range(2000)]
solutions = genetic_algorithm(basis, CACHE, solutions)
print(code(solutions[0]))
