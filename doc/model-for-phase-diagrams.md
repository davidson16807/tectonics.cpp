vaporization:
`1/T = -(R/L)*lnP + b`
where
`b = 1/T0 + (R/L)*lnP`
`R = Rᵤ/M`
`Rᵤ= 8.3144598 J/(mol*K)`

    this is known as the "Clapeyron equation"
    L is specific latent heat (kJ/kg)
    R is the molar gas constant (8.3144598 J /K /mol)
    M is the molar mass of the substance (kg/mol)
    m is the slope for ln(P) in relation to 1/T
    b is the intercept for ln(P), the value ln(P) when T is very large, so maybe ln(Pc) or greater

melting:
`T = T0 ((P-P0)/a + 1)^(1/c)`

    this is known as the "Simon-Glatzel equation", an empirical relation

So for a typical substance we can fully characterize the phase diagram using only the following parameters:
* T0    temperature at the triple point
* P0    pressure at the triple point
* a     Simon-Glatzel parameter
* c     Simon-Glatzel parameter
* A     Simon-Glatzel alternate parameter, assuming linear relation where c cannot be determined
* L     specific latent heat of vaporization (J/kg)
* R     individual gas constant (J/(kgK))
* Tc    temperature at the critical point
* Pc    pressure at the critical point

   
Substance           T0      P0(kPa)    a(bars)  c        L(J/kg)   M(kg/mol) m = -R/L  b       Tc     Pc(kPa)   
Hydrogen            13.84   7.04       274.22   1.74407  455000    0.002016  -0.009064 0.1564  33.20  1300     
Helium              2.1768  5.048      50.96    1.5602   20700     0.004     -0.080453 1.1557  5.19   227      
Carbon(vapor)       4765    10132                        26546807  0.012     -2.63e-5  6.3e-4  
Carbon(liquid)      4765    10132                        19810105  0.012     -3.49e-5  1.0e-3  
Nitrogen            63.1604 12.6       1606.56  1.7910   199000    0.028     -0.001379 0.0287  126.2  3390     
Oxygen              54.383  0.152      2732.95  1.7425   213000    0.032     -0.002121 0.0384  154.6  5050     
Argon               83.812  68.9       2114     1.593    6447      0.04      -0.001311 0.0268  150.8  4870     
Iron                1805               1070000  1.76     6090e3    0.056     -2.81e-5  5.2e-4  8500   >510000 lower bound for Pc is based on critical point of gold
Methane             90.66   11.7       A = 36.629        112000    0.016     -0.000140 0.0085  190.8  4640    
Ammonia             195.30  6.060      5270     4.3      1369000   0.017     -0.000386 0.0087  405.5  11280    
Water I(vapor)      273.15  0.6116     -3952    9.0      2501000   0.018     -0.000190 0.0048  647.1  22060
Water I(liquid)     273.15  0.6116     -3952    9.0      2501000             -0.000668 0.0169  647.1  22060
Water III           251.20             620      60                                             
Water V             256.2              4100     8.1                                            
Water VI            273.32             7070     4.46                                           
Water VII           354.80             12980    3.11                                           
Ethane              90.35   8e-4       A = 61.4          487645.2  0.03      -0.001277 0.0223  305.3  4890   
Carbon Monoxide     68.146  15.37      A = 45.27         216000    0.028     -0.001374 0.0132  132.85 3500    m and b constructed from L and M
Carbon Dioxide      216.56  517        4000     2.60     205000    0.044     -0.000465 0.0107  304.19 7380    
Nitric Oxide        109.50  21.92      A = 82.29         461076.8  0.030     -0.000601 0.0031  180.15 6486    m and b constructed from L and M
Sulfur dioxide      197.69  1.67                         381464    0.064     -0.000340 0.0025  430.65 7873    m and b constructed from L and M
Tetraflouromethane   89.54  0.1012                       137000    0.088     -0.000689 0.0080  227.6  3793    m and b constructed from L and M
                                                                                                     
Cyanide             259.86                               1030000                                        456.65 5400
Formaldehyde        155.10                                                                              137.2  6788
Ethanol             150     4.3e-7                       855000    180.50                               514    6300
Formic Acid         281.4   2.2                                                                                    





phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i], -0.080453  , 1.1557 , 50.96e5 , 1.5602 , 5.048e3  , 2.1768  , 227e3     , 5.19   , TRUE , TRUE ) }) #Helium                                           
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i], -0.001379  , 0.0287 , 1606.5e5, 1.7910 , 12.6e3   , 63.1604 , 3390e3    , 126.2  , TRUE , TRUE ) }) #Nitrogen                                         
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i], -0.002121  , 0.0384 , 2732.9e5, 1.7425 , 0.152e3  , 54.383  , 5050e3    , 154.6  , TRUE , TRUE ) }) #Oxygen                                           
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i], -0.001311  , 0.0268 , 2114e5  , 1.593  , 68.9e3   , 83.812  , 4870e3    , 150.8  , TRUE , TRUE ) }) #Argon                                            
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i], -2.81e-5   , 5.2e-4 , 1.07e11 , 1.76   , 0         , 1805    , 510000e3 , 8500   , FALSE, TRUE ) }) #Iron              lower bound for Pc is based on critical point of gold                                 
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i], -0.000140  , 0.0085 , 36.629e5, 0      , 11.7e3   , 90.66   , 4640e3    , 190.8  , FALSE, TRUE ) }) #Methane                                         
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i], -0.000386  , 0.0087 , 5270e5  , 4.3    , 6.060e3  , 195.30  , 11280e3   , 405.5  , TRUE , TRUE ) }) #Ammonia                                          
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i], -0.000190  , 0.0048 , -3952e5 , 9.0    , 0.6116e3 , 273.15  , 22060e3   , 647.1  , TRUE , TRUE ) }) #Water I(vapor) 
plot(t,p,col=phase+1, pch=15)
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i], -0.001277  , 0.0223 , 61.4e5,  0       , 8e-1     , 90.35   , 4890e3    , 305.3  , FALSE, TRUE ) }) #Ethane                                         
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i],            ,        , 45.27e5, 0       , 15.37e3  , 68.146  , 3500e3    , 132.85 , FALSE, TRUE ) }) #Carbon Monoxide                                 
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i], -0.000465  , 0.0107 , 4000e5  , 2.60   , 517e3    , 216.56  , 7380e3    , 304.19 , TRUE , TRUE ) }) #Carbon Dioxide                                  
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i],            ,        , 82.29e5, 0       , 87.85e3  , 182.34  , 7240e3    , 309.5  , FALSE, TRUE ) }) #Nitrous Oxide                                   
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i],            ,        ,                  , 1.67e3   , 197.69  , 7873e3    , 430.65 , TRUE , TRUE ) }) #Sulfur dioxide                                
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i],            ,        ,                  , 26e3     , 173.08  , 4059e3    , 374.21 , TRUE , TRUE ) }) #Perflouromethane  (properties are actually Hexaflouroethane or R-134A)                                

phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i], -2.61e-5   , 6.3e-4 ,         ,        , 10132e3  , 4765    ,           ,        , TRUE , FALSE) }) #Carbon(vapor)                                      
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i], -e.48e-5   , 1.0e-3 ,         ,        , 10132e3  , 4765    ,           ,        , TRUE , FALSE) }) #Carbon(liquid)                                    
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i],            ,        , 620e5   , 60     ,          , 251.20  ,           ,        , TRUE , FALSE) }) #Water III                                       
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i],            ,        , 4100e5  , 8.1    ,          , 256.2   ,           ,        , TRUE , FALSE) }) #Water V                                         
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i],            ,        , 7070e5  , 4.46   ,          , 273.32  ,           ,        , TRUE , FALSE) }) #Water VI                                        
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i],            ,        , 12980e5 , 3.11   ,          , 354.80  ,           ,        , TRUE , FALSE) }) #Water VII                                        

phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i],            ,        ,                  ,          , 259.86  , 5400e3    , 456.65 , TRUE , TRUE ) }) #Cyanide                                       
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i],            ,        ,                  ,          , 155.10  , 6788e3    , 137.2  , TRUE , TRUE ) }) #Formaldehyde                                  
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i],            ,        ,                  , 4.3e-4   , 150     , 6300e3    , 514    , TRUE , TRUE ) }) #Ethanol                                       
phase=sapply(seq(p), function(i){ get_basic_phase( p[i], t[i],            ,        ,                  , 2.2e3    , 281.4   ,           ,        , TRUE , FALSE) }) #Formic Acid