The "orbit" namespace exclusively represents data structures and functions that define mathematical categories.

The data structures are as follows:
* **Elements** The six orbital elements, including mean anomaly to track position in the orbit. This allows updating a satellite's position over time simply by updating mean anomaly, which is linear to time. 
* **State** A position and velocity vector. This allows applying a force to a satellite simply by updating its velocity.

An application can switch back and forth between "State" and "Element" data structures, using whichever is appropriate for the problem at hand.

The categories are as follows:
* **Properties** A category implemented as a class with conceptually pure methods mapping properties of an orbit
* **ElementAndState** A category where homsets are classes with conceptually pure methods mapping the data structures above
* **orbit** A category where arrows are the constructors of class categories, indicating dependencies


The "universal" namespace exclusively represents mathematical categories that describe orbits using the universal variable formulation, as discussed in "Orbital Mechanics" by Prussing and Conway. 

The categories are as follows:
* **Properties** A category implemented as a class with conceptually pure methods mapping properties of an orbit
* **ElementAndState** A category where homsets are classes with conceptually pure methods mapping the data structures above
* **universal** A category where arrows are the constructors of class categories, indicating dependencies

```
% https://q.uiver.app/?q=WzAsNyxbMSwwLCJcXGJvbGR7cl8wdl8wfXQiXSxbMSwxLCJcXGJvbGR7cnZ9Il0sWzIsMSwiXFxib2xke3J2XFxEZWx0YSB2fSJdLFsyLDAsIlxcYm9sZHtyXzB2XzB9dFxcYm9sZHtcXERlbHRhIHZ9ICJdLFswLDAsIlxcYm9sZHtyXzB2XzB9dFxcRGVsdGEgdCJdLFsxLDIsIlxcYm9sZHtyXzB2XzB9dCJdLFswLDEsIlxcYm9sZHtydn1cXERlbHRhIHQiXSxbMCwxXSxbMiwxXSxbMywwLCIiLDAseyJzdHlsZSI6eyJib2R5Ijp7Im5hbWUiOiJkYXNoZWQifX19XSxbMywyXSxbMSw1XSxbNCwwXSxbNiwxLCIiLDIseyJzdHlsZSI6eyJib2R5Ijp7Im5hbWUiOiJkYXNoZWQifX19XSxbNCw2LCIiLDEseyJzdHlsZSI6eyJib2R5Ijp7Im5hbWUiOiJkYXNoZWQifX19XV0=
\begin{tikzcd}
	{\bold{r_0v_0}t\Delta t} & {\bold{r_0v_0}t} & {\bold{r_0v_0}t\bold{\Delta v} } \\
	{\bold{rv}\Delta t} & {\bold{rv}} & {\bold{rv\Delta v}} \\
	& {\bold{r_0v_0}t}
	\arrow[from=1-2, to=2-2]
	\arrow[from=2-3, to=2-2]
	\arrow[dashed, from=1-3, to=1-2]
	\arrow[from=1-3, to=2-3]
	\arrow[from=2-2, to=3-2]
	\arrow[from=1-1, to=1-2]
	\arrow[dashed, from=2-1, to=2-2]
	\arrow[dashed, from=1-1, to=2-1]
\end{tikzcd}
```