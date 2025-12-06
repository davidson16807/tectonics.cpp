import sympy as sp

# symbols
x, a, r0, sigma0, dtsqrtm, s0, s1 = sp.symbols(
    'x a r0 sigma0 dtsqrtmu s0 s1', real=True
)

Y = sp.symbols('Y', real=True)

c = sp.Function('c') # c(x)=C(y(x))=C(ax²), used to structure the implementation
s = sp.Function('s') # s(x)=S(y(x))=S(ax²), used to structure the implementation
y = sp.Function('y') # y(x)=Y=ax², used to structure the implementation

# C(Y)
C = sp.Piecewise(
    ((s1 - sp.cos(sp.sqrt(sp.Abs(Y)))) / sp.Abs(Y), Y > 0),
    ((sp.cosh(sp.sqrt(sp.Abs(Y))) - s1) / sp.Abs(Y), True)
)

# S(Y)
S = sp.Piecewise(
    ((sp.sqrt(sp.Abs(Y)) - sp.sin(sp.sqrt(sp.Abs(Y)))) /
        (sp.sqrt(sp.Abs(Y))**3), Y > 0),
    ((sp.sinh(sp.sqrt(sp.Abs(Y))) - sp.sqrt(sp.Abs(Y))) /
        (sp.sqrt(sp.Abs(Y))**3), True)
)

# ax2, C(ax2), S(ax2)
x2   = x * x
x3   = x * x2
ax2  = a * x2
one_r0a = s1 - r0 * a

Cy = C.subs(Y, y(x))
Sy = S.subs(Y, y(x))

Cax2 = C.subs(Y, ax2)
Sax2 = S.subs(Y, ax2)

F = (
    sigma0 * x2 * c(x) + 
    one_r0a * x3 * s(x) + 
    r0 * x - dtsqrtmu
)

dFdx = sp.diff(F, x)
d2Fdx2 = sp.diff(F, x, 2)

dCax2dx   = sp.diff(Cax2, x)
dSax2dx   = sp.diff(Sax2, x)
d2Cax2dx2 = sp.diff(Cax2, x, 2)
d2Sax2dx2 = sp.diff(Sax2, x, 2)

Aa = sp.Abs(a)
Ax = sp.Abs(x)
SAa = sp.sqrt(Aa)
sx = sp.sign(x)

# dFdx = (sigma0 * x * (1 - ax2 * s(x)) + (1-r0*a) * x2 * c(x) + r0) 
# ^^^ derivative stated by Conway & Prussing
# d2Fdx2 = sp.diff(dFdx, x)

dCax2dx = 
    ax2 > 0?
      sin(SAa*Ax)*sx/(x2*SAa) - 2*(1 - cos(SAa*Ax))/(x3*Aa)
    : sinh(SAa*Ax)*sx/(x2*SAa) - 2*(-1 + cosh(SAa*Ax))/(x3*Aa)

dSax2dx = 
    ax2 > 0?
      (-cos(SAa*Ax)*SAa*sx + SAa*sx)*Ax/(x4*SAa**3) + (-sin(SAa*Ax) + SAa*Ax)*sx/(x4*SAa**3) - 4*(-sin(SAa*Ax) + SAa*Ax)*Ax/(x5*SAa**3) 
    : (cosh(SAa*Ax)*SAa*sx - SAa*sx)*Ax/(x4*SAa**3) + (sinh(SAa*Ax) - SAa*Ax)*sx/(x4*SAa**3) - 4*(sinh(SAa*Ax) - SAa*Ax)*Ax/(x5*SAa**3)

d2Cax2dx2 = 
    ax2 > 0? 
      (2*sin(SAa*Ax)*DiracDelta(x)/SAa + cos(SAa*Ax)*sx**2 - 4*sin(SAa*Ax)*sx/(x*SAa) + 6*(1 - cos(SAa*Ax))/(x2*Aa))/x2, 
    : (2*sinh(SAa*Ax)*DiracDelta(x)/SAa + cosh(SAa*Ax)*sx**2 - 4*sinh(SAa*Ax)*sx/(x*SAa) - 6*(1 - cosh(SAa*Ax))/(x2*Aa))/x2

d2Sax2dx2 = 
    ax2 > 0? 
      (-2*(sin(SAa*Ax) - SAa*Ax)*DiracDelta(x)/Aa**(3/2) - 2*(cos(SAa*Ax) - 1)*sx**2/Aa + (sin(SAa*Ax)*Aa*sx**2 - 2*cos(SAa*Ax)*SAa*DiracDelta(x) + 2*SAa*DiracDelta(x))*Ax/Aa**(3/2) + 8*(sin(SAa*Ax) - SAa*Ax)*sx/(x*Aa**(3/2)) + 8*(cos(SAa*Ax) - 1)*Ax*sx/(x*Aa) - 20*(sin(SAa*Ax) - SAa*Ax)*Ax/(x2*Aa**(3/2)))/x4
    : (2*(sinh(SAa*Ax) - SAa*Ax)*DiracDelta(x)/Aa**(3/2) + 2*(cosh(SAa*Ax) - 1)*sx**2/Aa + (sinh(SAa*Ax)*Aa*sx**2 + 2*cosh(SAa*Ax)*SAa*DiracDelta(x) - 2*SAa*DiracDelta(x))*Ax/Aa**(3/2) - 8*(sinh(SAa*Ax) - SAa*Ax)*sx/(x*Aa**(3/2)) - 8*(cosh(SAa*Ax) - 1)*Ax*sx/(x*Aa) + 20*(sinh(SAa*Ax) - SAa*Ax)*Ax/(x2*Aa**(3/2)))/x4

