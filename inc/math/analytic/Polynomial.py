import math
import cmath
import collections

scalar_types = {int,float}

# See Polynomial.hpp for description
class Polynomial:
    def __init__(self, *args, weights={}):
        self.k = collections.defaultdict(lambda: 0, 
            {**{i:k for (i,k) in enumerate(args)},
             **weights})
    def __call__(self, x:float):
        return sum([k*x**i for (i,k) in self.k.items()])
    def __str__(self):
        def superscript(n):
            replacements = [
                ('0','⁰'),('1','¹'),('2','²'),('3','³'),('4','⁴'),
                ('5','⁵'),('6','⁶'),('7','⁷'),('8','⁸'),('9','⁹'),
                ('-','⁻'),('.','⋅')]
            text = str(n)
            text[0:min(5,len(text))]
            for replaced, replacement in replacements:
                text = text.replace(replaced, replacement)
            return text
        def exponent(ik):
            return ik[0]
        terms = sorted(self.k.items(), key=exponent)
        return '+'.join([
            {i:f'{k}x{superscript(i)}',
             0:str(k), 1:f'{k}x'}[i] for (i,k) in terms])
    def __repr__(self):
        return str(self)
    def __getitem__(self, i:int):
        return self.k[i]
    def __setitem__(self, i:int, k:float):
        self.k[i] = k
    def exponents(self):
        return set([i for (i,k) in self.k.items() if k!=0])
    def __iadd__(self, other):
        if isinstance(other, Polynomial):
            for (i,k) in other.k.items():
                self.k[i]+=k
        elif type(other) in scalar_types:
            self.k[0]+=other
        else:
            raise ValueError(f'Attempted addition between a polynomial and an unrecognized type, {type(other).__name__}')
        return self
    def __isub__(self, other):
        if isinstance(other, Polynomial):
            for (i,k) in other.k.items():
                self.k[i]-=k
        elif type(other) in scalar_types:
            self.k[0]-=other
        else:
            raise ValueError(f'Attempted subtraction between a polynomial and an unrecognized type, {type(other).__name__}')
        return self
    def __imul__(self, other):
        if isinstance(other, Polynomial):
            raise ValueError('In-place multiplication of polynomials offers no performance benefit and is not supported.')
        elif type(other) in scalar_types:
            for (i,k) in self.k.items():
                self.k[i]*=other
        else:
            raise ValueError(f'Attempted multiplication between a polynomial and an unrecognized type, {type(other).__name__}')
        return self
    def __idiv__(self, other):
        if isinstance(other, Polynomial):
            if len(other.exponents())==1:
                raise ValueError('In-place division between a polynomial and a monomial offers no performance benefit and is not supported.')
            else:
                raise ValueError('In-place division between polynomials is not possible')
        elif type(other) in scalar_types:
            for (i,k) in self.k.items():
                self.k[i]*=other
        else:
            raise ValueError(f'Attempted division between a polynomial and an unrecognized type, {type(other).__name__}')
        return self
    def __add__(self, other):
        y = Polynomial(weights=self.k)
        y += other
        return y
    def __sub__(self, other):
        y = Polynomial(weights=self.k)
        y -= other
        return y
    def __mul__(self, other):
        if type(other) == Polynomial:
            y = Polynomial()
            for (i,ki) in self.k.items():
                for j,kj in other.k.items():
                    y[i+j] += ki*kj
            return y
        else:
            y = Polynomial(weights=self.k)
            y *= other
            return y
    def __div__(self, other):
        if type(other) == Polynomial:
            y = Polynomial(weights=self.k)
            exponents = other.exponents()
            if len(exponents)==1:
                j = exponents[0]
                return Polynomial(weights={i-j:self[i]/other[j] for (i,ki) in enumerate(self.args)})
            else:
                return Rational(self, other)
        else:
            y = Polynomial(weights=self.k)
            y /= other
            return y
    def __radd__(self, other):
        y = Polynomial(weights=self.k)
        y += other
        return y
    def __rsub__(self, other):
        y = Polynomial(weights=self.k)
        y -= other
        return y
    def __rmul__(self, other):
        if type(other) == Polynomial:
            y = Polynomial()
            for (i,ki) in self.k.items():
                for j,kj in other.k.items():
                    y[i+j] += ki*kj
            return y
        else:
            y = Polynomial(weights=self.k)
            y *= other
            return y
    def __rdiv__(self, other):
        if type(other) == Polynomial:
            y = Polynomial(weights=self.k)
            exponents = other.exponents()
            if len(exponents)==1:
                j = exponents[0]
                return Polynomial(weights={i-j:self[i]/other[j] for (i,ki) in enumerate(self.args)})
            else:
                return Rational(self, other)
        else:
            y = Polynomial(weights=self.k)
            y /= other
            return y
    def __ipow__(self):
        raise ValueError('In-place exponentiation of a polynomial offers no performance benefit and is not supported.')
    def __pow__(self, n):
        if type(other) in {int}:
            if other == 0:
                return 1
            elif other > 0:
                return self*(self**(n-1))
            else:
                return Rational(1,self**(n-1))
        else:
            raise ValueError(f'Attempted division between a polynomial and an unrecognized type, {type(other).__name__}')
    def __rpow__(self, n):
        raise ValueError('Exponentiation by a polynomial is not supported.')

