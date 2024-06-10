The "orbit" namespace exclusively represents data structures and functions that define mathematical categories.

The data structures are as follows:
* **Elements** The six orbital elements, including mean anomaly to track position in the orbit. This allows updating a satellite's position over time simply by updating mean anomaly, which is linear to time. 
* **State** A position and velocity vector. This allows applying a force to a satellite simply by updating its velocity.

An application can switch back and forth between "State" and "Element" data structures, using whichever is appropriate for the problem at hand.

The categories are as follows:
* **Properties** A category implemented as a class with conceptually pure methods mapping properties of an orbit
* **ElementAndState** A category where homsets are classes with conceptually pure methods mapping the data structures above
* **orbit** A category where arrows are the constructors of class categories, indicating dependencies

