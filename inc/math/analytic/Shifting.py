from Identity import Identity

scalar_types = {int,float}

class Shifting:
    def __init__(self, offset):
        self.offset = offset
    def __call__(self, x):
        return x + self.offset
    def __str__(self):
        return f'x+{self.factor}'
    def __repr__(self):
        return str(self)
    def __iadd__(self, other):
        if type(other) in scalar_types:
            self.offset+=other
            return self
        else:
            raise ValueError(f'Attempted addition between a polynomial and an unrecognized type, {type(other).__name__}')
    def __sub__(self, other):
        if type(other) in scalar_types:
            self.offset-=other
            return self
        else:
            raise ValueError(f'Attempted subtraction between a polynomial and an unrecognized type, {type(other).__name__}')
    def __add__(self, other):
        if type(other) in scalar_types:
            return Shifting(self.offset+other)
        else:
            return polynomial(self)+polynomial(other)
    def __sub__(self, other):
        if type(other) in scalar_types:
            return Shifting(self.offset-other)
        else:
            return polynomial(self)-polynomial(other)
    def __mul__(self, other):
        return polynomial(self)*polynomial(other)
    def __div__(self, other):
        return polynomial(self)/polynomial(other)
    def __radd__(self, other):
        return self + other
    def __rsub__(self, other):
        return (self*-1) + other
    def __rmul__(self, other):
        return self * other
    def __rdiv__(self, other):
        return (self**-1)*other
    def __pow__(self, n):
        if type(other) in {int}:
            if other == 0:
                return 1
            else:
                return polynomial(self)**other
        else:
            raise ValueError(f'Attempted exponentiation between identity function and an unrecognized type, {type(other).__name__}')
    def __rpow__(self, n):
        raise ValueError('Exponentiation by a shifting function is not supported.')