def polynomial(f):
    if type(f) in {int, float}:
        return Polynomial(f)
    if type(f) == Polynomial:
        return f
    if type(f) == Identity:
        return Polynomial(0,1)
    if type(f) == Scaling:
        return Polynomial(0,f.factor)
    if type(f) == Shifting:
        return Polynomial(f.offset,1)
    else:
        raise ValueError(f'Attempted cast to a polynomial from an unrecognized type, {type(other).__name__}')

def inverse(y):
    exponents = y.exponents()
    if exponents == {0,1}:
        # we know:           y=a+bx
        # therefore: -a/b+1/by=x
        return Polynomial(-y[0]/y[1], 1/y[1])
    elif len(exponents) == 1:
        # we know:        y=axⁱ
        # therefore:    ⁱ√y/a=x
        i = exponents[0]
        Polynomial(0,y[i]**-i)
    else:
        raise ValueError('An inverse function of a polynomial does not exist in the general case')

''' 
NOTE: we avoid expressing derivatives and integrals as methods,
since not all equations are closed under derivatives and integrals,
and such methods would complicate the order with which classes must be declared
'''
def derivative(p, x=None):
    exponents = p.exponents()
    if exponents == {}:
        return 0
    if exponents == {0}:
        return 0
    if exponents == {1}:
        return p[0]
    else:
        dpdx = Polynomial(
            weights={i-1:i*pi for i,pi in p.k.items() if i*pi!=0})
        return dpdx if x is None else dpdx(x)

'''
`integral` returns a polynomial that represents the indefinite integral of `p`.
'''
def integral(p, x=None, lo=None, hi=None):
    exponents = p.exponents()
    if exponents == {}:
        return 0
    elif x is not None:
        return sum([
            (pi*x**(i+1)/(i+1) if i!=-1 and pi!=0 
             else math.log(abs(x)))
            for i,pi in p.k.items()
        ])
    elif lo is not None and hi is not None:
        return sum([
            (pi*hi**(i+1)/(i+1) - pi*lo**(i+1)/(i+1) if i!=-1 and pi!=0 
             else math.log(abs(hi)) - math.log(abs(lo)))
            for i,pi in p.k.items()
        ])
    else:
        return Polynomial(weights={i+1:pi/(i+1) for i,pi in p.k.items()})

'''
`solutions()` provides all possible solutions to a polynomial, complex or otherwise.
The number of solutions under these conditions is always known at compile time,
so the output is stored either as a T (for linear functions) 
or as an array of complex numbers (for any other polynomial).
This can be useful if performance takes precedence over ease of implementation.
'''
def solution(p:Polynomial, y:float):
    exponents = p.exponents()
    Plo = min(exponents)
    Phi = max(exponents)
    if len(exponents) == 1:
        return [y**-exponents[0]]
    elif Plo==-1:
        # we know that for any laurent polynomial where Plo=-1:
        #  a/x + b + cx + … = y
        #        b + cx + … = y - a/x
        #        bx+ cx²+ … = y - a
        return solution(Polynomial(weights={i+1:p[i] for i in exponents if i>=0}), y-p[-1])
    elif 0 <= Plo and Phi == 1:
        return (y-p[0]) / p[1]
    else:
        raise ValueError('No single solution exists in the general case for polynomials of degree 1')

