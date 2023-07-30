#pragma once

/*
`macros.hpp` is an optional header that contains macros 
that are defined for convenience to simplify specification 
of lambdas that follow very regular formats.

Macros are used since some language constructs such as 
class methods and operators cannot be passed directly as parameters,
and defining lambdas that wraps these constructs 
would make it harder to read and write unit tests.

Since the intended use case is restricted to a test environment,
the risks associated with macros are reduced 
to the point where ease of use is preferred.
*/

#define TEST_NUDGE(DX)     [=](auto x){ return x+(DX); }
#define TEST_LOWER(LO)     [=](auto x){ return (LO) <= x; }
#define TEST_UPPER(HI)     [=](auto x){ return x <= HI; }
#define TEST_RANGE(LO,HI)  [=](auto x){ return LO <= x&&x <= HI; }
#define TEST_UNARY(F)      [=](auto x){ return (F(x)); }
#define TEST_BINARY(F)     [=](auto x, auto y){ return (F(x,y)); }
#define TEST_TRINARY(F)    [=](auto x, auto y, auto z){ return (F(x,y,z)); }
#define TEST_SYMBOL(SYMBOL)[=](auto x, auto y){ return (x SYMBOL y); }

