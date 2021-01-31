#pragma once

namespace compound
{
namespace property
{
/*
NOTE: 
The namespace here serves as a "category of primitives" where objects are variables of primitive types.
We may consider moving to the academic layer if we discover its implementation can be done in a general way.

This namespace maps to and from different elastic moduli.

it namely implements this table: https://en.wikipedia.org/wiki/Elastic_modulus
we may consider coupling it with data from http://dx.doi.org/10.5061/dryad.h505v in the future

The parameters are abbreviated as follows:

bulk     K    bulk modulus
tensile  E    tensile Modulus, Young's modulus, modulus of elasticity
lame     λ    Lamé's first parameter
shear    G,μ  shear modulus, dynamic viscosity, or modulus of rigidity
poisson  ν    Poisson's ratio
pwave    M    P-wave modulus
*/


// (K,E)->* 
constexpr float get_lame_from_bulk_and_tensile ( const float bulk, const float tensile) { return ( 3.0f*bulk*(3.0f*bulk-tensile) )/( 9.0f*bulk-tensile )                                                       ;}
constexpr float get_shear_from_bulk_and_tensile ( const float bulk, const float tensile) { return ( 3.0f*bulk*tensile )/( 9.0f*bulk-tensile )                                                                  ;}
constexpr float get_poisson_from_bulk_and_tensile ( const float bulk, const float tensile) { return ( 3.0f*bulk-tensile )/( 6.0f*bulk )                                                                      ;}
constexpr float get_pwave_from_bulk_and_tensile ( const float bulk, const float tensile) { return ( 3.0f*bulk*(3.0f*bulk+tensile) )/( 9.0f*bulk-tensile )                                                      ;}

// (K,λ)->* 
constexpr float get_tensile_from_bulk_and_lame ( const float bulk, const float lame) { return ( 9.0f*bulk*(bulk-lame) )/( 3.0f*bulk-lame )                                                               ;}
constexpr float get_shear_from_bulk_and_lame ( const float bulk, const float lame) { return ( 3.0f*(bulk-lame) )/( 2.0f )                                                                              ;}
constexpr float get_poisson_from_bulk_and_lame ( const float bulk, const float lame) { return ( lame )/( 3.0f*bulk-lame )                                                                              ;}
constexpr float get_pwave_from_bulk_and_lame ( const float bulk, const float lame) { return 3.0f*bulk-2.0f*lame                                                                                        ;}

// (K,G)->* 
constexpr float get_tensile_from_bulk_and_shear ( const float bulk, const float shear) { return ( 9.0f*bulk*shear )/( 3.0f*bulk+shear )                                                                  ;}
constexpr float get_lame_from_bulk_and_shear ( const float bulk, const float shear) { return bulk - ( 2.0f*shear )/( 3.0f )                                                                            ;}
constexpr float get_poisson_from_bulk_and_shear ( const float bulk, const float shear) { return ( 3.0f*bulk-2.0f*shear )/( 2.0f*(3.0f*bulk+shear) )                                                    ;}
constexpr float get_pwave_from_bulk_and_shear ( const float bulk, const float shear) { return bulk + ( 4.0f*shear )/( 3.0f )                                                                           ;}

// (K,ν)->* 
constexpr float get_tensile_from_bulk_and_poisson ( const float bulk, const float poisson) { return 3.0f*bulk*(1.0f-2.0f*poisson)                                                                        ;}
constexpr float get_lame_from_bulk_and_poisson ( const float bulk, const float poisson) { return ( 3.0f*bulk*poisson )/( 1.0f+poisson )                                                                ;}
constexpr float get_shear_from_bulk_and_poisson ( const float bulk, const float poisson) { return ( 3.0f*bulk*(1.0f-2.0f*poisson) )/( 2.0f*(1.0f+poisson) )                                            ;}
constexpr float get_pwave_from_bulk_and_poisson ( const float bulk, const float poisson) { return ( 3.0f*bulk*(1.0f-poisson) )/( 1.0f+poisson )                                                        ;}

// (K,M)->* 
constexpr float get_tensile_from_bulk_and_pwave ( const float bulk, const float pwave) { return ( 9.0f*bulk*(pwave-bulk) )/( 3.0f*bulk+pwave )                                                           ;}
constexpr float get_lame_from_bulk_and_pwave ( const float bulk, const float pwave) { return ( 3.0f*bulk-pwave )/( 2.0f )                                                                              ;}
constexpr float get_shear_from_bulk_and_pwave ( const float bulk, const float pwave) { return ( 3.0f*(pwave-bulk) )/( 4.0f )                                                                           ;}
constexpr float get_poisson_from_bulk_and_pwave ( const float bulk, const float pwave) { return ( 3.0f*bulk-pwave )/( 3.0f*bulk+pwave )                                                                ;}

// (E,λ)->* 
// NOTE: these functions cannot be marked constexpr due to their use of `sqrt`, however we still want to convey they should be inlined
inline float get_bulk_from_tensile_and_lame ( const float tensile, const float lame) { float R = sqrt(tensile*tensile + 9*lame*lame + 2*tensile*lame ); return ( tensile + 3.0f*lame + R )/( 6.0f )             ;}
inline float get_shear_from_tensile_and_lame ( const float tensile, const float lame) { float R = sqrt(tensile*tensile + 9*lame*lame + 2*tensile*lame ); return ( tensile-3.0f*lame+R )/( 4.0f )                ;}
inline float get_poisson_from_tensile_and_lame ( const float tensile, const float lame) { float R = sqrt(tensile*tensile + 9*lame*lame + 2*tensile*lame ); return ( 2.0f*lame )/( tensile+lame+R )              ;}
inline float get_pwave_from_tensile_and_lame ( const float tensile, const float lame) { float R = sqrt(tensile*tensile + 9*lame*lame + 2*tensile*lame ); return ( tensile-lame+R )/( 2.0f )                     ;}

// (E,G)->* 
constexpr float get_bulk_from_tensile_and_shear ( const float tensile, const float shear) { return ( tensile*shear )/( 3.0f*(3.0f*shear-tensile) )                                                             ;}
constexpr float get_lame_from_tensile_and_shear ( const float tensile, const float shear) { return ( shear*(tensile-2.0f*shear) )/( 3.0f*shear-tensile )                                                       ;}
constexpr float get_poisson_from_tensile_and_shear ( const float tensile, const float shear) { return ( tensile )/( 2.0f*shear)-1.0f                                                                         ;}
constexpr float get_pwave_from_tensile_and_shear ( const float tensile, const float shear) { return ( shear*(4.0f*shear-tensile) )/( 3.0f*shear-tensile )                                                      ;}

// (E,ν)->* 
constexpr float get_bulk_from_tensile_and_poisson ( const float tensile, const float poisson) { return ( tensile )/( 3.0f*(1.0f-2.0f*poisson) )                                                              ;}
constexpr float get_lame_from_tensile_and_poisson ( const float tensile, const float poisson) { return ( tensile*poisson )/( (1.0f+poisson)(1.0f-2.0f*poisson) )                                             ;}
constexpr float get_shear_from_tensile_and_poisson ( const float tensile, const float poisson) { return ( tensile )/( 2.0f*(1.0f+poisson) )                                                                  ;}
constexpr float get_pwave_from_tensile_and_poisson ( const float tensile, const float poisson) { return ( tensile*(1.0f-poisson) )/( (1.0f+poisson)(1.0f-2.0f*poisson) )                                     ;}

// (E,M)->* 
// NOTE: There are two valid solutions, such that S can be positive or negative. To prevent introducing dependencies we represent them as separate functions
// NOTE: these functions cannot be marked constexpr due to their use of `sqrt`, however we still want to convey they should be inlined
inline float get_bulk1_from_tensile_and_pwave ( const float tensile, const float pwave) { float S = sqrt(tensile*tensile + 9*pwave*pwave - 10.0f*tensile*pwave); return ( 3.0f*pwave-tensile+S )/( 6.0f )        ;}
inline float get_lame1_from_tensile_and_pwave ( const float tensile, const float pwave) { float S = sqrt(tensile*tensile + 9*pwave*pwave - 10.0f*tensile*pwave); return ( pwave-tensile+S )/( 4.0f )             ;}
inline float get_shear1_from_tensile_and_pwave ( const float tensile, const float pwave) { float S = sqrt(tensile*tensile + 9*pwave*pwave - 10.0f*tensile*pwave); return ( 3.0f*pwave+tensile-S )/( 8.0f )       ;}
inline float get_poisson1_from_tensile_and_pwave ( const float tensile, const float pwave) { float S = sqrt(tensile*tensile + 9*pwave*pwave - 10.0f*tensile*pwave); return ( tensile-pwave+S )/( 4.0f*pwave )    ;}
inline float get_bulk2_from_tensile_and_pwave ( const float tensile, const float pwave) { float S = -sqrt(tensile*tensile + 9*pwave*pwave - 10.0f*tensile*pwave); return ( 3.0f*pwave-tensile+S )/( 6.0f )        ;}
inline float get_lame2_from_tensile_and_pwave ( const float tensile, const float pwave) { float S = -sqrt(tensile*tensile + 9*pwave*pwave - 10.0f*tensile*pwave); return ( pwave-tensile+S )/( 4.0f )             ;}
inline float get_shear2_from_tensile_and_pwave ( const float tensile, const float pwave) { float S = -sqrt(tensile*tensile + 9*pwave*pwave - 10.0f*tensile*pwave); return ( 3.0f*pwave+tensile-S )/( 8.0f )       ;}
inline float get_poisson2_from_tensile_and_pwave ( const float tensile, const float pwave) { float S = -sqrt(tensile*tensile + 9*pwave*pwave - 10.0f*tensile*pwave); return ( tensile-pwave+S )/( 4.0f*pwave )    ;}

// (λ,G)->* 
constexpr float get_bulk_from_lame_and_shear ( const float lame, const float shear) { return lame+ ( 2.0f*shear )/( 3.0f )                                                                             ;}
constexpr float get_tensile_from_lame_and_shear ( const float lame, const float shear) { return ( shear*(3.0f*lame + 2.0f*shear) )/( lame + shear )                                                      ;}
constexpr float get_poisson_from_lame_and_shear ( const float lame, const float shear) { return ( lame )/( 2.0f*(lame + shear) )                                                                       ;}
constexpr float get_pwave_from_lame_and_shear ( const float lame, const float shear) { return lame+2.0f*shear                                                                                          ;}

// (λ,ν)->* 
constexpr float get_bulk_from_lame_and_poisson ( const float lame, const float poisson) { return ( lame*(1.0f+poisson) )/( 3.0f*poisson )                                                              ;}
constexpr float get_tensile_from_lame_and_poisson ( const float lame, const float poisson) { return ( lame*(1.0f+poisson)(1.0f-2.0f*poisson) )/( poisson )                                               ;}
constexpr float get_shear_from_lame_and_poisson ( const float lame, const float poisson) { return ( lame*(1.0f-2.0f*poisson) )/( 2.0f*poisson )                                                        ;}
constexpr float get_pwave_from_lame_and_poisson ( const float lame, const float poisson) { return ( lame*(1.0f-poisson) )/( poisson )                                                                  ;}

// (λ,M)->* 
constexpr float get_bulk_from_lame_and_pwave ( const float lame, const float pwave) { return ( pwave + 2.0f*lame )/( 3.0f )                                                                            ;}
constexpr float get_tensile_from_lame_and_pwave ( const float lame, const float pwave) { return ( (pwave-lame)(pwave+2.0f*lame) )/( pwave+lame )                                                         ;}
constexpr float get_shear_from_lame_and_pwave ( const float lame, const float pwave) { return ( pwave-lame )/( 2.0f )                                                                                  ;}
constexpr float get_poisson_from_lame_and_pwave ( const float lame, const float pwave) { return ( lame )/( pwave+lame )                                                                                ;}

// (G,ν)->* 
constexpr float get_bulk_from_shear_and_poisson ( const float shear, const float poisson) { return ( 2.0f*shear*(1.0f+poisson) )/( 3.0f*(1.0f-2.0f*poisson) )                                          ;}
constexpr float get_tensile_from_shear_and_poisson ( const float shear, const float poisson) { return 2.0f*shear*(1.0f+poisson)                                                                          ;}
constexpr float get_lame_from_shear_and_poisson ( const float shear, const float poisson) { return ( 2.0f* shear poisson )/( 1.0f-2.0f*poisson )                                                       ;}
constexpr float get_pwave_from_shear_and_poisson ( const float shear, const float poisson) { return ( 2.0f*shear*(1.0f-poisson) )/( 1.0f-2.0f*poisson )                                                ;}

// (G,M)->* 
constexpr float get_bulk_from_shear_and_pwave ( const float shear, const float pwave) { return pwave - ( 4.0f*shear )/( 3.0f )                                                                         ;}
constexpr float get_tensile_from_shear_and_pwave ( const float shear, const float pwave) { return ( shear*(3.0f*pwave-4.0f*shear) )/( pwave-shear )                                                      ;}
constexpr float get_lame_from_shear_and_pwave ( const float shear, const float pwave) { return pwave - 2.0f*shear                                                                                      ;}
constexpr float get_poisson_from_shear_and_pwave ( const float shear, const float pwave) { return ( pwave - 2.0f*shear )/( 2.0f*pwave - 2.0f*shear )                                                   ;}

// (ν,M)->* 
constexpr float get_bulk_from_poisson_and_pwave ( const float poisson, const float pwave) { return ( pwave*(1.0f+poisson) )/( 3.0f*(1.0f-poisson) )                                                    ;}
constexpr float get_tensile_from_poisson_and_pwave ( const float poisson, const float pwave) { return ( pwave*(1.0f+poisson)(1.0f-2.0f*poisson) )/( 1.0f-poisson )                                       ;}
constexpr float get_lame_from_poisson_and_pwave ( const float poisson, const float pwave) { return ( pwave*poisson )/( 1.0f-poisson )                                                                  ;}
constexpr float get_shear_from_poisson_and_pwave ( const float poisson, const float pwave) { return ( pwave*(1.0f-2.0f*poisson) )/( 2.0f*(1.0f-poisson) )                                              ;}

    }
}