def solutions(p:Polynomial, y:float):
    exponents = p.exponents()
    Plo = min(exponents)
    Phi = max(exponents)
    if len(exponents) == 1:
        return [y**-exponents[0]]
    elif Plo==-1:
        # we know that for any laurent polynomial where Plo=-1:
        #  a/x + b + cx + … = y
        #        b + cx + … = y - a/x
        #        bx+ cx²+ … = y - a
        return solutions(Polynomial(weights={i+1:p[i] for i in exponents if i>=0}), y-p[-1])
    elif Plo >= 0 and Phi == 1:
        return [(y-p[0]) / p[1]]
    elif Plo >= 0 and Phi == 2:
        # the quadratic formula
        sqrt_argument = p[1]*p[1] - 4.0*(p[0]-y)*p[2]
        return [
            -p[1]+cmath.sqrt(sqrt_argument) / p[2],
            -p[1]-cmath.sqrt(sqrt_argument) / p[2]
        ]
    elif Plo >= 0 and Phi == 3:
        # the cubic formula
        a = p/p[3] # monic cubic polynomial
        q =  a[1]/3.0  - a[2]*a[2]/9.0
        r = (a[1]*a[2] - 3.0*a[0])/6.0 - a[2]*a[2]*a[2]/27.0
        s1 = (r+cmath.sqrt(q*q*q+r*r))**(1.0/3.0)
        s2 = (r-cmath.sqrt(q*q*q+r*r))**(1.0/3.0)
        z1 =  (s1+s2) - a[2]/3.0
        z2 = -(s1+s2)/2.0 - a[2]/3.0 + (s1-s2)*(3.0**(1.0/3.0))/2.0 # *i
        z3 = -(s1+s2)/2.0 - a[2]/3.0 - (s1-s2)*(3.0**(1.0/3.0))/2.0 # *i
        return [z1,z2,z3]
    elif max(p.exponents()) == 4:
        raise NotImplementedError('Solutions for quintic formula are not implemented')
    else:
        raise ValueError('No solution exists in the general case for polynomials with terms of degree less than -1 or greater than 4')

def extremum(p:Polynomial):
    return solution(derivative(p), 0.0)

def extrema(p:Polynomial):
    return solutions(derivative(p), 0.0)

def clamp(x, lo, hi):
    return min(max(x, lo), hi)

'''
`max()` and `min()` returns the highest output of the function over the given range.
These are used together when determining whether to include the function as a term in a larger equation.
'''
def maximum(p:Polynomial, lo:float=None, hi:float=None):
    if 0 <= min(p.exponents) && max(p.exponents) <=1:
        # function is monotonic, so solution must be either lo or hi
        return max([lo,hi],key=p)
    else:
        return max([lo,hi,*[x for x in extremum(p) if x.imag==0]],key=p)

def minimum(p:Polynomial, lo:float=None, hi:float=None):
    if 0 <= min(p.exponents) && max(p.exponents) <=1:
        # function is monotonic, so solution must be either lo or hi
        return min([lo,hi],key=p)
    else:
        return min([lo,hi,*[x for x in extremum(p) if x.imag==0]],key=p)

'''
`distance` is the root of the integrated squared difference 
between two polynomials over a given range divided by the range.
It provides a metric that expresses the extent to which two polynomials differ 
using the same units as the output of the polynomial. 
It is analogous to the standard deviation of a dataset in statistics,
and it satisfies all criteria needed to be considered a metric:
* d(a,b) ∈ ℝ⁺ 
* d(a,b) = 0 ⟺ a=b
* d(a,b) = d(b,a)
* d(a,b) ≤ d(a,c)+d(c,b)
Other functions satisfy these criteria and may also serve as distance metrics
(for instance, maximum of the squared difference, or the mean of the absolute difference),
however those we could find have no closed-form, non-piecewise solution for arbitrary polynomials 
We do not presume that this metric serves more naturally as a distance metric compared to others,
however we address this function as `distance` since the alternative names
are either unwieldy (`root_mean_square_difference`) or nonobvious (`rmsd`).
'''
def distance(p, q, lo:float, hi:float):
    return math.sqrt(integral((p-q)*(p-q), lo=lo, hi=hi)) / (hi-lo)

def compose(p, q):
    degenerations = {Identity,Scaling,Shifting}
    if type(p) in {int, float}:
        return p
    elif type(p) in degenerations or type(q) in degenerations:
        return compose(polynomial(p), polynomial(q))
    elif type(p) == Polynomial and type(q) == Polynomial:
        if type(q) in {int, float}:
            return p(q)
        exponents = p.exponents()
        if len(exponents) < 1:
            return 0
        if exponents == {0}:
            return p[0]
        if min(exponents) < 0:
            raise NotImplementedError('Composition with a polynomial of negative degree is not supported.')
        '''
        we rely on the fact that, e.g.
            p(x) = k₀x⁰+k₁x¹+k₂x²+k₃x³ = k₀+(k₁+(k₂+k₃x)x)x
        so there is a p₀(u) and pₙ₋₁(u) such that 
            p(x) = p₀∘pₙ₋₁.
        and it turns out that, in this case:
            p₀(u) = k₀+xu 
            pₙ₋₁(u) = k₁+(k₂+k₃u)u = k₁u⁰+k₂u¹+k₃u²
        '''
        uq = compose(Polynomial(weights={i-1:ki for (i,ki) in p.k.items() if i!=0}), q)
        return p[0]+q*compose(Polynomial(weights={i-1:ki for (i,ki) in p.k.items() if i!=0}), q)
    else:
        raise ValueError(f'Attempted division between a polynomial and an unrecognized type, {type(other).__name__}')


