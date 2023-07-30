#pragma once

#define TEST_NUDGE(dx)    [=](auto x){ return x+(dx); }
#define TEST_LOWER(lo)    [=](auto x){ return (lo) <= x; }
#define TEST_UPPER(hi)    [=](auto x){ return x <= hi; }
#define TEST_RANGE(lo,hi) [=](auto x){ return lo <= x&&x <= hi; }
#define TEST_UNARY(f)     [=](auto x){ return (f(x)); }
#define TEST_BINARY(f)    [=](auto x, auto y){ return (f(x,y)); }
#define TEST_TRINARY(f)   [=](auto x, auto y){ return (f(x,y,z)); }

