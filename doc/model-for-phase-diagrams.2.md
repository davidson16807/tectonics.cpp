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
* Tc    temperature at the critical point
* Pc    pressure at the critical point
* Tf    temperature at melting point at STP, used to 
* L     specific latent heat of vaporization (J/kg)
* M     individual gas constant (J/(kgK))
* a     Simon-Glatzel parameter
* c     Simon-Glatzel parameter
* has_simon_glatzel_parameters
* has_critical_point
   
P0(kPa) , T0      , Pc(Pa)  , Tc     , Pf(Pa), Tf     , L(J/kg)  , M(kg/mol) , a       , c       # Substance         
0.6116e3, 273.15  , 22060e3 , 647.1  , 10132 , 273.15 , 2501000  , 0.018     , -3952   , 9.0     # Water I(vapor)    
0.6116e3, 273.15  , 22060e3 , 647.1  , 10132 ,        , 2501000  ,           , -3952   , 9.0     # Water I(liquid)   
        , 251.20  ,         ,        , 10132 ,        ,          ,           , 620     , 60      # Water III         
        , 256.2   ,         ,        , 10132 ,        ,          ,           , 4100    , 8.1     # Water V           
        , 273.32  ,         ,        , 10132 ,        ,          ,           , 7070    , 4.46    # Water VI          
        , 354.80  ,         ,        , 10132 ,        ,          ,           , 12980   , 3.11    # Water VII         
10132e3 , 4765    ,         ,        , 10132 , 3800   , 26546807 , 0.012     ,         ,         # Carbon(vapor)     
10132e3 , 4765    ,         ,        , 10132 , 3800   , 19853056 , 0.012     ,         ,         # Carbon(liquid)    
7.04e3  , 13.84   , 1300e3  , 33.20  , 10132 , 13.99  , 455000   , 0.002016  , 274.22  , 1.74407 # Hydrogen          
5.048e3 , 2.1768  , 227e3   , 5.19   , 10132 , 0.95   , 20700    , 0.004     , 50.96   , 1.5602  # Helium            
12.6e3  , 63.1604 , 3390e3  , 126.2  , 10132 , 63.15  , 199000   , 0.028     , 1606.56 , 1.7910  # Nitrogen          
0.152e3 , 54.383  , 5050e3  , 154.6  , 10132 , 54.36  , 213000   , 0.032     , 2732.95 , 1.7425  # Oxygen            
68.9e3  , 83.812  , 4870e3  , 150.8  , 10132 , 83.8   , 6447     , 0.04      , 2114    , 1.593   # Argon             
        , 1805    , 510000e3, 8500   , 10132 , 1811   , 6090e3   , 0.056     , 1070000 , 1.76    # Iron                 lower bound for Pc is based on critical point of gold
11.7e3  , 90.66   , 4640e3  , 190.8  , 10132 , 90.68  , 112000   , 0.016     , 2080    , 1.698   # Methane           
6.060e3 , 195.30  , 11280e3 , 405.5  , 10132 , 195.4  , 1369000  , 0.017     , 5270    , 4.3     # Ammonia           
0.0014e3, 90.35   , 4890e3  , 305.3  , 10132 , 90.36  , 487645.2 , 0.03      ,         ,         # Ethane            
15.37e3 , 68.146  , 3500e3  , 132.85 , 10132 , 68.15  , 216000   , 0.028     ,         ,         # Carbon Monoxide   
517e3   , 216.56  , 7380e3  , 304.19 , 10132 , 216.6  , 205000   , 0.044     , 4000    , 2.60    # Carbon Dioxide    
87.85e3 , 182.34  , 7240e3  , 309.5  , 10132 , 182.2  , 376141.6 , 0.044     ,         ,         # Nitric Oxide     
1.67e3  , 197.69  , 7873e3  , 430.65 , 10132 , 200.2  , 381464   , 0.064     ,         ,         # Sulfur dioxide    
0.1012e3, 89.54   , 3793e3  , 227.6  , 10132 , 89.15  , 137000   , 0.088     ,         ,         # Tetraflouromethane
0.153e3 , 259.86  , 5400e3  , 456.65 , 10132 , 259.7  , 1030000  , 0.026019  , 3080    , 3.6     # Cyanide           
        , 155.10  , 6788e3  , 137.2  , 10132 , 181    , 775994   , 0.03      ,         ,         # Formaldehyde      
4.3e-4  , 150     , 6300e3  , 514    , 10132 , 159.2  , 855000   , 0.046     , 10600   , 1.61    # Ethanol           
2.2e3   , 281.4   , 5810e3  , 588    , 10132 , 281.5  , 437000   , 0.046     , 4100    , 5.2     # Formic Acid       
                                                        600000   , 0.060     ,                   # Silica