def linear_newton_polynomial(
        x1:float,      x2:float, 
        y1:float=None, y2:float=None, f=None):
    dfdx = (y2-y1) / (x2-x1)
    return (y1 + dfdx * Shifting(-x1)) if f is None else linear_newton_polynomial(x1,x2, f(x1),f(x2))

def quadratic_newton_polynomial(
        x1:float,      x2:float,      x3:float,      
        y1:float=None, y2:float=None, y3:float=None, f=None):
    if f is None:
        dydx_12  = (y2-y1) / (x2-x1)
        dydx_23  = (y3-y2) / (x3-x2)
        dy2dx2 = (dydx_23-dydx_12) / (x3-x1)
        return (y1 
            + dydx_12 * Shifting(-x1)
            + dy2dx2  * Shifting(-x1) * Shifting(-x2))
    else:
        return quadratic_newton_polynomial(x1,x2,x3, f(x1),f(x2),f(x3))

def cubic_newton_polynomial(
        x1:float,      x2:float,      x3:float,      x4:float,      
        y1:float=None, y2:float=None, y3:float=None, y4:float=None, f=None):
    if f is None:
        dydx_12 = (y2-y1) / (x2-x1)
        dydx_23 = (y3-y2) / (x3-x2)
        dydx_34 = (y4-y3) / (x4-x3)
        dy2dx2_13 = (dydx_23-dydx_12) / (x3-x1)
        dy2dx2_24 = (dydx_34-dydx_23) / (x4-x2)
        dy3dx3    = (dy2dx2_24-dy2dx2_13) / (x4-x1)
        return (y1 
            + dydx_12   * Shifting(-x1)
            + dy2dx2_13 * Shifting(-x1) * Shifting(-x2)
            + dy3dx3    * Shifting(-x1) * Shifting(-x2) * Shifting(-x3))
    else:
        return cubic_newton_polynomial(x1,x2,x3,x4, f(x1),f(x2),f(x3),f(x4))

def linear_taylor_series(f, x:float, dx:float):
    dx2 = dx*dx
    return compose(
        Polynomial(f(x), central_finite_difference(f, x, dx, 1) / dx),
        Shifting(-x)
    )

def quadratic_taylor_series(f, x:float, dx:float):
    dx2 = dx*dx
    return compose(
        Polynomial(f(x), central_finite_difference(f, x, dx, 1) / dx), 
        Shifting(-x)
    )

def cubic_taylor_series(f, x:float, dx:float):
    dx2 = dx*dx
    dx3 = dx2*dx
    return compose(
        Polynomial(f(x), 
            central_finite_difference(f, x, dx, 1) / dx, 
            central_finite_difference(f, x, dx, 2) /(dx2*2.0), 
            central_finite_difference(f, x, dx, 3) /(dx3*6.0)),
        Shifting(-x)
    )

''' 
NOTE: The following is an alternate implementation to "cubic_spline()".
It requires no familiarity with linear algebra, it places no dependency on the glm library,
and it provides a good way to verify other spline functions, 
however its implementation is more complex and cannot be easily adapted to higher order splines.
'''
def cubic_algebraic_spline(
        xa:float, # lower bound of spline
        xb:float, # upper bound of spline
        ya:float, # value of y at xb
        yb:float, # value of y at xb
        ya_ddx:float, # derivative of y at xa
        yb_ddx:float  # derivative of y at xb
    ):
    '''
    First, we construct a new coordinate system based around (xa,ya) as the origin. 
    In this coordinate system, the coordinates (xb,yb) are denoted (X,Y).
    '''
    Y = yb-ya
    X = xb-xa
    '''
    in the new coordinate system, we know that xa=0 and ya=0, 
    so we can adopt a simpler system of equations:
       Y =  aX² +  cX³
      yb_ddx = 2aX  + 3cX²
    we divide Y and yb_ddx by X² and X respectively to get:
       Y =  a +  cX
      yb_ddx = 2a + 3cX
    '''
    u = (Y-ya_ddx)/(X*X)
    v = (yb_ddx-ya_ddx)/X
    '''
    We then solve this system of equations, which results in the following code:
    '''
    x = Identity()
    return ya + compose(
            ya_ddx*x + (T(3)*u-v)*x**2 + ((v-T(2)*u)/X)*x**3,
            x-xa
        )
