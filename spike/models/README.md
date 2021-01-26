The model is implemented at all levels of abstraction using pure functions or, when pressed for performance, regular functions (as defined by Stepanov) that use reference parameters or owner objects to store output. Functions and objects are organized into mathematical categories. 

At the highest level of abstraction, the model can be thought to implement the commutative ciagram [here](https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZABgBoBmAXVJADcBDAGwFcYkQAFAGQHEQBfUuky58hFGQBM1Ok1bsAagH0AjAKEgM2PASJkArDIYs2iTrwA6FgCIxGOegAIAkjnXDtYoiopG5pkABhXmcAFXdNER1xZB9pGmN5MwBlCK1RXRQfFT8Tdmc0qK8USV8E-3YAIULPTORS+Nk8s2VJGoyY8jKmpJAAaXboogAWbsSAvscrPABbeEcARUHi5FHh3N6lwQ8OkdJDcuaQK1CACxgHKYtZ+asuehmAIygnaaw5uEcAJWW6-VJ1odNlcbp8thp0kMUF1AT0AgAJEHvebgnZQ5BdA5w9gnc4OX4xHxY8Y4mx2fHbSK1GKlYkVMyVJEfK5nC70AneUgANg2ASstnsr2uyM+-PJ7MpkJWXR5QL5FlZlzezPCkqKdR8sJJZmCPGqaupRFKsuxZgAghyUP86UcfgbdlbubzSfcni9LcguU65exAJBkTPmbnt6K9Nt6XwDnwUkccQYh6piAHZ9s6zABRGNWGbMGNxtErZMm7XHCwMABOMDQ2EYBBjpxjADom7mPQAOFM+sxWDhYTMWbMt4MF0jEVMgC1Durt0edzhKGYxssx2ge5M5Wf6+OGlDJxrFtNt0jr01BXiOSpKQLnpRca88RyqrcO5Dtvf0kD0GC9qwQOYAcyFAB5f8JRkGAoD-eAiFAAAzMtfyQUoQBwCAkGISk4IQxAumQ1DEGGDD4JmJBRlwpB9EIrCyDIxAVEo4jEGolCkBw856CgdgcAAdwgNioAQGg4FOLAYLcRAAFo6I0TCGJw5jEC5eikC9GjEyU7CaHk6ihJEsT0OkojlM0vDW3UgBOYykBUfTYMM2ifBo6z1JUJD5KcgysJUOS8JUAiPIYlQVLcij-Ks5NHJC2ysNcnz-hARgsDAAIoAgHAcHAiIZKs9tHL8qKAosiLnO8qzqISpL2BStKMvMyzEBy8rktS9KOOcoKfLykAsvsurfPUyQmNi-qSsQSRJH60j5MkBzGsq5qatC0a4qm0idNEpBJImurJDitaxM2xbJBGo6aBrCA0G8VsyBgpg4BgGRGHoR47A4BN2DLLA-1OPMursyRJrw-7Togc7Luu277tOp6XreswPq+n7uskdrEN24T1okqT8tR7aVL2jasd+6LlsBtTDvCqbwrOi6siu0gbsYO6HuhxhXu3EB4e+zK7JUByppmxKmuq1rFt57akNmswqpa7nopyqbCslkAUuYR5GDYGg+PYSAKs0+gsEYbWCDYdTyEGliJcFubVfVkBNZgdijd15D9cNswdZNxbyD5vDyBwpWVbVjWQC193jbtl2Dadz3sdolGFNOq2pfmkXY5UQr5JUgOU4ESh+CAA), which can also be viewed with the following [tikzcd](http://ctan.math.washington.edu/tex-archive/graphics/pgf/contrib/tikz-cd/tikz-cd-doc.pdf) markup:


```
% https://tikzcd.yichuanshen.de/#N4Igdg9gJgpgziAXAbVABwnAlgFyxMJZABgBoBmAXVJADcBDAGwFcYkQAFAGQHEQBfUuky58hFGQBM1Ok1bsAagH0AjAKEgM2PASJkArDIYs2iTrwA6FgCIxGOegAIAkjnXDtYoiopG5pkABhXmcAFXdNER1xZB9pGmN5MwBlCK1RXRQfFT8Tdmc0qK8USV8E-3YAIULPTORS+Nk8s2VJGoyY8jKmpJAAaXboogAWbsSAvscrPABbeEcARUHi5FHh3N6lwQ8OkdJDcuaQK1CACxgHKYtZ+asuehmAIygnaaw5uEcAJWW6-VJ1odNlcbp8thp0kMUF1AT0AgAJEHvebgnZQ5BdA5w9gnc4OX4xHxY8Y4mx2fHbSK1GKlYkVMyVJEfK5nC70AneUgANg2ASstnsr2uyM+-PJ7MpkJWXR5QL5FlZlzezPCkqKdR8sJJZmCPGqaupRFKsuxZgAghyUP86UcfgbdlbubzSfcni9LcguU65exAJBkTPmbnt6K9Nt6XwDnwUkccQYh6piAHZ9s6zABRGNWGbMGNxtErZMm7XHCwMABOMDQ2EYBBjpxjADom7mPQAOFM+sxWDhYTMWbMt4MF0jEVMgC1Durt0edzhKGYxssx2ge5M5Wf6+OGlDJxrFtNt0jr01BXiOSpKQLnpRca88RyqrcO5Dtvf0kD0GC9qwQOYAcyFAB5f8JRkGAoD-eAiFAAAzMtfyQUoQBwCAkGISk4IQxAumQ1DEGGDD4JmJBRlwpB9EIrCyDIxAVEo4jEGolCkBw856CgdgcAAdwgNioAQGg4FOLAYLcRAAFo6I0TCGJw5jEC5eikC9GjEyU7CaHk6ihJEsT0OkojlM0vDW3UgBOYykBUfTYMM2ifBo6z1JUJD5KcgysJUOS8JUAiPIYlQVLcij-Ks5NHJC2ysNcnz-hARgsDAAIoAgHAcHAiIZKs9tHL8qKAosiLnO8qzqISpL2BStKMvMyzEBy8rktS9KOOcoKfLykAsvsurfPUyQmNi-qSsQSRJH60j5MkBzGsq5qatC0a4qm0idNEpBJImurJDitaxM2xbJBGo6aBrCA0G8VsyBgpg4BgGRGHoR47A4BN2DLLA-1OPMursyRJrw-7Togc7Luu277tOp6XreswPq+n7uskdrEN24T1okqT8tR7aVL2jasd+6LlsBtTDvCqbwrOi6siu0gbsYO6HuhxhXu3EB4e+zK7JUByppmxKmuq1rFt57akNmswqpa7nopyqbCslkAUuYR5GDYGg+PYSAKs0+gsEYbWCDYdTyEGliJcFubVfVkBNZgdijd15D9cNswdZNxbyD5vDyBwpWVbVjWQC193jbtl2Dadz3sdolGFNOq2pfmkXY5UQr5JUgOU4ESh+CAA
\begin{tikzcd}
                                               &                                     &                                                        &                                        &                                                                                         &                      &                                                                                                        & A \arrow[r, no head, Rightarrow]                                            & P_m \times r \times v                                              \\
                                               & I                                   &                                                        &                                        &                                                                                         &                      &                                                                                                        & B \arrow[r, no head, Rightarrow]                                            & CLG B_C B_L B_G T                                                  \\
V_1                                            & S \arrow[u]                         & V_2                                                    &                                        &                                                                                         &                      &                                                                                                        & E \arrow[r, no head, Rightarrow]                                            & aei \omega \Omega                                                  \\
PLG \arrow[u] \arrow[r, two heads, shift left] & CLGIT \arrow[u] \arrow[r] \arrow[l] & B \arrow[u] \arrow[r]                                  & K                                      & K \times Q \arrow[d] \arrow[l, dotted]                                                  &                      &                                                                                                        &                                                                             &                                                                    \\
                                               & CLGB \arrow[d] \arrow[ru, dotted]   &                                                        & H \times Q \arrow[d] \arrow[r, dotted] & Q                                                                                       & Q \times Q \arrow[l] &                                                                                                        &                                                                             &                                                                    \\
PLG\Delta It \arrow[uu] \arrow[r, dotted]      & \Delta                              & B \times \Theta \arrow[l] \arrow[r] \arrow[uu, dotted] & \Theta                                 & \Theta \times \Lambda \times R \arrow[u] \arrow[l] \arrow[r, dotted] \arrow[rd, dotted] & R                    & R \times V \times t \arrow[l, dotted] \arrow[r, shift left] \arrow[loop, distance=2em, in=125, out=55] & E \times \mu \times t \arrow[l, shift left] \arrow[d, shift left] \arrow[r] & \Pi \times \mu \times t \arrow[loop, distance=2em, in=125, out=55] \\
                                               & \Delta \times \Delta \arrow[u]      & A \arrow[lu]                                           & \Theta \times T \arrow[u]              &                                                                                         & \Lambda              & ☉ \times t \arrow[l] \arrow[loop, distance=2em, in=125, out=55]                                        & \varepsilon \times h \times ... \times t \arrow[u, shift left]              &                                                                   
\end{tikzcd}
```

Identity arrows are omitted. Dotted arrows indicate trivial product morphisms, which are still implied if omitted. Products are depicted using concatenation where needed for brevity. 

Objects within the commutative diagram represent classes within the implementation. 

The most important objects in the diagram are as follows. 
* `P` plates                                                                    raster of columns with variable width layers and bitmask
* `C` global crust                                                              raster of columns with variable width layers
* `L` liquid mass pools                                                         scalars mapping to mass pools
* `G` gaseous mass pools                                                        scalars mapping to mass pools
* `V*`view layers
* `B` boundary indicators for rock strata, oceans, and atmosphere
* `K` thermal conductivity and heat capacitance                                 fixed-width layered raster
* `Q` heat transfer                                                             fixed-width layered raster
* `H` thermal energy                                                            fixed-width layered raster
* `Θ` temperature                                                               fixed-width layered raster
* `Λ` solar radiation                                                           scalars mapping to bins of fixed wavelength
* `R` placement of celestial bodies                                             
* `Δ` deltas indicating rates of change
* `t` timestep                                                                  scalar

And the following are objects that play secondary roles:
* `T` tables of chemical properties
* `I` indexes indicating the grid cells of global crust that belong to plates
* `S` mass-spring representation of crust
* `☉` mass pools of stars and other state variables
* `V` velocities of celestial bodies
* `E` orbital elements of celestial bodies
* `ε` orbital energies of celestial bodies
* `Π` periods of celestial cycles
* `μ` mean anomaly
* `A` asteroid impact

Importance here is defined subjectively, but is meant to help concentrate your attention.