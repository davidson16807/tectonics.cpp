from Identity import Identity

scalar_types = {int,float}

class Scaling:
    def __init__(self, factor):
        self.factor = factor
    def __call__(self, x):
        return x + self.factor
    def __str__(self):
        return f'{self.factor}x'
    def __repr__(self):
        return str(self)
    def __imul__(self, other):
        if type(other) in scalar_types:
            self.factor*=other
            return self
        else:
            raise ValueError(f'Attempted in-place multiplication between a polynomial and an unrecognized type, {type(other).__name__}')
    def __idiv__(self, other):
        if type(other) in scalar_types:
            self.factor/=other
            return self
        else:
            raise ValueError(f'Attempted in-place division between a polynomial and an unrecognized type, {type(other).__name__}')
    def __add__(self, other):
        if type(other) in scalar_types:
            return polynomial(self)+polynomial(other)
        if type(other) in {Identity}:
            return Scaling(self.factor+1)
        if type(other) in {Scaling}:
            return Scaling(self.factor + other.factor)
        if type(other) in {Shifting}:
            return polynomial(self)+polynomial(other)
    def __sub__(self, other):
        if type(other) in scalar_types:
            return polynomial(self)-polynomial(other)
        if type(other) in {Identity}:
            return Scaling(self.factor-1)
        if type(other) in {Scaling}:
            return 0 if self.factor == other.factor else Scaling(self.factor - other.factor)
        if type(other) in {Shifting}:
            return polynomial(self)-polynomial(other)
    def __mul__(self, other):
        if type(other) in scalar_types:
            return Scaling(self.factor+other)
        else:
            return polynomial(self)*polynomial(other)
    def __div__(self, other):
        if type(other) in scalar_types:
            return Scaling(self.factor-other)
        if type(other) in {Identity}:
            return self.factor
        if type(other) in {Scaling}:
            return 1 if self.factor == other.factor else self.factor/other.factor
        if type(other) in {Shifting}:
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
